#include "CommunicationSupport.hpp"
#include "Communicator.hpp"


using namespace sim_mob;
namespace sim_mob
{
	CommunicationSupport::CommunicationSupport()
	:
		incomingIsDirty(false),
		outgoingIsDirty(false),
		writeIncomingDone(false),
		readOutgoingDone(false),
		agentUpdateDone(false)
	{

	}

	subscriptionInfo CommunicationSupport::getSubscriptionInfo(){
		subscriptionInfo info(
				(sim_mob::Entity*)0,
				isIncomingDirty(),
				isOutgoingDirty(),
				iswriteIncomingDone(),
				isreadOutgoingDone(),
				isAgentUpdateDone(),
				getIncoming(),
				getOutgoing()
				);
		return info;
	}

	//we use original dataMessage(or DATA_MSG) type to avoid wrong read/write
	DataContainer& CommunicationSupport::getIncoming() {
		ReadLock Lock(myLock);
		return incoming;
	}
	DataContainer& CommunicationSupport::getOutgoing() {
		ReadLock Lock(myLock);
		return outgoing;
	}
	void CommunicationSupport::setIncoming(DataContainer values) {
		WriteLock(myLock);
		incoming = values; }
	void CommunicationSupport::setOutgoing(DataContainer values) {
		WriteLock(myLock);
		outgoing = values;
		outgoingIsDirty = true;}

	void CommunicationSupport::addIncoming(DATA_MSG_PTR value) {
		WriteLock(myLock);
		incoming.add(value); }
	void CommunicationSupport::addOutgoing(DATA_MSG_PTR value) { std::cout << "pushing data to " << &outgoing << std::endl;
	WriteLock(myLock);
	outgoing.add(value); outgoingIsDirty = true;}

	void CommunicationSupport::setwriteIncomingDone(bool value) {
		WriteLock(myLock);
		writeIncomingDone = value;
	}
	void CommunicationSupport::setWriteOutgoingDone(bool value) {
		WriteLock(myLock);
		readOutgoingDone = value;
	}
	void CommunicationSupport::setAgentUpdateDone(bool value) {
		WriteLock(myLock);
		agentUpdateDone = value;
	}
	bool &CommunicationSupport::iswriteIncomingDone() {
		ReadLock Lock(myLock);
		return writeIncomingDone;
	}
	bool &CommunicationSupport::isreadOutgoingDone() {
		ReadLock Lock(myLock);
		return readOutgoingDone;
	}
	bool &CommunicationSupport::isAgentUpdateDone() {
		ReadLock Lock(myLock);
		return agentUpdateDone;
	}

	bool &CommunicationSupport::isOutgoingDirty() {
		ReadLock Lock(myLock);
		return outgoingIsDirty;
	}
	bool &CommunicationSupport::isIncomingDirty() {
		ReadLock Lock(myLock);
		return incomingIsDirty;
	}

//todo
	void CommunicationSupport::clear(){
		WriteLock Lock(myLock);
		outgoingIsDirty = false ;
		incomingIsDirty = false ;
		agentUpdateDone = false ;
		writeIncomingDone = false ;
		readOutgoingDone = false ;
	}
	void CommunicationSupport::init(){

	}
	//this is used to subscribe the drived class
	//(which is also an agent) to the communicator agent
	bool CommunicationSupport::subscribe(sim_mob::Entity* subscriber, sim_mob::NS3_Communicator &communicator = sim_mob::NS3_Communicator::GetInstance())
	{
		//todo here you are copying twice while once is possibl, I guess.
		subscriptionInfo info = getSubscriptionInfo();
		info.setEntity(subscriber);
		std::cout << "Reguesting to  agent[" << info.getEntity() << "] with outgoing[" << &(info.getOutgoing()) << "]" << std::endl;

		communicator.subscribeEntity(info);
	}

};
