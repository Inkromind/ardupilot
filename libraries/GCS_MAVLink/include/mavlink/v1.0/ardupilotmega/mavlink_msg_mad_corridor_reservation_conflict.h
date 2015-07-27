// MESSAGE MAD_CORRIDOR_RESERVATION_CONFLICT PACKING

#define MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT 201

typedef struct __mavlink_mad_corridor_reservation_conflict_t
{
 uint8_t drone_id; ///< Requesting drone id
 uint8_t conflicting_drone; ///< Conflicting drone id
 uint8_t reservation_id; ///< Reservation ID
 uint8_t preliminary_id; ///< Preliminary Corridor ID
 uint8_t preliminary_type; ///< Preliminary Corridor Type
 uint8_t preliminary_alt; ///< Preliminary Corridor Altitude
 uint8_t conflicting_id; ///< Conflicting Corridor ID
 uint8_t conflicting_type; ///< Conflicting Corridor Type
 uint8_t conflicting_alt; ///< Conflicting Corridor Altitude
} mavlink_mad_corridor_reservation_conflict_t;

#define MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN 9
#define MAVLINK_MSG_ID_201_LEN 9

#define MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_CRC 17
#define MAVLINK_MSG_ID_201_CRC 17



#define MAVLINK_MESSAGE_INFO_MAD_CORRIDOR_RESERVATION_CONFLICT { \
	"MAD_CORRIDOR_RESERVATION_CONFLICT", \
	9, \
	{  { "drone_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_mad_corridor_reservation_conflict_t, drone_id) }, \
         { "conflicting_drone", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_mad_corridor_reservation_conflict_t, conflicting_drone) }, \
         { "reservation_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_mad_corridor_reservation_conflict_t, reservation_id) }, \
         { "preliminary_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 3, offsetof(mavlink_mad_corridor_reservation_conflict_t, preliminary_id) }, \
         { "preliminary_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_mad_corridor_reservation_conflict_t, preliminary_type) }, \
         { "preliminary_alt", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_mad_corridor_reservation_conflict_t, preliminary_alt) }, \
         { "conflicting_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_mad_corridor_reservation_conflict_t, conflicting_id) }, \
         { "conflicting_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 7, offsetof(mavlink_mad_corridor_reservation_conflict_t, conflicting_type) }, \
         { "conflicting_alt", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_mad_corridor_reservation_conflict_t, conflicting_alt) }, \
         } \
}


