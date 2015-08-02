// MESSAGE MAD_CORRIDOR_ANNOUNCEMENT PACKING

#define MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT 204

typedef struct __mavlink_mad_corridor_announcement_t
{
 float alt; ///< Altitude
 float p1x; ///< Point 1: X
 float p1y; ///< Point 1: Y
 float p2x; ///< Point 2: X
 float p2y; ///< Point 2: Y
 uint8_t drone_id; ///< Drone id
 uint8_t corridor_id; ///< Corridor ID
 uint8_t corridor_type; ///< Corridor Type
} mavlink_mad_corridor_announcement_t;

#define MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN 23
#define MAVLINK_MSG_ID_204_LEN 23

#define MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_CRC 83
#define MAVLINK_MSG_ID_204_CRC 83



#define MAVLINK_MESSAGE_INFO_MAD_CORRIDOR_ANNOUNCEMENT { \
	"MAD_CORRIDOR_ANNOUNCEMENT", \
	8, \
	{  { "alt", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_mad_corridor_announcement_t, alt) }, \
         { "p1x", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_mad_corridor_announcement_t, p1x) }, \
         { "p1y", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_mad_corridor_announcement_t, p1y) }, \
         { "p2x", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_mad_corridor_announcement_t, p2x) }, \
         { "p2y", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_mad_corridor_announcement_t, p2y) }, \
         { "drone_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_mad_corridor_announcement_t, drone_id) }, \
         { "corridor_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 21, offsetof(mavlink_mad_corridor_announcement_t, corridor_id) }, \
         { "corridor_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 22, offsetof(mavlink_mad_corridor_announcement_t, corridor_type) }, \
         } \
}


/**
 * @brief Pack a mad_corridor_announcement message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param drone_id Drone id
 * @param corridor_id Corridor ID
 * @param corridor_type Corridor Type
 * @param alt Altitude
 * @param p1x Point 1: X
 * @param p1y Point 1: Y
 * @param p2x Point 2: X
 * @param p2y Point 2: Y
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_corridor_announcement_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint8_t drone_id, uint8_t corridor_id, uint8_t corridor_type, float alt, float p1x, float p1y, float p2x, float p2y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN];
	_mav_put_float(buf, 0, alt);
	_mav_put_float(buf, 4, p1x);
	_mav_put_float(buf, 8, p1y);
	_mav_put_float(buf, 12, p2x);
	_mav_put_float(buf, 16, p2y);
	_mav_put_uint8_t(buf, 20, drone_id);
	_mav_put_uint8_t(buf, 21, corridor_id);
	_mav_put_uint8_t(buf, 22, corridor_type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN);
#else
	mavlink_mad_corridor_announcement_t packet;
	packet.alt = alt;
	packet.p1x = p1x;
	packet.p1y = p1y;
	packet.p2x = p2x;
	packet.p2y = p2y;
	packet.drone_id = drone_id;
	packet.corridor_id = corridor_id;
	packet.corridor_type = corridor_type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN);
#endif
}

/**
 * @brief Pack a mad_corridor_announcement message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param drone_id Drone id
 * @param corridor_id Corridor ID
 * @param corridor_type Corridor Type
 * @param alt Altitude
 * @param p1x Point 1: X
 * @param p1y Point 1: Y
 * @param p2x Point 2: X
 * @param p2y Point 2: Y
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_corridor_announcement_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint8_t drone_id,uint8_t corridor_id,uint8_t corridor_type,float alt,float p1x,float p1y,float p2x,float p2y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN];
	_mav_put_float(buf, 0, alt);
	_mav_put_float(buf, 4, p1x);
	_mav_put_float(buf, 8, p1y);
	_mav_put_float(buf, 12, p2x);
	_mav_put_float(buf, 16, p2y);
	_mav_put_uint8_t(buf, 20, drone_id);
	_mav_put_uint8_t(buf, 21, corridor_id);
	_mav_put_uint8_t(buf, 22, corridor_type);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN);
#else
	mavlink_mad_corridor_announcement_t packet;
	packet.alt = alt;
	packet.p1x = p1x;
	packet.p1y = p1y;
	packet.p2x = p2x;
	packet.p2y = p2y;
	packet.drone_id = drone_id;
	packet.corridor_id = corridor_id;
	packet.corridor_type = corridor_type;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN);
#endif
}

/**
 * @brief Encode a mad_corridor_announcement struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mad_corridor_announcement C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_corridor_announcement_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mad_corridor_announcement_t* mad_corridor_announcement)
{
	return mavlink_msg_mad_corridor_announcement_pack(system_id, component_id, msg, mad_corridor_announcement->drone_id, mad_corridor_announcement->corridor_id, mad_corridor_announcement->corridor_type, mad_corridor_announcement->alt, mad_corridor_announcement->p1x, mad_corridor_announcement->p1y, mad_corridor_announcement->p2x, mad_corridor_announcement->p2y);
}

/**
 * @brief Encode a mad_corridor_announcement struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mad_corridor_announcement C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_corridor_announcement_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mad_corridor_announcement_t* mad_corridor_announcement)
{
	return mavlink_msg_mad_corridor_announcement_pack_chan(system_id, component_id, chan, msg, mad_corridor_announcement->drone_id, mad_corridor_announcement->corridor_id, mad_corridor_announcement->corridor_type, mad_corridor_announcement->alt, mad_corridor_announcement->p1x, mad_corridor_announcement->p1y, mad_corridor_announcement->p2x, mad_corridor_announcement->p2y);
}

/**
 * @brief Send a mad_corridor_announcement message
 * @param chan MAVLink channel to send the message
 *
 * @param drone_id Drone id
 * @param corridor_id Corridor ID
 * @param corridor_type Corridor Type
 * @param alt Altitude
 * @param p1x Point 1: X
 * @param p1y Point 1: Y
 * @param p2x Point 2: X
 * @param p2y Point 2: Y
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mad_corridor_announcement_send(mavlink_channel_t chan, uint8_t drone_id, uint8_t corridor_id, uint8_t corridor_type, float alt, float p1x, float p1y, float p2x, float p2y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN];
	_mav_put_float(buf, 0, alt);
	_mav_put_float(buf, 4, p1x);
	_mav_put_float(buf, 8, p1y);
	_mav_put_float(buf, 12, p2x);
	_mav_put_float(buf, 16, p2y);
	_mav_put_uint8_t(buf, 20, drone_id);
	_mav_put_uint8_t(buf, 21, corridor_id);
	_mav_put_uint8_t(buf, 22, corridor_type);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT, buf, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT, buf, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN);
#endif
#else
	mavlink_mad_corridor_announcement_t packet;
	packet.alt = alt;
	packet.p1x = p1x;
	packet.p1y = p1y;
	packet.p2x = p2x;
	packet.p2y = p2y;
	packet.drone_id = drone_id;
	packet.corridor_id = corridor_id;
	packet.corridor_type = corridor_type;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT, (const char *)&packet, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT, (const char *)&packet, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN);
#endif
#endif
}

#if MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mad_corridor_announcement_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t drone_id, uint8_t corridor_id, uint8_t corridor_type, float alt, float p1x, float p1y, float p2x, float p2y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_float(buf, 0, alt);
	_mav_put_float(buf, 4, p1x);
	_mav_put_float(buf, 8, p1y);
	_mav_put_float(buf, 12, p2x);
	_mav_put_float(buf, 16, p2y);
	_mav_put_uint8_t(buf, 20, drone_id);
	_mav_put_uint8_t(buf, 21, corridor_id);
	_mav_put_uint8_t(buf, 22, corridor_type);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT, buf, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT, buf, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN);
#endif
#else
	mavlink_mad_corridor_announcement_t *packet = (mavlink_mad_corridor_announcement_t *)msgbuf;
	packet->alt = alt;
	packet->p1x = p1x;
	packet->p1y = p1y;
	packet->p2x = p2x;
	packet->p2y = p2y;
	packet->drone_id = drone_id;
	packet->corridor_id = corridor_id;
	packet->corridor_type = corridor_type;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT, (const char *)packet, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT, (const char *)packet, MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN);
#endif
#endif
}
#endif

#endif

// MESSAGE MAD_CORRIDOR_ANNOUNCEMENT UNPACKING


/**
 * @brief Get field drone_id from mad_corridor_announcement message
 *
 * @return Drone id
 */
static inline uint8_t mavlink_msg_mad_corridor_announcement_get_drone_id(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  20);
}

