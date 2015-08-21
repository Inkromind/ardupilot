/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include <AC_CommunicationFacade.h>
#include <GCS.h>
#include <AMW_Corridors.h>

#ifndef MAD_MSG_IDS
#define MAD_MSG_IDS
#define MAD_MSG_CORRIDOR_RESERVATION_ID 1
#define MAD_MSG_CORRIDOR_ANNOUNCEMENT_ID 2
#define MAD_MSG_COMPLETED_PACKAGE_ID 3
#define MAD_MSG_FAILED_PACKAGE_ID 4
#endif

void AC_CommunicationFacade::broadcastReservationRequest(uint8_t reservationId, AMW_List<AMW_Corridor*>* corridors) {
    AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
    while (iterator->hasNext()) {
        AMW_Corridor* corridor = iterator->next();
        for (uint8_t i=0; i<num_gcs; i++) {
            if (gcs[i].initialised) {
                gcs[i].send_MADmsg(MAD_MSG_CORRIDOR_RESERVATION_ID, reservationId, corridor);
            }
        }
    }
    delete iterator;
}

void AC_CommunicationFacade::broadcastCorridors(AMW_List<AMW_Corridor*>* corridors) {
    AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
    while (iterator->hasNext()) {
        AMW_Corridor* corridor = iterator->next();
        for (uint8_t i=0; i<num_gcs; i++) {
            if (gcs[i].initialised) {
                gcs[i].send_MADmsg(MAD_MSG_CORRIDOR_ANNOUNCEMENT_ID, corridor);
            }
        }
    }
    delete iterator;
}

void AC_CommunicationFacade::completedPackage(uint8_t packageId) {
    for (uint8_t i=0; i<num_gcs; i++) {
        if (gcs[i].initialised) {
            gcs[i].send_MADmsg(MAD_MSG_COMPLETED_PACKAGE_ID, packageId);
        }
    }
}

void AC_CommunicationFacade::failedPackage(uint8_t packageId) {
    for (uint8_t i=0; i<num_gcs; i++) {
        if (gcs[i].initialised) {
            gcs[i].send_MADmsg(MAD_MSG_FAILED_PACKAGE_ID, packageId);
        }
    }
}

void GCS_MAVLINK::send_MADmsg(uint8_t id, ...) {
    va_list args;
    switch (id) {
    case MAD_MSG_COMPLETED_PACKAGE_ID:
    {
        va_start(args, id);
        uint8_t packageId = va_arg(args, int);
        va_end(args);
        mavlink_msg_mad_completed_package_send(chan, packageId);
        break;
    }
    case MAD_MSG_FAILED_PACKAGE_ID:
    {
        va_start(args, id);
        uint8_t packageId = va_arg(args, int);
        va_end(args);
        mavlink_msg_mad_failed_package_send(chan, packageId);
        break;
    }
    case MAD_MSG_CORRIDOR_RESERVATION_ID:
    {
        float p1x, p1y, p2x, p2y;
        va_start(args, id);
        uint8_t reservationId = va_arg(args, int);
        AMW_Corridor* corridor = va_arg(args, AMW_Corridor*);
        va_end(args);
        switch (corridor->getType()) {
        case AMW_Corridor::HORIZONTAL: {
            Vector3f startpoint = corridor->getStartPoint(true);
            Vector3f endpoint = corridor->getEndPoint();
            p1x = startpoint.x;
            p1y = startpoint.y;
            p2x = endpoint.x;
            p2y = endpoint.y;
            break;
        }
        case AMW_Corridor::VERTICAL:
        case AMW_Corridor::POSITION: {
            Vector3f startpoint = corridor->getStartPoint(true);
            p1x = startpoint.x;
            p1y = startpoint.y;
            p2x = 0;
            p2y = 0;
            break;
        }
        default:
            break;
        }
        mavlink_msg_mad_request_corridor_reservation_send(chan, 0, reservationId, corridor->getId(), (uint8_t)corridor->getType(), corridor->getAltitude(), p1x, p1y, p2x, p2y);
        break;
    }
    case MAD_MSG_CORRIDOR_ANNOUNCEMENT_ID:
    {
        va_start(args, id);
        AMW_Corridor* corridor = va_arg(args, AMW_Corridor*);
        va_end(args);
        float p1x, p1y, p2x, p2y;
        switch (corridor->getType()) {
        case AMW_Corridor::HORIZONTAL: {
            Vector3f startpoint = corridor->getStartPoint(true);
            Vector3f endpoint = corridor->getEndPoint();
            p1x = startpoint.x;
            p1y = startpoint.y;
            p2x = endpoint.x;
            p2y = endpoint.y;
            break;
        }
        case AMW_Corridor::VERTICAL:
        case AMW_Corridor::POSITION: {
            Vector3f startpoint = corridor->getStartPoint();
            p1x = startpoint.x;
            p1y = startpoint.y;
            p2x = 0;
            p2y = 0;
            break;
        }
        default:
            break;
        }
        mavlink_msg_mad_corridor_announcement_send(chan, 0, corridor->getId(), (uint8_t)corridor->getType(), corridor->getAltitude(), p1x, p1y, p2x, p2y);
        break;
    }
    }
}

void AC_CommunicationFacade::sendDebug(const prog_char_t *str) {
    gcs_send_text_P(SEVERITY_LOW, str);
}

void AC_CommunicationFacade::sendFormattedDebug(const prog_char_t *str, ...) {
    va_list args;
    va_start(args, str);
    gcs_send_text_fmt2(str, args);
    va_end(args);
}
