/*
 * AMW_Logging.cpp
 *
 *  Created on: 26-nov.-2015
 *      Author: Arne
 */

#include "AMW_Logging.h"
#include <AP_Common.h>
#include <AC_Facade.h>
#include <AMW_Corridors.h>
#include <AMW_Planner.h>

#define LOG_MSG_AMW_POSITION 250
#define LOG_MSG_AMW_SYNC 249
#define LOG_MSG_AMW_COMPLETED_RESERVATION 248
#define LOG_MSG_AMW_FAILED_RESERVATION 247
#define LOG_MSG_AMW_CONFLICT 246
#define LOG_MSG_AMW_ASSIGNED_PACKAGE 245
#define LOG_MSG_AMW_COMPLETED_PACKAGE 244
#define LOG_MSG_AMW_FAILED_PACKAGE 243

AMW_Logging* AMW_Logging::instance = 0;

struct PACKED log_AMWPosition {
    LOG_PACKET_HEADER;
    uint32_t timestamp;
    float x;
    float y;
    float alt;
    float resAlt;
    float dev;
    float dist;
    uint8_t bat;
};

struct PACKED log_AMWSync {
    LOG_PACKET_HEADER;
    uint32_t timestamp;
    uint8_t syncId;
};

struct PACKED log_AMWCompletedReservation {
    LOG_PACKET_HEADER;
    uint32_t timestamp;
    float level;
    uint8_t attempts;
};

struct PACKED log_AMWFailedReservation {
    LOG_PACKET_HEADER;
    uint32_t timestamp;
    uint8_t attempts;
};

struct PACKED log_AMWConflict {
    LOG_PACKET_HEADER;
    uint32_t timestamp;
};

struct PACKED log_AMWAssignedPackage {
    LOG_PACKET_HEADER;
    uint32_t timestamp;
    uint8_t packageId;
    float estimate;
};

struct PACKED log_AMWCompletedPackage {
    LOG_PACKET_HEADER;
    uint32_t timestamp;
    uint8_t packageId;
};

struct PACKED log_AMWFailedPackage {
    LOG_PACKET_HEADER;
    uint32_t timestamp;
    uint8_t packageId;
};

static const struct LogStructure amw_log_structures[] PROGMEM = {
    { LOG_MSG_AMW_POSITION, sizeof(log_AMWPosition),
      "ADP", "IffffffB",  "TimeMS,x,y,alt,rAlt,dev,dist,bat" },
    { LOG_MSG_AMW_SYNC, sizeof(log_AMWSync),
      "ADSY", "IB",  "TimeMS,syncId" },
    { LOG_MSG_AMW_COMPLETED_RESERVATION, sizeof(log_AMWCompletedReservation),
      "ADSR", "IfB",  "TimeMS,alt,attempts" },
    { LOG_MSG_AMW_FAILED_RESERVATION, sizeof(log_AMWFailedReservation),
      "ADFR", "IB",  "TimeMS,attempts" },
    { LOG_MSG_AMW_CONFLICT, sizeof(log_AMWConflict),
      "ADC", "I",  "TimeMS" },
    { LOG_MSG_AMW_ASSIGNED_PACKAGE, sizeof(log_AMWAssignedPackage),
      "ADAP", "IBf",  "TimeMS,pId,est" },
    { LOG_MSG_AMW_COMPLETED_PACKAGE, sizeof(log_AMWCompletedPackage),
      "ADCP", "IB",  "TimeMS,pId" },
    { LOG_MSG_AMW_FAILED_PACKAGE, sizeof(log_AMWFailedPackage),
      "ADFP", "IB",  "TimeMS,pId" }

};

AMW_Logging::AMW_Logging() {
    this->loggingStarted = false;
    this->dataFlash = AC_Facade::getFacade()->getDataFlash();
}

AMW_Logging* AMW_Logging::getInstance() {
    if (!AMW_Logging::instance) {
        AMW_Logging::instance = new AMW_Logging();
    }
    return AMW_Logging::instance;
}

void AMW_Logging::writeHeaders(void)
{
    if (!loggingStarted) {
        loggingStarted = true;
        dataFlash->AddLogFormats(amw_log_structures, sizeof(amw_log_structures) / sizeof(LogStructure));
    }
}

