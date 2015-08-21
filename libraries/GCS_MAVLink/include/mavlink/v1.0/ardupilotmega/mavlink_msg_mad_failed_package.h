// MESSAGE MAD_FAILED_PACKAGE PACKING

#define MAVLINK_MSG_ID_MAD_FAILED_PACKAGE 206

typedef struct __mavlink_mad_failed_package_t
{
 uint8_t package_id; ///< Unique package id
} mavlink_mad_failed_package_t;

#define MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN 1
#define MAVLINK_MSG_ID_206_LEN 1

#define MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_CRC 237
#define MAVLINK_MSG_ID_206_CRC 237



#define MAVLINK_MESSAGE_INFO_MAD_FAILED_PACKAGE { \
	"MAD_FAILED_PACKAGE", \
	1, \
	{  { "package_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_mad_failed_package_t, package_id) }, \
         } \
}


/**
 * @brief Pack a mad_failed_package message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param package_id Unique package id
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_failed_package_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint8_t package_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN];
	_mav_put_uint8_t(buf, 0, package_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN);
#else
	mavlink_mad_failed_package_t packet;
	packet.package_id = package_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_FAILED_PACKAGE;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN);
#endif
}

/**
 * @brief Pack a mad_failed_package message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param package_id Unique package id
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_failed_package_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint8_t package_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN];
	_mav_put_uint8_t(buf, 0, package_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN);
#else
	mavlink_mad_failed_package_t packet;
	packet.package_id = package_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_FAILED_PACKAGE;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN);
#endif
}

/**
 * @brief Encode a mad_failed_package struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mad_failed_package C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_failed_package_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mad_failed_package_t* mad_failed_package)
{
	return mavlink_msg_mad_failed_package_pack(system_id, component_id, msg, mad_failed_package->package_id);
}

/**
 * @brief Encode a mad_failed_package struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mad_failed_package C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_failed_package_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mad_failed_package_t* mad_failed_package)
{
	return mavlink_msg_mad_failed_package_pack_chan(system_id, component_id, chan, msg, mad_failed_package->package_id);
}

/**
 * @brief Send a mad_failed_package message
 * @param chan MAVLink channel to send the message
 *
 * @param package_id Unique package id
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mad_failed_package_send(mavlink_channel_t chan, uint8_t package_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN];
	_mav_put_uint8_t(buf, 0, package_id);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE, buf, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE, buf, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN);
#endif
#else
	mavlink_mad_failed_package_t packet;
	packet.package_id = package_id;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE, (const char *)&packet, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE, (const char *)&packet, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN);
#endif
#endif
}

#if MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mad_failed_package_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t package_id)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_uint8_t(buf, 0, package_id);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE, buf, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE, buf, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN);
#endif
#else
	mavlink_mad_failed_package_t *packet = (mavlink_mad_failed_package_t *)msgbuf;
	packet->package_id = package_id;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE, (const char *)packet, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE, (const char *)packet, MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN);
#endif
#endif
}
#endif

#endif

// MESSAGE MAD_FAILED_PACKAGE UNPACKING


/**
 * @brief Get field package_id from mad_failed_package message
 *
 * @return Unique package id
 */
static inline uint8_t mavlink_msg_mad_failed_package_get_package_id(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a mad_failed_package message into a struct
 *
 * @param msg The message to decode
 * @param mad_failed_package C-struct to decode the message contents into
 */
static inline void mavlink_msg_mad_failed_package_decode(const mavlink_message_t* msg, mavlink_mad_failed_package_t* mad_failed_package)
{
#if MAVLINK_NEED_BYTE_SWAP
	mad_failed_package->package_id = mavlink_msg_mad_failed_package_get_package_id(msg);
#else
	memcpy(mad_failed_package, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_MAD_FAILED_PACKAGE_LEN);
#endif
}
