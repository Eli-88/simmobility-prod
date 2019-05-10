//Copyright (c) 2013 Singapore-MIT Alliance for Research and Technology
//Licensed under the terms of the MIT License, as described in the file:
//   license.txt   (http://opensource.org/licenses/MIT)

#pragma once

#include "Driver.hpp"
#include "entities/controllers/MobilityServiceController.hpp"
#include "entities/controllers/MobilityServiceControllerManager.hpp"
#include "entities/mobilityServiceDriver/MobilityServiceDriver.hpp"
#include "entities/roles/passenger/Passenger.hpp"
#include "OnCallDriverFacets.hpp"
using namespace std;
namespace sim_mob
{
namespace medium
{

class OnCallDriver : public Driver, public MobilityServiceDriver
{
private:
    /**Stores the passengers that are in the vehicle*/
    std::unordered_map<std::string, Passenger *> passengers;

    /**Stores the controllers that the driver is subscribed to*/
    std::vector<MobilityServiceController *> subscribedControllers;

    /**Indicates whether the driver is to be removed from the parking*/
    bool toBeRemovedFromParking;

    /**Indicates that the driver is exiting a parking node to go to a pickup node
     * (or a drop off node if the parking and pick up nodes were the same)*/
    bool isExitingParking;


    //track how many persons are dropped off by this driver from the time driver available  to till now
    int passengerInteractedDropOff =0 ;

    /**Indicates whether the controller needs to be notified about the completion of
    * the current item
    */
    bool informController = true;

protected:
    /**Pointer to the on call driver's movement facet object*/
    OnCallDriverMovement *movement;

    /**Pointer to the on call driver's behaviour facet object*/
    OnCallDriverBehaviour *behaviour;

    /**Indicates whether the driver is waiting for an acknowledgement from the controller
     * regarding successful unsubscription*/
    bool isWaitingForUnsubscribeAck;

    /**Indicates that the schedule has been set or updated by the controller*/
    bool isScheduleUpdated;

    /**Stores items which are served together at the same node*/
    std::multimap<ScheduleItem, ScheduleItem> sameNodeItems;

    /**Wrapper for the schedule that has been given by the controller. */
    struct DriverSchedule
    {
    private:
        /**Stores the schedule currently assigned to the driver*/
        Schedule assignedSchedule;

        /**Points to the current schedule item being performed*/
        Schedule::const_iterator currentItem;

        /**Points to the next schedule item to be performed*/
        Schedule::const_iterator nextItem;
        Schedule prevSchedule;

        /**Points to the node where driver completed its last schedule item*/
        const Node *completedNode = nullptr;

    public:
        void setSchedule(const Schedule &newSchedule)
        {
            assignedSchedule = newSchedule;
            currentItem = assignedSchedule.begin();
            nextItem = currentItem + 1;
        }

        void updateSchedule(const Schedule &updatedSchedule)
        {
            auto currIt = *currentItem;
            assignedSchedule = updatedSchedule;
            assignedSchedule.insert(assignedSchedule.begin(), currIt);
            currentItem = assignedSchedule.begin();
            nextItem = currentItem + 1;
        }

        const Schedule& getSchedule() const
        {
            return assignedSchedule;
        }

        Schedule::const_iterator getCurrScheduleItem() const
        {
            return currentItem;
        }
        Schedule& getPrevSchedule()
        {
            return prevSchedule;
        }

        void  setPrevSchedule(const Schedule & thisScheduleItem)
        {
            prevSchedule = thisScheduleItem;
        }
        Schedule::const_iterator getNextScheduleItem() const
        {
            return nextItem;
        }

        void itemCompleted()
        {
            auto item = assignedSchedule.begin();
            completedNode = item->getNode();
            setPrevSchedule(assignedSchedule);
            assignedSchedule.erase(item);
            currentItem = assignedSchedule.begin();
            nextItem = currentItem + 1;
        }

        bool isScheduleCompleted() const
        {
            return currentItem == assignedSchedule.end();
        }

        void setCompletedNode()
        {
            completedNode = nullptr;
        }

        bool isSameNodeItem(const Node *node) const
        {
            return node == completedNode;
        }
    } driverSchedule;

    /**
     * Marks the current schedule item as completed
     */
    void scheduleItemCompleted();

    /**
     * Checks if the current item in the schedule has been rescheduled
     * @param updatedSchedule  the updated schedule
     * @return if the current item in the schedule has been rescheduled,
     * the respective iterator is returned.
     */
    const bool currentItemRescheduled(Schedule &updatedSchedule);

    /*
     * Performs the schedule item at the head of the schedule, without
     * informing the controller. It is completed in the same tick.
     */
    void immediatelyPerformItem();
    /**
     * Sends the schedule received acknowledgement message
     * @param success indicates whether the schedule can be performed
     */
    void sendScheduleAckMessage(bool success);

