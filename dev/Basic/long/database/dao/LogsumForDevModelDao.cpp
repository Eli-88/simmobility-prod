/*
 * LogsumForDevModelDao.cpp
 *
 *  Created on: May 13, 2015
 *      Author: gishara
 */

#include "LogsumForDevModelDao.hpp"
#include "DatabaseHelper.hpp"

using namespace sim_mob::db;
using namespace sim_mob::long_term;

LogsumForDevModelDao::LogsumForDevModelDao(DB_Connection& connection): SqlAbstractDao<LogsumForDevModel>(connection, DB_TABLE_LOGSUM_FOR_DEVMODEL,EMPTY_STR, EMPTY_STR, EMPTY_STR,DB_GETALL_DEV_LOGSUMS, EMPTY_STR)
{}

LogsumForDevModelDao::~LogsumForDevModelDao() {}

void LogsumForDevModelDao::fromRow(Row& result, LogsumForDevModel& outObj)
{
	outObj.taz2012Id = result.get<BigSerial>("taz_id_2012", INVALID_ID);
    outObj.taz2008Id = result.get<BigSerial>("taz_id_2008", INVALID_ID);
    outObj.accessibility = result.get<double>("accessibility", 0);
}

void LogsumForDevModelDao::toRow(LogsumForDevModel& data, Parameters& outParams, bool update) {}


