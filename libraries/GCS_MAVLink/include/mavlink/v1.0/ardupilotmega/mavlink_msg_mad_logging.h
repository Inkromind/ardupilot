// MESSAGE MAD_LOGGING PACKING

#define MAVLINK_MSG_ID_MAD_LOGGING 210

typedef struct __mavlink_mad_logging_t
{
 float x; ///< X-position
 float y; ///< Y-position
 float alt; ///< Current altitude
 float assAlt; ///< Assigned altitude
 float dev; ///< Deviation
} mavlink_mad_logging_t;

#define MAVLINK_MSG_ID_MAD_LOGGING_LEN 20
#define MAVLINK_MSG_ID_210_LEN 20

#define MAVLINK_MSG_ID_MAD_LOGGING_CRC 114
#define MAVLINK_MSG_ID_210_CRC 114



#define MAVLINK_MESSAGE_INFO_MAD_LOGGING { \
	"MAD_LOGGING", \
	5, \
	{  { "x", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_mad_logging_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_mad_logging_t, y) }, \
         { "alt", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_mad_logging_t, alt) }, \
         { "assAlt", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_mad_logging_t, assAlt) }, \
         { "dev", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_mad_logging_t, dev) }, \
         } \
}


/**
 * @brief Pack a mad_logging message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param x X-position
 * @param y Y-position
 * @param alt Current altitude
 * @param assAlt Assigned altitude
 * @param dev Deviation
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_logging_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       float x, float y, float alt, float assAlt, float dev)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_LOGGING_LEN];
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);
	_mav_put_float(buf, 8, alt);
	_mav_put_float(buf, 12, assAlt);
	_mav_put_float(buf, 16, dev);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_LOGGING_LEN);
#else
	mavlink_mad_logging_t packet;
	packet.x = x;
	packet.y = y;
	packet.alt = alt;
	packet.assAlt = assAlt;
	packet.dev = dev;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_LOGGING_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_LOGGING;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_LOGGING_LEN, MAVLINK_MSG_ID_MAD_LOGGING_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_LOGGING_LEN);
#endif
}

/**
 * @brief Pack a mad_logging message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param x X-position
 * @param y Y-position
 * @param alt Current altitude
 * @param assAlt Assigned altitude
 * @param dev Deviation
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_logging_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           float x,float y,float alt,float assAlt,float dev)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_LOGGING_LEN];
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);
	_mav_put_float(buf, 8, alt);
	_mav_put_float(buf, 12, assAlt);
	_mav_put_float(buf, 16, dev);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_LOGGING_LEN);
#else
	mavlink_mad_logging_t packet;
	packet.x = x;
	packet.y = y;
	packet.alt = alt;
	packet.assAlt = assAlt;
	packet.dev = dev;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_LOGGING_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_LOGGING;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_LOGGING_LEN, MAVLINK_MSG_ID_MAD_LOGGING_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_LOGGING_LEN);
#endif
}

/**
 * @brief Encode a mad_logging struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mad_logging C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_logging_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mad_logging_t* mad_logging)
{
	return mavlink_msg_mad_logging_pack(system_id, component_id, msg, mad_logging->x, mad_logging->y, mad_logging->alt, mad_logging->assAlt, mad_logging->dev);
}

/**
 * @brief Encode a mad_logging struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mad_logging C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_logging_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mad_logging_t* mad_logging)
{
	return mavlink_msg_mad_logging_pack_chan(system_id, component_id, chan, msg, mad_logging->x, mad_logging->y, mad_logging->alt, mad_logging->assAlt, mad_logging->dev);
}

/**
 * @brief Send a mad_logging message
 * @param chan MAVLink channel to send the message
 *
 * @param x X-position
 * @param y Y-position
 * @param alt Current altitude
 * @param assAlt Assigned altitude
 * @param dev Deviation
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mad_logging_send(mavlink_channel_t chan, float x, float y, float alt, float assAlt, float dev)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_LOGGING_LEN];
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);
	_mav_put_float(buf, 8, alt);
	_mav_put_float(buf, 12, assAlt);
	_mav_put_float(buf, 16, dev);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_LOGGING, buf, MAVLINK_MSG_ID_MAD_LOGGING_LEN, MAVLINK_MSG_ID_MAD_LOGGING_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_LOGGING, buf, MAVLINK_MSG_ID_MAD_LOGGING_LEN);
#endif
#else
	mavlink_mad_logging_t packet;
	packet.x = x;
	packet.y = y;
	packet.alt = alt;
	packet.assAlt = assAlt;
	packet.dev = dev;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_LOGGING, (const char *)&packet, MAVLINK_MSG_ID_MAD_LOGGING_LEN, MAVLINK_MSG_ID_MAD_LOGGING_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_LOGGING, (const char *)&packet, MAVLINK_MSG_ID_MAD_LOGGING_LEN);
#endif
#endif
}

#if MAVLINK_MSG_ID_MAD_LOGGING_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mad_logging_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float x, float y, float alt, float assAlt, float dev)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);
	_mav_put_float(buf, 8, alt);
	_mav_put_float(buf, 12, assAlt);
	_mav_put_float(buf, 16, dev);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_LOGGING, buf, MAVLINK_MSG_ID_MAD_LOGGING_LEN, MAVLINK_MSG_ID_MAD_LOGGING_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_LOGGING, buf, MAVLINK_MSG_ID_MAD_LOGGING_LEN);
#endif
#else
	mavlink_mad_logging_t *packet = (mavlink_mad_logging_t *)msgbuf;
	packet->x = x;
	packet->y = y;
	packet->alt = alt;
	packet->assAlt = assAlt;
	packet->dev = dev;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_LOGGING, (const char *)packet, MAVLINK_MSG_ID_MAD_LOGGING_LEN, MAVLINK_MSG_ID_MAD_LOGGING_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_LOGGING, (const char *)packet, MAVLINK_MSG_ID_MAD_LOGGING_LEN);
#endif
#endif
}
#endif

#endif

// MESSAGE MAD_LOGGING UNPACKING


/**
 * @brief Get field x from mad_logging message
 *
 * @return X-position
 */
static inline float mavlink_msg_mad_logging_get_x(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field y from mad_logging message
 *
 * @return Y-position
 */
static inline float mavlink_msg_mad_logging_get_y(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field alt from mad_logging message
 *
 * @return Current altitude
 */
static inline float mavlink_msg_mad_logging_get_alt(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field assAlt from mad_logging message
 *
 * @return Assigned altitude
 */
static inline float mavlink_msg_mad_logging_get_assAlt(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field dev from mad_logging message
 *
 * @return Deviation
 */
static inline float mavlink_msg_mad_logging_get_dev(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Decode a mad_logging message into a struct
 *
 * @param msg The message to decode
 * @param mad_logging C-struct to decode the message contents into
 */
static inline void mavlink_msg_mad_logging_decode(const mavlink_message_t* msg, mavlink_mad_logging_t* mad_logging)
{
#if MAVLINK_NEED_BYTE_SWAP
	mad_logging->x = mavlink_msg_mad_logging_get_x(msg);
	mad_logging->y = mavlink_msg_mad_logging_get_y(msg);
	mad_logging->alt = mavlink_msg_mad_logging_get_alt(msg);
	mad_logging->assAlt = mavlink_msg_mad_logging_get_assAlt(msg);
	mad_logging->dev = mavlink_msg_mad_logging_get_dev(msg);
#else
	memcpy(mad_logging, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_MAD_LOGGING_LEN);
#endif
}
