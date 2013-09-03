//Copyright (c) 2013 Singapore-MIT Alliance for Research and Technology
//Licensed under the terms of the MIT License, as described in the file:
//   license.txt   (http://opensource.org/licenses/MIT)

#pragma once

#include <vector>
#include <map>
#include <string>

#include "buffering/Shared.hpp"
#include "conf/CMakeConfigParams.hpp"
#include "conf/Constructs.hpp"
#include "entities/AuraManager.hpp"
#include "geospatial/Point2D.hpp"
#include "workers/WorkGroup.hpp"
#include "util/DailyTime.hpp"

//TODO: Need to move the useful "Constructs" out of this file (e.g., Passwords)
#include "conf/Constructs.hpp"

namespace sim_mob {

//helper class: to be moved.
struct FMOD_ControllerParams {
	FMOD_ControllerParams() : enabled(false), port(0), updateTimeMS(0), blockingTimeSec(0) {}

	bool enabled;
	std::string ipAddress;
	unsigned int port;
	unsigned int updateTimeMS;
	std::string mapfile;
	unsigned int blockingTimeSec;
};


//helper class: to be moved.
struct BusStopScheduledTime {
	BusStopScheduledTime() : offsetAT(0), offsetDT(0) {}

	unsigned int offsetAT; //<Presumably arrival time?
	unsigned int offsetDT; //<Presumably departure time?
};


//helper class. Note that this is somewhat different from the DatabaseConnection class, as it stores credentials elsewhere.
class Database : public Identifiable {
public:
	Database(const std::string& id="") : Identifiable(id) {}

	std::string host;
	std::string port;
	std::string dbName;
};


//helper class: to be moved.
class Credential : public Identifiable {
public:
	Credential(const std::string& id="") : Identifiable(id) {}

	std::string getUsername() {
		return username;
	}

	std::string getPassword(bool mask=true) {
		if (mask) {
			return std::string(password.size(), '*');
		} else {
			return password;
		}
	}

	///Helper: load FileCredentials
	static void LoadFileCredentials(const std::vector<std::string>& paths);
	static void SetPlaintextCredentials(const std::string& username, const std::string& password);

protected:
	std::string username;
	std::string password;
};




//helper class: to be moved.
class Constructs {
public:
	//std::map<std::string, Distribution> distributions; //<TODO
	std::map<std::string, Database> databases;
	std::map<std::string, StoredProcedureMap> procedureMaps;
	std::map<std::string, Credential> credentials;
};


//helper class: to be moved.
class SimulationParams {
public:
	SimulationParams();

	///Sources of Agents.
	enum LoadAgentsOrderOption {
		LoadAg_Drivers,       ///<Load Drivers from the config file.
		LoadAg_Pedestrians,   ///<Load Pedestrians from the config file.
		LoadAg_Passengers,    ///<Load Passengers from the config file.
		LoadAg_Database,      ///<Load Trip-Chain based entities from the database.
		LoadAg_XmlTripChains, ///<Not sure what this does exactly....
	};

	///Our reaction time distributions.
	struct ReactionTimeDistDescription {
		ReactionTimeDistDescription() : typeId(0), mean(0), stdev(0) {}

		int typeId;
		int mean;
		int stdev;
	};

	unsigned int baseGranMS;       ///<Base system granularity, in milliseconds. Each "tick" is this long.
	unsigned int totalRuntimeMS;   ///<Total time (in milliseconds) to run the simulation for. (Includes "warmup" time.)
	unsigned int totalWarmupMS;    ///<Total time (in milliseconds) considered "warmup".

	DailyTime simStartTime; ///<When the simulation begins(based on configuration)

	AuraManager::AuraManagerImplementation auraManagerImplementation; ///<What type of Aura Manager we're using.

	WorkGroup::ASSIGNMENT_STRATEGY workGroupAssigmentStrategy;  ///<Defautl assignment strategy for Workgroups.

	int partitioningSolutionId;  ///<Property specific to MPI version; not fully documented.

	std::vector<LoadAgentsOrderOption> loadAgentsOrder; ///<What order to load agents in.

	int startingAutoAgentID; ///<Default starting ID for agents with auto-generated IDs.

	sim_mob::MutexStrategy mutexStategy; ///<Locking strategy for Shared<> properties.

	bool commSimEnabled;  ///<Is our communication simulator enabled?
	bool androidClientEnabled; ///<Is the Android client for our communication simulator enabled?

	//Reaction time parameters.
	//TODO: This should be one of the first areas we clean up.
	ReactionTimeDistDescription reactTimeDistribution1;
	ReactionTimeDistDescription reactTimeDistribution2;

    //Passenger distribution parameters.
    //TODO: This should be the second thing we clean up.
    int passenger_distribution_busstop;
    int passenger_mean_busstop;
    int passenger_standardDev_busstop;
    int passenger_percent_boarding;
    int passenger_percent_alighting;
    int passenger_min_uniform_distribution;
    int passenger_max_uniform_distribution;
};

//helper class: to be moved.
class WorkerParams {
public:
	struct Worker {
		Worker();
		unsigned int count;
		unsigned int granularityMs;
	};

	Worker person;
	Worker signal;
	Worker communication;
};


//helper class: to be moved.
class SystemParams {
public:
	SystemParams();

	enum NetworkSource {
		NETSRC_XML,
		NETSRC_DATABASE,
	};

	SimulationParams simulation;
	WorkerParams workers;

	bool singleThreaded; ///<If true, we are running everything on one thread.
	bool mergeLogFiles;  ///<If true, we take time to merge the output of the individual log files after the simulation is complete.

	NetworkSource networkSource; ///<Whethere to load the network from the database or from an XML file.
	std::string networkXmlFile;  ///<If loading the network from an XML file, which file? Empty=data/SimMobilityInput.xml

	std::string roadNetworkXsdSchemaFile; ///<Valid path to a schema file for loading XML road network files.

	std::map<std::string, std::string> genericProps; ///<Generic properties, for testing new features.
};


//helper class: to be moved.
class GeometryParams {
public:
	DatabaseConnection connection;
	StoredProcedureMap procedures;
};


//helper class: to be moved.
struct EntityTemplate {
	EntityTemplate();
	Point2D originPos;
	Point2D destPos;
	unsigned int startTimeMs;
};


/**
 * Contains the properties of the config file as they appear in, e.g., test_road_network.xml, with
 *   minimal conversion.
 * Derived properties (such as the road network) are listed in ConfigParams.
 *
 * \author Seth N. Hetu
 */
class RawConfigParams : public sim_mob::CMakeConfigParams {
public:
	RawConfigParams();

	///"Sytem" level settings, including simulation parameters and global flags.
	SystemParams system;

	///Available gemetries (currently only database geometries).
	GeometryParams geometry;

	///Settings for the FMOD controller.
	FMOD_ControllerParams fmod;

	///Some settings for bus stop arrivals/departures.
	std::map<int, BusStopScheduledTime> busScheduledTimes; //The int is a "bus stop ID", starting from 0.

	//@{
	///Templates for creating entities of various types.
	std::vector<EntityTemplate> driverTemplates;
	std::vector<EntityTemplate> pedestrianTemplates;
	std::vector<EntityTemplate> busDriverTemplates;
	std::vector<EntityTemplate> signalTemplates;
	std::vector<EntityTemplate> passengerTemplates;
	std::vector<EntityTemplate> busControllerTemplates;
	//@}
};


}
