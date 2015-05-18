// MESSAGE MAD_CURRENT_LOCATION PACKING

#define MAVLINK_MSG_ID_MAD_CURRENT_LOCATION 195

typedef struct __mavlink_mad_current_location_t
{
 float x; ///< X Coordinate of current location
 float y; ///< Y Coordinate of current location
 float alt; ///< Current altitude
} mavlink_mad_current_location_t;

#define MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN 12
#define MAVLINK_MSG_ID_195_LEN 12

#define MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_CRC 41
#define MAVLINK_MSG_ID_195_CRC 41



#define MAVLINK_MESSAGE_INFO_MAD_CURRENT_LOCATION { \
	"MAD_CURRENT_LOCATION", \
	3, \
	{  { "x", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_mad_current_location_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_mad_current_location_t, y) }, \
         { "alt", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_mad_current_location_t, alt) }, \
         } \
}


/**
 * @brief Pack a mad_current_location message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param x X Coordinate of current location
 * @param y Y Coordinate of current location
 * @param alt Current altitude
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_current_location_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       float x, float y, float alt)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN];
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);
	_mav_put_float(buf, 8, alt);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN);
#else
	mavlink_mad_current_location_t packet;
	packet.x = x;
	packet.y = y;
	packet.alt = alt;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_CURRENT_LOCATION;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN);
#endif
}

/**
 * @brief Pack a mad_current_location message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param x X Coordinate of current location
 * @param y Y Coordinate of current location
 * @param alt Current altitude
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_current_location_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           float x,float y,float alt)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN];
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);
	_mav_put_float(buf, 8, alt);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN);
#else
	mavlink_mad_current_location_t packet;
	packet.x = x;
	packet.y = y;
	packet.alt = alt;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_CURRENT_LOCATION;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN);
#endif
}

/**
 * @brief Encode a mad_current_location struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mad_current_location C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_current_location_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mad_current_location_t* mad_current_location)
{
	return mavlink_msg_mad_current_location_pack(system_id, component_id, msg, mad_current_location->x, mad_current_location->y, mad_current_location->alt);
}

/**
 * @brief Encode a mad_current_location struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mad_current_location C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_current_location_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mad_current_location_t* mad_current_location)
{
	return mavlink_msg_mad_current_location_pack_chan(system_id, component_id, chan, msg, mad_current_location->x, mad_current_location->y, mad_current_location->alt);
}

/**
 * @brief Send a mad_current_location message
 * @param chan MAVLink channel to send the message
 *
 * @param x X Coordinate of current location
 * @param y Y Coordinate of current location
 * @param alt Current altitude
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mad_current_location_send(mavlink_channel_t chan, float x, float y, float alt)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN];
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);
	_mav_put_float(buf, 8, alt);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION, buf, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION, buf, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN);
#endif
#else
	mavlink_mad_current_location_t packet;
	packet.x = x;
	packet.y = y;
	packet.alt = alt;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION, (const char *)&packet, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION, (const char *)&packet, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN);
#endif
#endif
}

#if MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mad_current_location_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float x, float y, float alt)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);
	_mav_put_float(buf, 8, alt);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION, buf, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION, buf, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN);
#endif
#else
	mavlink_mad_current_location_t *packet = (mavlink_mad_current_location_t *)msgbuf;
	packet->x = x;
	packet->y = y;
	packet->alt = alt;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION, (const char *)packet, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION, (const char *)packet, MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN);
#endif
#endif
}
#endif

#endif

// MESSAGE MAD_CURRENT_LOCATION UNPACKING


/**
 * @brief Get field x from mad_current_location message
 *
 * @return X Coordinate of current location
 */
static inline float mavlink_msg_mad_current_location_get_x(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field y from mad_current_location message
 *
 * @return Y Coordinate of current location
 */
static inline float mavlink_msg_mad_current_location_get_y(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field alt from mad_current_location message
 *
 * @return Current altitude
 */
static inline float mavlink_msg_mad_current_location_get_alt(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Decode a mad_current_location message into a struct
 *
 * @param msg The message to decode
 * @param mad_current_location C-struct to decode the message contents into
 */
static inline void mavlink_msg_mad_current_location_decode(const mavlink_message_t* msg, mavlink_mad_current_location_t* mad_current_location)
{
#if MAVLINK_NEED_BYTE_SWAP
	mad_current_location->x = mavlink_msg_mad_current_location_get_x(msg);
	mad_current_location->y = mavlink_msg_mad_current_location_get_y(msg);
	mad_current_location->alt = mavlink_msg_mad_current_location_get_alt(msg);
#else
	memcpy(mad_current_location, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_MAD_CURRENT_LOCATION_LEN);
#endif
}