    /**
     * Sends the driver available message to the controllers
     */
    void sendAvailableMessage();

    /**
     * Sends the driver status update message
     */
    void sendStatusMessage();

    /**
     * When an error in the schedule is detected, this function sends a
     * message to the controller, forcing the controller to update its
     * copy.
     */
    void sendSyncMessage();

    /**
     * Sends the message, indicating that the shift has ended and it has to wake up. This is
     * required to wake up drivers who are parked when their shift ends
     */
    void sendWakeUpShiftEndMsg();

public:
    OnCallDriver(Person_MT *parent, const MutexStrategy &mtx,
                 OnCallDriverBehaviour *behaviour, OnCallDriverMovement *movement,
                 std::string roleName,
                 Role<Person_MT>::Type = Role<Person_MT>::RL_ON_CALL_DRIVER);

    OnCallDriver(Person_MT *parent);

    virtual ~OnCallDriver();

    /**
     * Clones the on call driver object
     * @param person the person who will take on the cloned role
     * @return the cloned on call driver role
     */
    virtual Role<Person_MT> *clone(Person_MT *person) const;

    /**
     * Message handler to provide a chance to handle message forwarded by the parent person.
     * @param type type of the message.
     * @param message the message containing the required data.
     */
    virtual void HandleParentMessage(messaging::Message::MessageType type, const messaging::Message &message);

    /**
     * Looks through the current schedule for consecutive items which are served at the same node.
     * This method populates the multimap setSameNodeItems as well.
     * @param updatedSchedule schedule provided by the controller which needs to be checked
     * @param controller sender of the new schedule
     * @param hasExistingSchedule true if called due to MSG_SCHEDULE_UPDATE message, false if called
     * due to MSG_SCHEDULE_PROPOSITION
     * @return true if the current item needs to be preempted
     */
    void setSharedSchedule(Schedule &updatedSchedule, messaging::MessageHandler *controller = nullptr,
            const bool hasExistingSchedule = false);

    /**
     * Scans the driver's current passengers and checks if the new schedule contains a pickup that
     * has already been performed.
     * @param newSchedule the new schedule sent by the controller
     * @return true if there is a problem in the new schedule
     */
    const bool checkForRepeatedPickups(Schedule &newSchedule) const;

    /**
     * The current node of the driver is the node which has most recently been crossed by the driver
     * This method retrieves the node using the current segment from the path mover object.
     * @return current node if available, else nullptr
     */
    virtual const Node* getCurrentNode() const;

    /**
     * Set Current Node .It will call the Driver movement Set Current Node function to set.
     */
    void  setCurrentNode(const Node* thsNode);

    /**
     * Obtain number of passengers assigned to the driver
     */
    virtual const unsigned getNumAssigned() const;

    /**
     * Recursive function that traverses the multimap sameNodeItems and returns number of dropoffs in the schedule
     * @param item schedule item which needs serves as the key to the multimap
     * @return count of dropoffs in the multimap with the given key
     */
    virtual const unsigned getNumDropoffs(const ScheduleItem item) const;

    /**
     * @return vector of controllers that the driver has subscribed to
     */
    virtual const std::vector<MobilityServiceController *>& getSubscribedControllers() const;

    /**
     * Overrides the parent function. As on call drivers follow a schedule, this method will
     * return the current schedule assigned to the driver
     * @return
     */
    virtual Schedule getAssignedSchedule() const;

    /**
    * There is no travel time collection for on call drivers. This method does nothing
    */
    virtual void collectTravelTime();

    /**
     * @return the number of passengers in the vehicle.
     */
    virtual unsigned long getPassengerCount() const;

    /**
     * Export service driver
     * @return exporting result
     */
    virtual const MobilityServiceDriver *exportServiceDriver() const;

    /**
     * Sends a subscription message to the controller the driver subscribes to
     */
    void subscribeToController();

    /**
     * Picks up the passenger
     */
    void pickupPassenger();

    /**
     * Drops off the passenger
     */
    void dropoffPassenger();

    /**
     * Performs the tasks required to end the driver shift
     */
    void endShift();

    const bool isToBeRemovedFromParking() const
    {
        return toBeRemovedFromParking;
    }

    void setToBeRemovedFromParking(bool value)
    {
        toBeRemovedFromParking = value;
    }

    friend class OnCallDriverMovement;
    friend class OnCallDriverBehaviour;

    string getPassengersId() const;

    void removeDriverEntryFromAllContainer();

    bool isPassengerExistInVehicle(const std::string& userid) const {
        return passengers.find(userid) != passengers.end();
    }

};

}
}