/**
 * @brief Get field corridor_id from mad_corridor_announcement message
 *
 * @return Corridor ID
 */
static inline uint8_t mavlink_msg_mad_corridor_announcement_get_corridor_id(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  21);
}

/**
 * @brief Get field corridor_type from mad_corridor_announcement message
 *
 * @return Corridor Type
 */
static inline uint8_t mavlink_msg_mad_corridor_announcement_get_corridor_type(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  22);
}

/**
 * @brief Get field alt from mad_corridor_announcement message
 *
 * @return Altitude
 */
static inline float mavlink_msg_mad_corridor_announcement_get_alt(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field p1x from mad_corridor_announcement message
 *
 * @return Point 1: X
 */
static inline float mavlink_msg_mad_corridor_announcement_get_p1x(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field p1y from mad_corridor_announcement message
 *
 * @return Point 1: Y
 */
static inline float mavlink_msg_mad_corridor_announcement_get_p1y(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field p2x from mad_corridor_announcement message
 *
 * @return Point 2: X
 */
static inline float mavlink_msg_mad_corridor_announcement_get_p2x(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field p2y from mad_corridor_announcement message
 *
 * @return Point 2: Y
 */
static inline float mavlink_msg_mad_corridor_announcement_get_p2y(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Decode a mad_corridor_announcement message into a struct
 *
 * @param msg The message to decode
 * @param mad_corridor_announcement C-struct to decode the message contents into
 */
static inline void mavlink_msg_mad_corridor_announcement_decode(const mavlink_message_t* msg, mavlink_mad_corridor_announcement_t* mad_corridor_announcement)
{
#if MAVLINK_NEED_BYTE_SWAP
	mad_corridor_announcement->alt = mavlink_msg_mad_corridor_announcement_get_alt(msg);
	mad_corridor_announcement->p1x = mavlink_msg_mad_corridor_announcement_get_p1x(msg);
	mad_corridor_announcement->p1y = mavlink_msg_mad_corridor_announcement_get_p1y(msg);
	mad_corridor_announcement->p2x = mavlink_msg_mad_corridor_announcement_get_p2x(msg);
	mad_corridor_announcement->p2y = mavlink_msg_mad_corridor_announcement_get_p2y(msg);
	mad_corridor_announcement->drone_id = mavlink_msg_mad_corridor_announcement_get_drone_id(msg);
	mad_corridor_announcement->corridor_id = mavlink_msg_mad_corridor_announcement_get_corridor_id(msg);
	mad_corridor_announcement->corridor_type = mavlink_msg_mad_corridor_announcement_get_corridor_type(msg);
#else
	memcpy(mad_corridor_announcement, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_MAD_CORRIDOR_ANNOUNCEMENT_LEN);
#endif
}
