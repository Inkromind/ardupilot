// MESSAGE MAD_GET_LOGGING PACKING

#define MAVLINK_MSG_ID_MAD_GET_LOGGING 210

typedef struct __mavlink_mad_get_logging_t
{
 uint8_t drone_id; ///< Drone id
} mavlink_mad_get_logging_t;

#define MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN 1
#define MAVLINK_MSG_ID_210_LEN 1

#define MAVLINK_MSG_ID_MAD_GET_LOGGING_CRC 147
#define MAVLINK_MSG_ID_210_CRC 147



#define MAVLINK_MESSAGE_INFO_MAD_GET_LOGGING { \
	"MAD_GET_LOGGING", \
	1, \
	{  { "drone_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_mad_get_logging_t, drone_id) }, \
         } \
}


/**
 * @brief Pack a mad_get_logging message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param drone_id Drone id
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_get_logging_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint8_t drone_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN];
	_mav_put_uint8_t(buf, 0, drone_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN);
#else
	mavlink_mad_get_logging_t packet;
	packet.drone_id = drone_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_GET_LOGGING;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN, MAVLINK_MSG_ID_MAD_GET_LOGGING_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN);
#endif
}

/**
 * @brief Pack a mad_get_logging message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param drone_id Drone id
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_get_logging_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint8_t drone_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN];
	_mav_put_uint8_t(buf, 0, drone_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN);
#else
	mavlink_mad_get_logging_t packet;
	packet.drone_id = drone_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_GET_LOGGING;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN, MAVLINK_MSG_ID_MAD_GET_LOGGING_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN);
#endif
}

/**
 * @brief Encode a mad_get_logging struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mad_get_logging C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_get_logging_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mad_get_logging_t* mad_get_logging)
{
	return mavlink_msg_mad_get_logging_pack(system_id, component_id, msg, mad_get_logging->drone_id);
}

/**
 * @brief Encode a mad_get_logging struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mad_get_logging C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_get_logging_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mad_get_logging_t* mad_get_logging)
{
	return mavlink_msg_mad_get_logging_pack_chan(system_id, component_id, chan, msg, mad_get_logging->drone_id);
}

/**
 * @brief Send a mad_get_logging message
 * @param chan MAVLink channel to send the message
 *
 * @param drone_id Drone id
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mad_get_logging_send(mavlink_channel_t chan, uint8_t drone_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN];
	_mav_put_uint8_t(buf, 0, drone_id);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_GET_LOGGING, buf, MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN, MAVLINK_MSG_ID_MAD_GET_LOGGING_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_GET_LOGGING, buf, MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN);
#endif
#else
	mavlink_mad_get_logging_t packet;
	packet.drone_id = drone_id;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_GET_LOGGING, (const char *)&packet, MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN, MAVLINK_MSG_ID_MAD_GET_LOGGING_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_GET_LOGGING, (const char *)&packet, MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN);
#endif
#endif
}

#if MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mad_get_logging_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t drone_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_uint8_t(buf, 0, drone_id);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_GET_LOGGING, buf, MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN, MAVLINK_MSG_ID_MAD_GET_LOGGING_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_GET_LOGGING, buf, MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN);
#endif
#else
	mavlink_mad_get_logging_t *packet = (mavlink_mad_get_logging_t *)msgbuf;
	packet->drone_id = drone_id;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_GET_LOGGING, (const char *)packet, MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN, MAVLINK_MSG_ID_MAD_GET_LOGGING_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_GET_LOGGING, (const char *)packet, MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN);
#endif
#endif
}
#endif

#endif

// MESSAGE MAD_GET_LOGGING UNPACKING


/**
 * @brief Get field drone_id from mad_get_logging message
 *
 * @return Drone id
 */
static inline uint8_t mavlink_msg_mad_get_logging_get_drone_id(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a mad_get_logging message into a struct
 *
 * @param msg The message to decode
 * @param mad_get_logging C-struct to decode the message contents into
 */
static inline void mavlink_msg_mad_get_logging_decode(const mavlink_message_t* msg, mavlink_mad_get_logging_t* mad_get_logging)
{
#if MAVLINK_NEED_BYTE_SWAP
	mad_get_logging->drone_id = mavlink_msg_mad_get_logging_get_drone_id(msg);
#else
	memcpy(mad_get_logging, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_MAD_GET_LOGGING_LEN);
#endif
}
