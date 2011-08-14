/* Copyright Singapore-MIT Alliance for Research and Technology */

#pragma once

#include <vector>

#include "RoadItem.hpp"


namespace sim_mob
{

//Forward declarations
class Lane;


/**
 * Representation of a Bus Stop.
 */
class BusStop : public sim_mob::RoadItem {
public:
	///Which RoadItem and lane is this bus stop located at?
	Lane* location;

	///Is this a bus bay, or does it take up space on the lane?
	///Bus bays are always to the dominant position away from the lane.
	///So, if drivingSide = OnLeft, then the bay extends to the left in its own lane.
	bool is_bay;

	///Is this a bus terminal? Currently, the only effect this has is to avoid
	///   requiring a bus to wait for the bus in front of it to leave.
	bool is_terminal;

	///How many meters of "bus" can park in the bus lane/bay to pick up pedestrians.
	///  Used to more easily represent double-length or mini-buses.
	unsigned int busCapacityAsLength;

	///Is the pedestrian waiting area sheltered? Currently does not affect anything.
	bool has_shelter;


private:
	///Get the bus lines available at this stop. Used for route planning.
	///NOTE: Placeholder method; will obviously not be returning void.
	void getBusLines() {  }

	///Get a list of bus arrival times. Pedestrians can consult this (assuming the bus stop is VMS-enabled).
	///NOTE: Placeholder method; will obviously not be returning void.
	void getBusArrivalVMS() {  }


};





}