/**
 * @brief Pack a mad_corridor_reservation_conflict message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param drone_id Requesting drone id
 * @param conflicting_drone Conflicting drone id
 * @param reservation_id Reservation ID
 * @param preliminary_id Preliminary Corridor ID
 * @param preliminary_type Preliminary Corridor Type
 * @param preliminary_alt Preliminary Corridor Altitude
 * @param conflicting_id Conflicting Corridor ID
 * @param conflicting_type Conflicting Corridor Type
 * @param conflicting_alt Conflicting Corridor Altitude
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_corridor_reservation_conflict_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint8_t drone_id, uint8_t conflicting_drone, uint8_t reservation_id, uint8_t preliminary_id, uint8_t preliminary_type, uint8_t preliminary_alt, uint8_t conflicting_id, uint8_t conflicting_type, uint8_t conflicting_alt)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN];
	_mav_put_uint8_t(buf, 0, drone_id);
	_mav_put_uint8_t(buf, 1, conflicting_drone);
	_mav_put_uint8_t(buf, 2, reservation_id);
	_mav_put_uint8_t(buf, 3, preliminary_id);
	_mav_put_uint8_t(buf, 4, preliminary_type);
	_mav_put_uint8_t(buf, 5, preliminary_alt);
	_mav_put_uint8_t(buf, 6, conflicting_id);
	_mav_put_uint8_t(buf, 7, conflicting_type);
	_mav_put_uint8_t(buf, 8, conflicting_alt);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN);
#else
	mavlink_mad_corridor_reservation_conflict_t packet;
	packet.drone_id = drone_id;
	packet.conflicting_drone = conflicting_drone;
	packet.reservation_id = reservation_id;
	packet.preliminary_id = preliminary_id;
	packet.preliminary_type = preliminary_type;
	packet.preliminary_alt = preliminary_alt;
	packet.conflicting_id = conflicting_id;
	packet.conflicting_type = conflicting_type;
	packet.conflicting_alt = conflicting_alt;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN);
#endif
}

/**
 * @brief Pack a mad_corridor_reservation_conflict message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param drone_id Requesting drone id
 * @param conflicting_drone Conflicting drone id
 * @param reservation_id Reservation ID
 * @param preliminary_id Preliminary Corridor ID
 * @param preliminary_type Preliminary Corridor Type
 * @param preliminary_alt Preliminary Corridor Altitude
 * @param conflicting_id Conflicting Corridor ID
 * @param conflicting_type Conflicting Corridor Type
 * @param conflicting_alt Conflicting Corridor Altitude
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_corridor_reservation_conflict_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint8_t drone_id,uint8_t conflicting_drone,uint8_t reservation_id,uint8_t preliminary_id,uint8_t preliminary_type,uint8_t preliminary_alt,uint8_t conflicting_id,uint8_t conflicting_type,uint8_t conflicting_alt)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN];
	_mav_put_uint8_t(buf, 0, drone_id);
	_mav_put_uint8_t(buf, 1, conflicting_drone);
	_mav_put_uint8_t(buf, 2, reservation_id);
	_mav_put_uint8_t(buf, 3, preliminary_id);
	_mav_put_uint8_t(buf, 4, preliminary_type);
	_mav_put_uint8_t(buf, 5, preliminary_alt);
	_mav_put_uint8_t(buf, 6, conflicting_id);
	_mav_put_uint8_t(buf, 7, conflicting_type);
	_mav_put_uint8_t(buf, 8, conflicting_alt);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN);
#else
	mavlink_mad_corridor_reservation_conflict_t packet;
	packet.drone_id = drone_id;
	packet.conflicting_drone = conflicting_drone;
	packet.reservation_id = reservation_id;
	packet.preliminary_id = preliminary_id;
	packet.preliminary_type = preliminary_type;
	packet.preliminary_alt = preliminary_alt;
	packet.conflicting_id = conflicting_id;
	packet.conflicting_type = conflicting_type;
	packet.conflicting_alt = conflicting_alt;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN);
#endif
}

/**
 * @brief Encode a mad_corridor_reservation_conflict struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mad_corridor_reservation_conflict C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_corridor_reservation_conflict_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mad_corridor_reservation_conflict_t* mad_corridor_reservation_conflict)
{
	return mavlink_msg_mad_corridor_reservation_conflict_pack(system_id, component_id, msg, mad_corridor_reservation_conflict->drone_id, mad_corridor_reservation_conflict->conflicting_drone, mad_corridor_reservation_conflict->reservation_id, mad_corridor_reservation_conflict->preliminary_id, mad_corridor_reservation_conflict->preliminary_type, mad_corridor_reservation_conflict->preliminary_alt, mad_corridor_reservation_conflict->conflicting_id, mad_corridor_reservation_conflict->conflicting_type, mad_corridor_reservation_conflict->conflicting_alt);
}

/**
 * @brief Encode a mad_corridor_reservation_conflict struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mad_corridor_reservation_conflict C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_corridor_reservation_conflict_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mad_corridor_reservation_conflict_t* mad_corridor_reservation_conflict)
{
	return mavlink_msg_mad_corridor_reservation_conflict_pack_chan(system_id, component_id, chan, msg, mad_corridor_reservation_conflict->drone_id, mad_corridor_reservation_conflict->conflicting_drone, mad_corridor_reservation_conflict->reservation_id, mad_corridor_reservation_conflict->preliminary_id, mad_corridor_reservation_conflict->preliminary_type, mad_corridor_reservation_conflict->preliminary_alt, mad_corridor_reservation_conflict->conflicting_id, mad_corridor_reservation_conflict->conflicting_type, mad_corridor_reservation_conflict->conflicting_alt);
}

/**
 * @brief Send a mad_corridor_reservation_conflict message
 * @param chan MAVLink channel to send the message
 *
 * @param drone_id Requesting drone id
 * @param conflicting_drone Conflicting drone id
 * @param reservation_id Reservation ID
 * @param preliminary_id Preliminary Corridor ID
 * @param preliminary_type Preliminary Corridor Type
 * @param preliminary_alt Preliminary Corridor Altitude
 * @param conflicting_id Conflicting Corridor ID
 * @param conflicting_type Conflicting Corridor Type
 * @param conflicting_alt Conflicting Corridor Altitude
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mad_corridor_reservation_conflict_send(mavlink_channel_t chan, uint8_t drone_id, uint8_t conflicting_drone, uint8_t reservation_id, uint8_t preliminary_id, uint8_t preliminary_type, uint8_t preliminary_alt, uint8_t conflicting_id, uint8_t conflicting_type, uint8_t conflicting_alt)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN];
	_mav_put_uint8_t(buf, 0, drone_id);
	_mav_put_uint8_t(buf, 1, conflicting_drone);
	_mav_put_uint8_t(buf, 2, reservation_id);
	_mav_put_uint8_t(buf, 3, preliminary_id);
	_mav_put_uint8_t(buf, 4, preliminary_type);
	_mav_put_uint8_t(buf, 5, preliminary_alt);
	_mav_put_uint8_t(buf, 6, conflicting_id);
	_mav_put_uint8_t(buf, 7, conflicting_type);
	_mav_put_uint8_t(buf, 8, conflicting_alt);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT, buf, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT, buf, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN);
#endif
#else
	mavlink_mad_corridor_reservation_conflict_t packet;
	packet.drone_id = drone_id;
	packet.conflicting_drone = conflicting_drone;
	packet.reservation_id = reservation_id;
	packet.preliminary_id = preliminary_id;
	packet.preliminary_type = preliminary_type;
	packet.preliminary_alt = preliminary_alt;
	packet.conflicting_id = conflicting_id;
	packet.conflicting_type = conflicting_type;
	packet.conflicting_alt = conflicting_alt;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT, (const char *)&packet, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT, (const char *)&packet, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN);
#endif
#endif
}

#if MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mad_corridor_reservation_conflict_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t drone_id, uint8_t conflicting_drone, uint8_t reservation_id, uint8_t preliminary_id, uint8_t preliminary_type, uint8_t preliminary_alt, uint8_t conflicting_id, uint8_t conflicting_type, uint8_t conflicting_alt)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_uint8_t(buf, 0, drone_id);
	_mav_put_uint8_t(buf, 1, conflicting_drone);
	_mav_put_uint8_t(buf, 2, reservation_id);
	_mav_put_uint8_t(buf, 3, preliminary_id);
	_mav_put_uint8_t(buf, 4, preliminary_type);
	_mav_put_uint8_t(buf, 5, preliminary_alt);
	_mav_put_uint8_t(buf, 6, conflicting_id);
	_mav_put_uint8_t(buf, 7, conflicting_type);
	_mav_put_uint8_t(buf, 8, conflicting_alt);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT, buf, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT, buf, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN);
#endif
#else
	mavlink_mad_corridor_reservation_conflict_t *packet = (mavlink_mad_corridor_reservation_conflict_t *)msgbuf;
	packet->drone_id = drone_id;
	packet->conflicting_drone = conflicting_drone;
	packet->reservation_id = reservation_id;
	packet->preliminary_id = preliminary_id;
	packet->preliminary_type = preliminary_type;
	packet->preliminary_alt = preliminary_alt;
	packet->conflicting_id = conflicting_id;
	packet->conflicting_type = conflicting_type;
	packet->conflicting_alt = conflicting_alt;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT, (const char *)packet, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT, (const char *)packet, MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN);
#endif
#endif
}
#endif

#endif

// MESSAGE MAD_CORRIDOR_RESERVATION_CONFLICT UNPACKING


/**
 * @brief Get field drone_id from mad_corridor_reservation_conflict message
 *
 * @return Requesting drone id
 */
