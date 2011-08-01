#include "Loader.hpp"

#include "soci/soci.h"
#include "soci/postgresql/soci-postgresql.h"

#include "Node.hpp"
#include "SOCI_Converters.hpp"


using namespace sim_mob::aimsun;
using std::vector;


namespace {

void LoadNodes(std::vector<Node>& nodelist)
{
	//Connect
	//NOTE: Our git repository is private (SSH-only) for now, so just storing the password to the DB here.
	soci::session sql(soci::postgresql, "host=localhost port=5432 dbname=SimMobility_DB user=postgres password=S!Mm0bility");

	//Statement
	Node newNode;
	sql <<"select * from get_node();", soci::into(newNode);



	std::streamsize orig = std::cout.precision();
	std::cout.precision(10);
	std::cout <<"Found: " <<newNode.id <<"," <<newNode.xPos <<"," <<newNode.yPos <<"," <<newNode.isIntersection <<"\n";
	std::cout.precision(orig);
}


} //End anon namespace


bool sim_mob::aimsun::Loader::LoadNetwork(sim_mob::RoadNetwork& rn)
{
	//try {
		//Load all nodes
		std::vector<Node> nodelist;
		LoadNodes(nodelist);



	/*} catch (std::exception& ex) {
		//TODO: We can provide ex.what() (the message) to the developer once
		//      we decide if our library will throw exceptions, use error codes,
		//      return pair<bool, string>, etc.
		std::cout <<"Error: " <<ex.what() <<std::endl;

		return false;
	}*/
	return true;
}