void AMW_Logging::logPosition()
{
    if (!dataFlash || !dataFlash->logging_started()) {
      return;
    }

    writeHeaders();

    Vector3f pos = AC_Facade::getFacade()->getRealPosition();
    float resAlt = AMW_Corridor_Manager::getInstance()->getReservedAltitude(0);
    float deviation = AMW_Corridor_Manager::getInstance()->getDeviation();
    uint8_t bat = AC_Facade::getFacade()->getBattery()->capacity_remaining_pct();

    struct log_AMWPosition pkt = {
            LOG_PACKET_HEADER_INIT(LOG_MSG_AMW_POSITION),
            timestamp   : AC_Facade::getFacade()->getTimeMillis(),
            x           : pos.x,
            y           : pos.y,
            alt         : pos.z,
            resAlt      : resAlt,
            dev         : deviation,
            dist        : AMW_Planner::distanceTravelled,
            bat         : bat
    };
    dataFlash->WriteBlock(&pkt, sizeof(pkt));

}

void AMW_Logging::sync(uint8_t id) {
    if (!dataFlash || !dataFlash->logging_started()) {
      return;
    }

    writeHeaders();

    struct log_AMWSync pkt = {
            LOG_PACKET_HEADER_INIT(LOG_MSG_AMW_SYNC),
            timestamp   : AC_Facade::getFacade()->getTimeMillis(),
            syncId      : id
    };
    dataFlash->WriteBlock(&pkt, sizeof(pkt));
}

void AMW_Logging::logCompletedReservation(float level, uint8_t attempts) {
    if (!dataFlash || !dataFlash->logging_started()) {
      return;
    }

    writeHeaders();

    struct log_AMWCompletedReservation pkt = {
            LOG_PACKET_HEADER_INIT(LOG_MSG_AMW_COMPLETED_RESERVATION),
            timestamp   : AC_Facade::getFacade()->getTimeMillis(),
            level       : level,
            attempts    : attempts
    };
    dataFlash->WriteBlock(&pkt, sizeof(pkt));
}

void AMW_Logging::logFailedReservation(uint8_t attempts) {
    if (!dataFlash || !dataFlash->logging_started()) {
      return;
    }

    writeHeaders();

    struct log_AMWFailedReservation pkt = {
            LOG_PACKET_HEADER_INIT(LOG_MSG_AMW_FAILED_RESERVATION),
            timestamp   : AC_Facade::getFacade()->getTimeMillis(),
            attempts    : attempts
    };
    dataFlash->WriteBlock(&pkt, sizeof(pkt));
}

void AMW_Logging::logConflict() {
    if (!dataFlash || !dataFlash->logging_started()) {
      return;
    }

    writeHeaders();

    struct log_AMWConflict pkt = {
            LOG_PACKET_HEADER_INIT(LOG_MSG_AMW_CONFLICT),
            timestamp   : AC_Facade::getFacade()->getTimeMillis()
    };
    dataFlash->WriteBlock(&pkt, sizeof(pkt));
}

void AMW_Logging::logAssignedPackage(uint8_t packageId, float estimate) {
    if (!dataFlash || !dataFlash->logging_started()) {
      return;
    }

    writeHeaders();

    struct log_AMWAssignedPackage pkt = {
            LOG_PACKET_HEADER_INIT(LOG_MSG_AMW_ASSIGNED_PACKAGE),
            timestamp   : AC_Facade::getFacade()->getTimeMillis(),
            packageId   : packageId,
            estimate    : estimate
    };
    dataFlash->WriteBlock(&pkt, sizeof(pkt));
}

void AMW_Logging::logCompletedPackage(uint8_t packageId) {
    if (!dataFlash || !dataFlash->logging_started()) {
      return;
    }

    writeHeaders();

    struct log_AMWCompletedPackage pkt = {
            LOG_PACKET_HEADER_INIT(LOG_MSG_AMW_COMPLETED_PACKAGE),
            timestamp   : AC_Facade::getFacade()->getTimeMillis(),
            packageId   : packageId,
    };
    dataFlash->WriteBlock(&pkt, sizeof(pkt));
}

void AMW_Logging::logFailedPackage(uint8_t packageId) {
    if (!dataFlash || !dataFlash->logging_started()) {
      return;
    }

    writeHeaders();

    struct log_AMWFailedPackage pkt = {
            LOG_PACKET_HEADER_INIT(LOG_MSG_AMW_FAILED_PACKAGE),
            timestamp   : AC_Facade::getFacade()->getTimeMillis(),
            packageId   : packageId,
    };
    dataFlash->WriteBlock(&pkt, sizeof(pkt));
}
