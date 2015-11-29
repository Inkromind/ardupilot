// MESSAGE MAD_HEARTBEAT PACKING

#define MAVLINK_MSG_ID_MAD_HEARTBEAT 212

typedef struct __mavlink_mad_heartbeat_t
{
 uint8_t drone_id; ///< Drone id
} mavlink_mad_heartbeat_t;

#define MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN 1
#define MAVLINK_MSG_ID_212_LEN 1

#define MAVLINK_MSG_ID_MAD_HEARTBEAT_CRC 159
#define MAVLINK_MSG_ID_212_CRC 159



#define MAVLINK_MESSAGE_INFO_MAD_HEARTBEAT { \
	"MAD_HEARTBEAT", \
	1, \
	{  { "drone_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_mad_heartbeat_t, drone_id) }, \
         } \
}


/**
 * @brief Pack a mad_heartbeat message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param drone_id Drone id
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_heartbeat_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint8_t drone_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN];
	_mav_put_uint8_t(buf, 0, drone_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN);
#else
	mavlink_mad_heartbeat_t packet;
	packet.drone_id = drone_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_HEARTBEAT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN, MAVLINK_MSG_ID_MAD_HEARTBEAT_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN);
#endif
}

/**
 * @brief Pack a mad_heartbeat message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param drone_id Drone id
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_heartbeat_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint8_t drone_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN];
	_mav_put_uint8_t(buf, 0, drone_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN);
#else
	mavlink_mad_heartbeat_t packet;
	packet.drone_id = drone_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_HEARTBEAT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN, MAVLINK_MSG_ID_MAD_HEARTBEAT_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN);
#endif
}

/**
 * @brief Encode a mad_heartbeat struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mad_heartbeat C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_heartbeat_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mad_heartbeat_t* mad_heartbeat)
{
	return mavlink_msg_mad_heartbeat_pack(system_id, component_id, msg, mad_heartbeat->drone_id);
}

/**
 * @brief Encode a mad_heartbeat struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mad_heartbeat C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_heartbeat_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mad_heartbeat_t* mad_heartbeat)
{
	return mavlink_msg_mad_heartbeat_pack_chan(system_id, component_id, chan, msg, mad_heartbeat->drone_id);
}

/**
 * @brief Send a mad_heartbeat message
 * @param chan MAVLink channel to send the message
 *
 * @param drone_id Drone id
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mad_heartbeat_send(mavlink_channel_t chan, uint8_t drone_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN];
	_mav_put_uint8_t(buf, 0, drone_id);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_HEARTBEAT, buf, MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN, MAVLINK_MSG_ID_MAD_HEARTBEAT_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_HEARTBEAT, buf, MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN);
#endif
#else
	mavlink_mad_heartbeat_t packet;
	packet.drone_id = drone_id;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_HEARTBEAT, (const char *)&packet, MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN, MAVLINK_MSG_ID_MAD_HEARTBEAT_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_HEARTBEAT, (const char *)&packet, MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN);
#endif
#endif
}

#if MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mad_heartbeat_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t drone_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_uint8_t(buf, 0, drone_id);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_HEARTBEAT, buf, MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN, MAVLINK_MSG_ID_MAD_HEARTBEAT_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_HEARTBEAT, buf, MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN);
#endif
#else
	mavlink_mad_heartbeat_t *packet = (mavlink_mad_heartbeat_t *)msgbuf;
	packet->drone_id = drone_id;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_HEARTBEAT, (const char *)packet, MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN, MAVLINK_MSG_ID_MAD_HEARTBEAT_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_HEARTBEAT, (const char *)packet, MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN);
#endif
#endif
}
#endif

#endif

// MESSAGE MAD_HEARTBEAT UNPACKING


/**
 * @brief Get field drone_id from mad_heartbeat message
 *
 * @return Drone id
 */
static inline uint8_t mavlink_msg_mad_heartbeat_get_drone_id(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a mad_heartbeat message into a struct
 *
 * @param msg The message to decode
 * @param mad_heartbeat C-struct to decode the message contents into
 */
static inline void mavlink_msg_mad_heartbeat_decode(const mavlink_message_t* msg, mavlink_mad_heartbeat_t* mad_heartbeat)
{
#if MAVLINK_NEED_BYTE_SWAP
	mad_heartbeat->drone_id = mavlink_msg_mad_heartbeat_get_drone_id(msg);
#else
	memcpy(mad_heartbeat, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_MAD_HEARTBEAT_LEN);
#endif
}
