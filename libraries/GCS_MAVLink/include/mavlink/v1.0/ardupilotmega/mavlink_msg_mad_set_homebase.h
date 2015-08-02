// MESSAGE MAD_SET_HOMEBASE PACKING

#define MAVLINK_MSG_ID_MAD_SET_HOMEBASE 196

typedef struct __mavlink_mad_set_homebase_t
{
 float x; ///< X Coordinate of homebase
 float y; ///< Y Coordinate of homebase
} mavlink_mad_set_homebase_t;

#define MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN 8
#define MAVLINK_MSG_ID_196_LEN 8

#define MAVLINK_MSG_ID_MAD_SET_HOMEBASE_CRC 32
#define MAVLINK_MSG_ID_196_CRC 32



#define MAVLINK_MESSAGE_INFO_MAD_SET_HOMEBASE { \
	"MAD_SET_HOMEBASE", \
	2, \
	{  { "x", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_mad_set_homebase_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_mad_set_homebase_t, y) }, \
         } \
}


/**
 * @brief Pack a mad_set_homebase message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param x X Coordinate of homebase
 * @param y Y Coordinate of homebase
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_set_homebase_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       float x, float y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN];
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN);
#else
	mavlink_mad_set_homebase_t packet;
	packet.x = x;
	packet.y = y;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_SET_HOMEBASE;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN);
#endif
}

/**
 * @brief Pack a mad_set_homebase message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param x X Coordinate of homebase
 * @param y Y Coordinate of homebase
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_set_homebase_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           float x,float y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN];
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN);
#else
	mavlink_mad_set_homebase_t packet;
	packet.x = x;
	packet.y = y;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_SET_HOMEBASE;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN);
#endif
}

/**
 * @brief Encode a mad_set_homebase struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mad_set_homebase C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_set_homebase_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mad_set_homebase_t* mad_set_homebase)
{
	return mavlink_msg_mad_set_homebase_pack(system_id, component_id, msg, mad_set_homebase->x, mad_set_homebase->y);
}

/**
 * @brief Encode a mad_set_homebase struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mad_set_homebase C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_set_homebase_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mad_set_homebase_t* mad_set_homebase)
{
	return mavlink_msg_mad_set_homebase_pack_chan(system_id, component_id, chan, msg, mad_set_homebase->x, mad_set_homebase->y);
}

/**
 * @brief Send a mad_set_homebase message
 * @param chan MAVLink channel to send the message
 *
 * @param x X Coordinate of homebase
 * @param y Y Coordinate of homebase
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mad_set_homebase_send(mavlink_channel_t chan, float x, float y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN];
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_SET_HOMEBASE, buf, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_SET_HOMEBASE, buf, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN);
#endif
#else
	mavlink_mad_set_homebase_t packet;
	packet.x = x;
	packet.y = y;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_SET_HOMEBASE, (const char *)&packet, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_SET_HOMEBASE, (const char *)&packet, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN);
#endif
#endif
}

#if MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mad_set_homebase_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float x, float y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_float(buf, 0, x);
	_mav_put_float(buf, 4, y);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_SET_HOMEBASE, buf, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_SET_HOMEBASE, buf, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN);
#endif
#else
	mavlink_mad_set_homebase_t *packet = (mavlink_mad_set_homebase_t *)msgbuf;
	packet->x = x;
	packet->y = y;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_SET_HOMEBASE, (const char *)packet, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_SET_HOMEBASE, (const char *)packet, MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN);
#endif
#endif
}
#endif

#endif

// MESSAGE MAD_SET_HOMEBASE UNPACKING


/**
 * @brief Get field x from mad_set_homebase message
 *
 * @return X Coordinate of homebase
 */
static inline float mavlink_msg_mad_set_homebase_get_x(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field y from mad_set_homebase message
 *
 * @return Y Coordinate of homebase
 */
static inline float mavlink_msg_mad_set_homebase_get_y(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Decode a mad_set_homebase message into a struct
 *
 * @param msg The message to decode
 * @param mad_set_homebase C-struct to decode the message contents into
 */
static inline void mavlink_msg_mad_set_homebase_decode(const mavlink_message_t* msg, mavlink_mad_set_homebase_t* mad_set_homebase)
{
#if MAVLINK_NEED_BYTE_SWAP
	mad_set_homebase->x = mavlink_msg_mad_set_homebase_get_x(msg);
	mad_set_homebase->y = mavlink_msg_mad_set_homebase_get_y(msg);
#else
	memcpy(mad_set_homebase, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_MAD_SET_HOMEBASE_LEN);
#endif
}
