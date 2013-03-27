#include "CommunicationData.hpp"
namespace sim_mob
{

void subscriptionInfo::setEntity(sim_mob::Entity* value)
{
	agent = value;
}
sim_mob::Entity* subscriptionInfo::getEntity()
{
	return agent;
}
//we use original dataMessage(or DATA_MSG) type to avoid wrong read/write
DataContainer& subscriptionInfo::getIncoming() {
	ReadLock Lock(*myLock);
	return incoming;
}
DataContainer& subscriptionInfo::getOutgoing() {
	ReadLock Lock(*myLock);
	return outgoing;
}
void subscriptionInfo::setIncoming(DataContainer values) {
	WriteLock(*myLock);
	incoming = values;
	incomingIsDirty = true;
}
void subscriptionInfo::setOutgoing(DataContainer values) {
	WriteLock(*myLock);
	outgoing = values;
	outgoingIsDirty = true;}

void subscriptionInfo::addIncoming(DATA_MSG_PTR value) {
	WriteLock(myLock);
	incoming.add(value);
	incomingIsDirty = true;
}
void subscriptionInfo::addOutgoing(DATA_MSG_PTR value) { std::cout << "pushing data to " << &outgoing << std::endl;
WriteLock(*myLock);
outgoing.add(value);
outgoingIsDirty = true;
}

void subscriptionInfo::setwriteIncomingDone(bool value) {
	WriteLock(*myLock);
	writeIncomingDone = value;
}
void subscriptionInfo::setWriteOutgoingDone(bool value) {
	WriteLock(*myLock);
	readOutgoingDone = value;
}
void subscriptionInfo::setAgentUpdateDone(bool value) {
	WriteLock(*myLock);
	agentUpdateDone = value;
}
bool subscriptionInfo::iswriteIncomingDone() {
	ReadLock Lock(*myLock);
	return writeIncomingDone;
}
bool subscriptionInfo::isreadOutgoingDone() {
	ReadLock Lock(*myLock);
	return readOutgoingDone;
}
bool subscriptionInfo::isAgentUpdateDone() {
	ReadLock Lock(*myLock);
	return agentUpdateDone;
}

bool subscriptionInfo::isOutgoingDirty() {
	ReadLock Lock(*myLock);
	return outgoingIsDirty;
}
bool subscriptionInfo::isIncomingDirty() {
	ReadLock Lock(*myLock);
	return incomingIsDirty;
}

void subscriptionInfo::reset()
{
	WriteLock Lock(*myLock);
	incomingIsDirty = false ;
	outgoingIsDirty = false ;
	agentUpdateDone = false ;
	readOutgoingDone = false ;
	writeIncomingDone = false ;
	incoming.reset();
	outgoing.reset();
	cnt_1 = cnt_2 = 0;
}
}
