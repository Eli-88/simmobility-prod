//Copyright (c) 2013 Singapore-MIT Alliance for Research and Technology
//Licensed under the terms of the MIT License, as described in the file:
//   license.txt   (http://opensource.org/licenses/MIT)

/*
 * ClientHandler.cpp
 *
 *  Created on: May 28, 2013
 *      Author: vahid
 */

#include "ClientHandler.hpp"
#include "entities/commsim/broker/Broker.hpp"
#include "entities/commsim/event/JsonSerializableEventArgs.hpp"

using namespace sim_mob;

sim_mob::ClientHandler::ClientHandler(sim_mob::Broker & broker_) :
	broker(broker_), valid(true), AgentCommUtility_(nullptr), agent(nullptr),
	client_type(0)
{
}

sim_mob::ClientHandler::~ClientHandler()
{
}

sim_mob::Broker& sim_mob::ClientHandler::getBroker()
{
	return broker;
}

/*void ClientHandler::OnTime(sim_mob::event::EventId id, sim_mob::event::Context context, sim_mob::event::EventPublisher* sender, const TimeEventArgs& args){
//   std::string timeJson = args.ToJSON();
   Json::Value timeJson = args.ToJSON();
   //now send to broker's buffer
   getBroker().insertSendBuffer(cnnHandler, timeJson);*/
void sim_mob::ClientHandler::OnEvent(sim_mob::event::EventId id, sim_mob::event::Context context, sim_mob::event::EventPublisher* sender, const sim_mob::comm::JsonSerializableEventArgs& argums)
{
	//now send to broker's buffer
	//std::cout <<"XXX\n" <<argums.toJSON().toStyledString() <<"\nXXXY\n";
	getBroker().insertSendBuffer(cnnHandler, argums.toJSON());
}

bool sim_mob::ClientHandler::isValid()
{
	return valid;
}

void sim_mob::ClientHandler::setValidation(bool value)
{
	valid = value;
}
