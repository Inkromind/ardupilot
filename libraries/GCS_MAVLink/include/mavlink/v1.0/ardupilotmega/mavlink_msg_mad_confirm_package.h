// MESSAGE MAD_CONFIRM_PACKAGE PACKING

#define MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE 201

typedef struct __mavlink_mad_confirm_package_t
{
 float estimate; ///< Package estimate
 uint8_t package_id; ///< Unique package id
} mavlink_mad_confirm_package_t;

#define MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN 5
#define MAVLINK_MSG_ID_201_LEN 5

#define MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_CRC 138
#define MAVLINK_MSG_ID_201_CRC 138



#define MAVLINK_MESSAGE_INFO_MAD_CONFIRM_PACKAGE { \
	"MAD_CONFIRM_PACKAGE", \
	2, \
	{  { "estimate", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_mad_confirm_package_t, estimate) }, \
         { "package_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_mad_confirm_package_t, package_id) }, \
         } \
}


/**
 * @brief Pack a mad_confirm_package message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param package_id Unique package id
 * @param estimate Package estimate
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_confirm_package_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint8_t package_id, float estimate)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN];
	_mav_put_float(buf, 0, estimate);
	_mav_put_uint8_t(buf, 4, package_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN);
#else
	mavlink_mad_confirm_package_t packet;
	packet.estimate = estimate;
	packet.package_id = package_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN);
#endif
}

/**
 * @brief Pack a mad_confirm_package message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param package_id Unique package id
 * @param estimate Package estimate
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_confirm_package_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint8_t package_id,float estimate)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN];
	_mav_put_float(buf, 0, estimate);
	_mav_put_uint8_t(buf, 4, package_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN);
#else
	mavlink_mad_confirm_package_t packet;
	packet.estimate = estimate;
	packet.package_id = package_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN);
#endif
}

/**
 * @brief Encode a mad_confirm_package struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mad_confirm_package C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_confirm_package_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mad_confirm_package_t* mad_confirm_package)
{
	return mavlink_msg_mad_confirm_package_pack(system_id, component_id, msg, mad_confirm_package->package_id, mad_confirm_package->estimate);
}

/**
 * @brief Encode a mad_confirm_package struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mad_confirm_package C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_confirm_package_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mad_confirm_package_t* mad_confirm_package)
{
	return mavlink_msg_mad_confirm_package_pack_chan(system_id, component_id, chan, msg, mad_confirm_package->package_id, mad_confirm_package->estimate);
}

/**
 * @brief Send a mad_confirm_package message
 * @param chan MAVLink channel to send the message
 *
 * @param package_id Unique package id
 * @param estimate Package estimate
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mad_confirm_package_send(mavlink_channel_t chan, uint8_t package_id, float estimate)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN];
	_mav_put_float(buf, 0, estimate);
	_mav_put_uint8_t(buf, 4, package_id);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE, buf, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE, buf, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN);
#endif
#else
	mavlink_mad_confirm_package_t packet;
	packet.estimate = estimate;
	packet.package_id = package_id;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE, (const char *)&packet, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE, (const char *)&packet, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN);
#endif
#endif
}

#if MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mad_confirm_package_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t package_id, float estimate)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_float(buf, 0, estimate);
	_mav_put_uint8_t(buf, 4, package_id);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE, buf, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE, buf, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN);
#endif
#else
	mavlink_mad_confirm_package_t *packet = (mavlink_mad_confirm_package_t *)msgbuf;
	packet->estimate = estimate;
	packet->package_id = package_id;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE, (const char *)packet, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE, (const char *)packet, MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN);
#endif
#endif
}
#endif

#endif

// MESSAGE MAD_CONFIRM_PACKAGE UNPACKING


/**
 * @brief Get field package_id from mad_confirm_package message
 *
 * @return Unique package id
 */
static inline uint8_t mavlink_msg_mad_confirm_package_get_package_id(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field estimate from mad_confirm_package message
 *
 * @return Package estimate
 */
static inline float mavlink_msg_mad_confirm_package_get_estimate(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Decode a mad_confirm_package message into a struct
 *
 * @param msg The message to decode
 * @param mad_confirm_package C-struct to decode the message contents into
 */
static inline void mavlink_msg_mad_confirm_package_decode(const mavlink_message_t* msg, mavlink_mad_confirm_package_t* mad_confirm_package)
{
#if MAVLINK_NEED_BYTE_SWAP
	mad_confirm_package->estimate = mavlink_msg_mad_confirm_package_get_estimate(msg);
	mad_confirm_package->package_id = mavlink_msg_mad_confirm_package_get_package_id(msg);
#else
	memcpy(mad_confirm_package, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_MAD_CONFIRM_PACKAGE_LEN);
#endif
}
