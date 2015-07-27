/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include <AC_CommunicationFacade.h>
#include <GCS.h>

void AC_CommunicationFacade::broadcastReservationRequest(uint8_t reservationId, AMW_List<AMW_Corridor*>* corridors) {
    AMW_List<AMW_Corridor*>::Iterator* iterator = corridors->iterator();
    while (iterator->hasNext()) {
        AMW_Corridor* corridor = iterator->next();
        for (uint8_t i=0; i<num_gcs; i++) {
            if (gcs[i].initialised) {
                gcs[i].broadcastReservationRequest(reservationId, corridor);
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
                gcs[i].broadcastCorridor(corridor);
            }
        }
    }
    delete iterator;
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

void GCS_MAVLINK::broadcastReservationRequest(uint8_t reservationId, AMW_Corridor* corridor) {
    float p1x, p1y, p2x, p2y;
    switch (corridor->getType()) {
    case AMW_Corridor::Type::HORIZONTAL: {
        Vector3f startpoint = corridor->getStartPoint(true);
        Vector3f endpoint = corridor->getEndPoint(true);
        p1x = startpoint.x;
        p1y = startpoint.y;
        p2x = endpoint.x;
        p2y = endpoint.y;
        break;
    }
    case AMW_Corridor::Type::VERTICAL:
    case AMW_Corridor::Type::POSITION: {
        Vector3f startpoint = corridor->getStartPoint(true);
        p1x = startpoint.x;
        p1y = startpoint.y;
        p2x = 0;
        p2y = 0;
        break;
    }
    }
    mavlink_msg_mad_request_corridor_reservation_send(chan, 0, reservationId, corridor->getId(), (uint8_t)corridor->getType(), corridor->getAltitude(), p1x, p1y, p2x, p2y);
}

void GCS_MAVLINK::broadcastCorridor(AMW_Corridor* corridor) {
    float p1x, p1y, p2x, p2y;
    switch (corridor->getType()) {
    case AMW_Corridor::Type::HORIZONTAL: {
        Vector3f startpoint = corridor->getStartPoint(true);
        Vector3f endpoint = corridor->getEndPoint(true);
        p1x = startpoint.x;
        p1y = startpoint.y;
        p2x = endpoint.x;
        p2y = endpoint.y;
        break;
    }
    case AMW_Corridor::Type::VERTICAL:
    case AMW_Corridor::Type::POSITION: {
        Vector3f startpoint = corridor->getStartPoint(true);
        p1x = startpoint.x;
        p1y = startpoint.y;
        p2x = 0;
        p2y = 0;
        break;
    }
    }
    mavlink_msg_mad_corridor_announcement_send(chan, 0, corridor->getId(), (uint8_t)corridor->getType(), corridor->getAltitude(), p1x, p1y, p2x, p2y);
}