static inline uint8_t mavlink_msg_mad_corridor_reservation_conflict_get_drone_id(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field conflicting_drone from mad_corridor_reservation_conflict message
 *
 * @return Conflicting drone id
 */
static inline uint8_t mavlink_msg_mad_corridor_reservation_conflict_get_conflicting_drone(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Get field reservation_id from mad_corridor_reservation_conflict message
 *
 * @return Reservation ID
 */
static inline uint8_t mavlink_msg_mad_corridor_reservation_conflict_get_reservation_id(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  2);
}

/**
 * @brief Get field preliminary_id from mad_corridor_reservation_conflict message
 *
 * @return Preliminary Corridor ID
 */
static inline uint8_t mavlink_msg_mad_corridor_reservation_conflict_get_preliminary_id(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  3);
}

/**
 * @brief Get field preliminary_type from mad_corridor_reservation_conflict message
 *
 * @return Preliminary Corridor Type
 */
static inline uint8_t mavlink_msg_mad_corridor_reservation_conflict_get_preliminary_type(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field preliminary_alt from mad_corridor_reservation_conflict message
 *
 * @return Preliminary Corridor Altitude
 */
static inline uint8_t mavlink_msg_mad_corridor_reservation_conflict_get_preliminary_alt(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  5);
}

/**
 * @brief Get field conflicting_id from mad_corridor_reservation_conflict message
 *
 * @return Conflicting Corridor ID
 */
