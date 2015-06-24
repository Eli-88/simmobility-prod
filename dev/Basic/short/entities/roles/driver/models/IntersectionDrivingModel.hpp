//Copyright (c) 2013 Singapore-MIT Alliance for Research and Technology
//Licensed under the terms of the MIT License, as described in the file:
//   license.txt   (http://opensource.org/licenses/MIT)


#pragma once

#include <vector>

#include "conf/params/ParameterManager.hpp"
#include "conf/settings/DisableMPI.h"
#include "entities/roles/driver/DriverUpdateParams.hpp"
#include "geospatial/TurningSection.hpp"
#include "util/DynamicVector.hpp"

#ifndef SIMMOB_DISABLE_MPI
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#endif

namespace sim_mob
{

  class TurningSection;

#ifndef SIMMOB_DISABLE_MPI
  class PackageUtils;
  class UnPackageUtils;
#endif
  
  enum IntModelType
  {
    //Indicates that the object is of the simple intersection driving model class
    Int_Model_Simple = 0,
    
    //Indicates that the object is of the MITSIM intersection driving model class
    Int_Model_MITSIM = 1,
    
    //Indicates that the object is of the slot based intersection driving model class
    Int_Model_SlotBased =2
  };

  /**
   * \author Seth N. Hetu
   */
  class IntersectionDrivingModel
  {
  protected:
    
    //Indicates the type of model being used
    IntModelType modelType;

    //Trajectory of the vehicle in the intersection
    DynamicVector intTrajectory;

    //Distance covered within the intersection
    double totalMovement;

    //Distance at which an intersection and vehicles approaching it from other links
    //is visible to the driver (metre)
    double intersectionVisbility;

    //The current turning
    const TurningSection *currTurning;

  public:
    
    IntersectionDrivingModel() :
    totalMovement(0), intersectionVisbility(50), currTurning(nullptr)
    {
    }

    //Allow propagation of delete
    virtual ~IntersectionDrivingModel()
    {
    }

    //Builds a straight line trajectory form the end point of the entry lane into the intersection
    //to the start point of the exit lane out of the intersection
    virtual void startDriving(const DPoint& fromLanePt, const DPoint& toLanePt, double startOffset) = 0;

    //Moves the vehicle by given amount along the trajectory
    virtual DPoint continueDriving(double amount, DriverUpdateParams& p) = 0;

    //Depending on the conflicting vehicles, calculates the acceleration that allows the vehicle to 
    //pass through without colliding with the other vehicles
    virtual double makeAcceleratingDecision(DriverUpdateParams& params) = 0;

    //Returns the current angle of the vehicle according to the position in the trajectory
    virtual double getCurrentAngle()
    {
      return intTrajectory.getAngle();
    }

    //Returns the distance covered within the intersection
    double getMoveDistance()
    {
      return totalMovement;
    }

    //Checks whether we've completed driving in the intersection
    virtual bool isDone()
    {
      return totalMovement >= intTrajectory.getMagnitude();
    }

    //Getter for the intersection visibility distance
    int getIntersectionVisbility() const
    {
      return intersectionVisbility;
    }

    //Setter for the current turning
    void setCurrTurning(const TurningSection* currTurning)
    {
      this->currTurning = currTurning;
    }
    
    //Getter for the intersection model type
    IntModelType getIntModelType() const
    {
      return modelType;
    }
    
  } ;

  /**
   *
   * Simple version of the intersection driving model
   * The purpose of this model is to demonstrate a very simple (yet reasonably accurate) model
   * which generates somewhat plausible visuals. This model should NOT be considered valid, but
   * it can be used for demonstrations and for learning how to write your own *Model subclasses.
   *
   * \author Seth N. Hetu
   */
  class SimpleIntDrivingModel : public IntersectionDrivingModel
  {
  public:
    
    SimpleIntDrivingModel()
    {
      modelType = Int_Model_Simple;
    }

    virtual double makeAcceleratingDecision(DriverUpdateParams& params)
    {
      return params.maxAcceleration;
    }

    virtual void startDriving(const DPoint& fromLanePt, const DPoint& toLanePt, double startOffset)
    {
      intTrajectory = DynamicVector(fromLanePt.x, fromLanePt.y, toLanePt.x, toLanePt.y);
      totalMovement = startOffset;
    }

