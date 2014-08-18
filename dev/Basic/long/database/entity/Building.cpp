//Copyright (c) 2013 Singapore-MIT Alliance for Research and Technology
//Licensed under the terms of the MIT License, as described in the file:
//   license.txt   (http://opensource.org/licenses/MIT)

/* 
 * File:   Building.cpp
 * Author: Pedro Gandola <pedrogandola@smart.mit.edu>
 * 
 * Created on May 8, 2013, 3:04 PM
 */

#include "Building.hpp"

using namespace sim_mob::long_term;

Building::Building( BigSerial fm_building_id, BigSerial fm_project_id, BigSerial fm_parcel_id, int storeys_above_ground, int storeys_below_ground,
					std::string from_date, std::string to_date, std::string building_status, float	gross_sq_m_res, float gross_sq_m_office,
					float gross_sq_m_retail, float gross_sq_m_other ) :
					fm_building_id(fm_building_id), fm_project_id(fm_project_id), fm_parcel_id(fm_parcel_id), storeys_above_ground(storeys_above_ground),
					storeys_below_ground(storeys_below_ground), from_date(from_date), to_date(to_date), building_status(building_status),gross_sq_m_res(gross_sq_m_res),
					gross_sq_m_office(gross_sq_m_office), gross_sq_m_retail(gross_sq_m_retail), gross_sq_m_other(gross_sq_m_other)
{



}

Building::~Building() {
}

Building& Building::operator=(const Building& source) {
	/*
    this->id = source.id;
    this->typeId = source.typeId;
    this->parcelId = source.parcelId;
    this->tenureId = source.tenureId;
    this->builtYear = source.builtYear;
    this->parkingSpaces = source.parkingSpaces;
    this->storeys = source.storeys;
    this->landedArea = source.landedArea;
   	*/

/*
    fm_building_id
    fm_project_id
    fm_parcel_id
    storeys_above_ground
    storeys_below_ground
    from_date
    to_date
    building_status
    gross_sq_m_res
    gross_sq_m_office
    gross_sq_m_retail
    gross_sq_m_other
*/

	this->fm_building_id 		= source.fm_building_id;
	this->fm_project_id 		= source.fm_project_id;
	this->fm_parcel_id 			= source.fm_parcel_id;
	this->storeys_above_ground	= source.storeys_above_ground;
	this->storeys_below_ground  = source.storeys_below_ground;
	this->from_date				= source.from_date;
	this->to_date				= source.to_date;
	this->building_status		= source.building_status;
	this->gross_sq_m_res		= source.gross_sq_m_res;
	this->gross_sq_m_office		= source.gross_sq_m_office;
	this->gross_sq_m_retail		= source.gross_sq_m_retail;
	this->gross_sq_m_other		= source.gross_sq_m_other;

    return *this;
}


BigSerial Building::getFmBuildingId() const
{
	return fm_building_id;
}

BigSerial Building::getFmProjectId() const
{
	return fm_project_id;
}

BigSerial Building::getFmParcelId() const
{
	return fm_parcel_id;
}

int	Building::getStoreysAboveGround() const
{
	return storeys_above_ground;
}

int Building::getStoreysBelowGround() const
{
	return storeys_below_ground;
}

std::string Building::getFromDate() const
{
	return from_date;
}

std::string Building::getToDate() const
{
	return to_date;
}

std::string Building::getBuildingStatus() const
{
	return building_status;
}


float	Building::getGrossSqmRes() const
{
	return gross_sq_m_res;
}

float	Building::getGrossSqmOffice() const
{
	return gross_sq_m_office;
}

float	Building::getGrossSqmRetail() const
{
	return gross_sq_m_retail;
}

float	Building::getGrossSqmOther() const
{
	return gross_sq_m_other;
}


/*
BigSerial Building::getId() const {
    return id;
}

BigSerial Building::getTypeId() const {
    return typeId;
}

BigSerial Building::getParcelId() const {
    return parcelId;
}

BigSerial Building::getTenureId() const {
    return tenureId;
}

int Building::getBuiltYear() const {
    return builtYear;
}

int Building::getStoreys() const {
    return storeys;
}

int Building::getParkingSpaces() const {
    return parkingSpaces;
}

double Building::getLandedArea() const {
    return landedArea;
}
*/

namespace sim_mob {
    namespace long_term {

        std::ostream& operator<<(std::ostream& strm, const Building& data) {
            return strm << "{"
            		/*
                    << "\"id\":\"" << data.id << "\","
                    << "\"typeId\":\"" << data.typeId << "\","
                    << "\"parcelId\":\"" << data.parcelId << "\","
                    << "\"tenureId\":\"" << data.tenureId << "\","
                    << "\"builtYear\":\"" << data.builtYear << "\","
                    << "\"storeys\":\"" << data.storeys << "\","
                    << "\"parkingSpaces\":\"" << data.parkingSpaces << "\","
                    << "\"landArea\":\"" << data.landedArea << "\""
                    */
            		<< "\"fm_building_id \":\"" << data.fm_building_id << "\","
            		<< "\"fm_project_id \":\"" << data.fm_project_id << "\","
            		<< "\"fm_parcel_id \":\"" << data.fm_parcel_id << "\","
            		<< "\"storeys_above_ground \":\"" << data.storeys_above_ground << "\","
            		<< "\"storeys_below_ground \":\"" << data.storeys_below_ground << "\","
            		<< "\"from_date \":\"" << data.from_date << "\","
            		<< "\"to_date \":\"" << data.to_date << "\","
            		<< "\"building_status \":\"" << data.building_status << "\","
            		<< "\"gross_sq_m_res \":\"" << data.gross_sq_m_res << "\","
            		<< "\"gross_sq_m_office \":\"" << data.gross_sq_m_office << "\","
            		<< "\"gross_sq_m_retail \":\"" << data.gross_sq_m_retail << "\","
            		<< "\"gross_sq_m_other \":\"" << data.gross_sq_m_other << "\""
                    << "}";
        }
    }
}
