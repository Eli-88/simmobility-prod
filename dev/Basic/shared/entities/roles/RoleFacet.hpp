/* Copyright Singapore-MIT Alliance for Research and Technology */

/*
 * RoleFacet.h
 *
 *  Created on: Mar 21, 2013
 *      Author: harish
 */

#pragma once

#include "conf/settings/DisableMPI.h"
#include "util/LangHelpers.hpp"
#include "entities/Agent.hpp"
#include "entities/vehicle/Vehicle.hpp"
#include "entities/UpdateParams.hpp"
#include "entities/misc/BusTrip.hpp"
#include "boost/thread/thread.hpp"
#include "boost/thread/locks.hpp"
#include "util/OutputUtil.hpp"
#include <boost/random.hpp>

namespace sim_mob {

#ifndef SIMMOB_DISABLE_MPI
class PartitionManager;
class PackageUtils;
class UnPackageUtils;
#endif

namespace {

class RoleFacet {

public:
	//NOTE: Don't forget to call this from sub-classes!
	explicit RoleFacet(sim_mob::Agent* parent = nullptr, std::string roleName = std::string()) :
		parent(parent), currResource(nullptr), name(roleName), dynamic_seed(0) { }

	//Allow propagating destructors
	virtual ~RoleFacet() {}

	//A Role must allow for copying via prototyping; this is how the RoleFactory creates roles.
	virtual Role* clone(Person* parent) const = 0;

	std::string getRoleName()const {return name;}

	///Called the first time an Agent's update() method is successfully called.
	/// This will be the tick of its startTime, rounded down(?).
	virtual void frame_init(UpdateParams& p) = 0;

	///Perform each frame's update tick for this Agent.
	virtual void frame_tick(UpdateParams& p) = 0;

	///Generate output for this frame's tick for this Agent.
	virtual void frame_tick_output(const UpdateParams& p) = 0;

	//generate output with fake attributes for MPI
	virtual void frame_tick_output_mpi(timeslice now) = 0;

	///Create the UpdateParams (or, more likely, sub-class) which will hold all
	///  the temporary information for this time tick.
	virtual UpdateParams& make_frame_tick_params(timeslice now) = 0;

	///Return a list of parameters that expect their subscriptions to be managed.
	/// Agents can append/remove this list to their own subscription list each time
	/// they change their Role.
	virtual std::vector<sim_mob::BufferedBase*> getSubscriptionParams() = 0;

	///Return a request list for asychronous communication.
	///  Subclasses of Role should override this method if they want to enable
	///  asynchronous communication.
	///NOTE: This function is only used by the Driver class, but it's required here
	///      due to the way we split Driver into the short-term folder.
	virtual sim_mob::DriverRequestParams getDriverRequestParams() {
		return sim_mob::DriverRequestParams();
	}

	//NOTE: Should not be virtual; this is a little hackish for now. ~Seth
	virtual Vehicle* getResource() { return currResource; }

	Agent* getParent()
	{
		return parent;
	}

	void setParent(Agent* parent)
	{
		this->parent = parent;
	}

	int getOwnRandomNumber(boost::mt19937& gen)
	{
		int one_try = -1;
		int second_try = -2;
		int third_try = -3;
		//		int forth_try = -4;

		while (one_try != second_try || third_try != second_try)
		{
			//TODO: I've replaced your calls to srand() and rand() (which are not
			//      thread-safe) with boost::random.
			//      This is likely to not work the way you want it to.
			//      Please read the boost::random docs. ~Seth
			boost::uniform_int<> dist(0, RAND_MAX);

			one_try = dist(gen);

			second_try = dist(gen);

			third_try = dist(gen);

//			if (one_try != second_try || third_try != second_try)
//			{
//				LogOut("Random:" << this->getParent()->getId() << "," << one_try << "," << second_try << "," << third_try << "\n");
//			}
//			else
//			{
//				LogOut("Random:" << this->getParent()->getId() << ",Use Seed:" << dynamic_seed << ", Get:" << one_try << "," << second_try<< "," << third_try<< "\n");
//			}
		}

		dynamic_seed = one_try;
		return one_try;
	}

	const std::string name;
protected:
	Agent* parent; ///<The owner of this role. Usually a Person, but I could see it possibly being another Agent.

	Vehicle* currResource; ///<Roles may hold "resources" for the current task. Expand later into multiple types.

	//add by xuyan
protected:
	int dynamic_seed;

	//Random number generator
	//TODO: We need a policy on who can get a generator and why.
	//boost::mt19937 gen;

public:
#ifndef SIMMOB_DISABLE_MPI
	friend class sim_mob::PartitionManager;
#endif

	//Serialization
#ifndef SIMMOB_DISABLE_MPI
public:
	virtual void pack(PackageUtils& packageUtil) = 0;
	virtual void unpack(UnPackageUtils& unpackageUtil) = 0;

	virtual void packProxy(PackageUtils& packageUtil) = 0;
	virtual void unpackProxy(UnPackageUtils& unpackageUtil) = 0;
#endif


};

} /* namespace sim_mob */