static inline uint8_t mavlink_msg_mad_corridor_reservation_conflict_get_conflicting_id(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  6);
}

/**
 * @brief Get field conflicting_type from mad_corridor_reservation_conflict message
 *
 * @return Conflicting Corridor Type
 */
static inline uint8_t mavlink_msg_mad_corridor_reservation_conflict_get_conflicting_type(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  7);
}

/**
 * @brief Get field conflicting_alt from mad_corridor_reservation_conflict message
 *
 * @return Conflicting Corridor Altitude
 */
static inline uint8_t mavlink_msg_mad_corridor_reservation_conflict_get_conflicting_alt(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Decode a mad_corridor_reservation_conflict message into a struct
 *
 * @param msg The message to decode
 * @param mad_corridor_reservation_conflict C-struct to decode the message contents into
 */
static inline void mavlink_msg_mad_corridor_reservation_conflict_decode(const mavlink_message_t* msg, mavlink_mad_corridor_reservation_conflict_t* mad_corridor_reservation_conflict)
{
#if MAVLINK_NEED_BYTE_SWAP
	mad_corridor_reservation_conflict->drone_id = mavlink_msg_mad_corridor_reservation_conflict_get_drone_id(msg);
	mad_corridor_reservation_conflict->conflicting_drone = mavlink_msg_mad_corridor_reservation_conflict_get_conflicting_drone(msg);
	mad_corridor_reservation_conflict->reservation_id = mavlink_msg_mad_corridor_reservation_conflict_get_reservation_id(msg);
	mad_corridor_reservation_conflict->preliminary_id = mavlink_msg_mad_corridor_reservation_conflict_get_preliminary_id(msg);
	mad_corridor_reservation_conflict->preliminary_type = mavlink_msg_mad_corridor_reservation_conflict_get_preliminary_type(msg);
	mad_corridor_reservation_conflict->preliminary_alt = mavlink_msg_mad_corridor_reservation_conflict_get_preliminary_alt(msg);
	mad_corridor_reservation_conflict->conflicting_id = mavlink_msg_mad_corridor_reservation_conflict_get_conflicting_id(msg);
	mad_corridor_reservation_conflict->conflicting_type = mavlink_msg_mad_corridor_reservation_conflict_get_conflicting_type(msg);
	mad_corridor_reservation_conflict->conflicting_alt = mavlink_msg_mad_corridor_reservation_conflict_get_conflicting_alt(msg);
#else
	memcpy(mad_corridor_reservation_conflict, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_MAD_CORRIDOR_RESERVATION_CONFLICT_LEN);
#endif
}