    virtual DPoint continueDriving(double amount, DriverUpdateParams& params)
    {
      totalMovement += amount;
      DynamicVector temp(intTrajectory);
      temp.scaleVectTo(totalMovement).translateVect();
      return DPoint(temp.getX(), temp.getY());
    }

    //add by xuyan
#ifndef SIMMOB_DISABLE_MPI
    static void pack(PackageUtils& package, const SimpleIntDrivingModel* params);

    static void unpack(UnPackageUtils& unpackage, SimpleIntDrivingModel* params);
#endif
  } ;

  /*
   * MITSIMLab version of the intersection driving model
   */
  class MITSIM_IntDriving_Model : public IntersectionDrivingModel
  {
  private:

    //The minimum value of the attentiveness factor within an intersection. The factor is a random value between the 
    //given min and max
    double intersectionAttentivenessFactorMin;

    //The maximum value of the attentiveness factor within an intersection. The factor is a random value between the 
    //given min and max
    double intersectionAttentivenessFactorMax;

    //The value of minimum gap
    double minimumGap;

    //Contains the mean and std dev values for generating a random add-on to the critical gap
    double criticalGapAddOn[2];

    //The multiplier for the impatience timer. 
    double impatienceFactor;

    //Stores the poly-points of the turning path
    std::vector<DPoint> polypoints;
    std::vector<DPoint>::iterator polypointIter;

    //Stores the current position in the intersection
    DPoint currPosition;
    DynamicVector currPolyline;

    //Length of the turning
    double length;

    double polylineMovement;

    //Reads and stores the parameters related to intersection driving from the driver parameter xml file
    //(data/driver_param.xml)
    void initParam(DriverUpdateParams& params);

    //Calculates the deceleration needed for the vehicle to come to a stop within a given distance
    double brakeToStop(double distance, DriverUpdateParams& params);

    //Calculate the acceleration needed to crawl
    double crawlingAcc(double distance, DriverUpdateParams& params);

  public:

    MITSIM_IntDriving_Model(DriverUpdateParams& params);

    virtual ~MITSIM_IntDriving_Model();

    //Getter for intersectionAttentivenessFactorMin
    double getIntersectionAttentivenessFactorMin() const;

    //Getter for intersectionAttentivenessFactorMax
    double getIntersectionAttentivenessFactorMax() const;

    //Getter for the impatience factor
    double getImpatienceFactor() const;

    //Builds a straight line trajectory form the end point of the entry lane into the intersection
    //to the start point of the exit lane out of the intersection
    virtual void startDriving (const DPoint& fromLanePt, const DPoint& toLanePt, double startOffset);

    //Moves the vehicle by given amount along the trajectory
    virtual DPoint continueDriving (double amount, DriverUpdateParams& p);

    //Return the current angle
    virtual double getCurrentAngle();

    //Returns true if the vehicle has left the intersection
    virtual bool isDone();

    void makePolypoints(const DPoint& fromLanePt, const DPoint& toLanePt);

    //Depending on the conflicting vehicles, calculates the acceleration that allows the vehicle to 
    //pass through without colliding with the other vehicles
    virtual double makeAcceleratingDecision(DriverUpdateParams& params);
  } ;

  /*
   * Slot-based intersection driving model
   */
  class SlotBased_IntDriving_Model : public MITSIM_IntDriving_Model
  {
  private:

    //Separation time between vehicles following one another (also known as T1)
    double tailgateSeparationTime;

    //Separation time between vehicles with conflicting trajectories (also known as T2)
    double conflictSeparationTime;

    //Reads and stores the parameters related to intersection driving from the driver parameter xml file
    //(data/driver_param.xml)
    void initParam(DriverUpdateParams& params);

  public:

    SlotBased_IntDriving_Model(DriverUpdateParams& params);

    virtual ~SlotBased_IntDriving_Model();

    //Calculates the acceleration required to reach the intersection at the allocated time so as to 
    //avoid collisions with conflicting vehicles
    virtual double makeAcceleratingDecision(DriverUpdateParams& params);
  } ;

}
