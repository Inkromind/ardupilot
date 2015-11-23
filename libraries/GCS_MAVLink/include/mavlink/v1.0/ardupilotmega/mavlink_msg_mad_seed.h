// MESSAGE MAD_SEED PACKING

#define MAVLINK_MSG_ID_MAD_SEED 211

typedef struct __mavlink_mad_seed_t
{
 uint8_t seed; ///< New Seed
} mavlink_mad_seed_t;

#define MAVLINK_MSG_ID_MAD_SEED_LEN 1
#define MAVLINK_MSG_ID_211_LEN 1

#define MAVLINK_MSG_ID_MAD_SEED_CRC 240
#define MAVLINK_MSG_ID_211_CRC 240



#define MAVLINK_MESSAGE_INFO_MAD_SEED { \
	"MAD_SEED", \
	1, \
	{  { "seed", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_mad_seed_t, seed) }, \
         } \
}


/**
 * @brief Pack a mad_seed message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param seed New Seed
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_seed_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint8_t seed)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_SEED_LEN];
	_mav_put_uint8_t(buf, 0, seed);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_SEED_LEN);
#else
	mavlink_mad_seed_t packet;
	packet.seed = seed;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_SEED_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_SEED;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_SEED_LEN, MAVLINK_MSG_ID_MAD_SEED_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_SEED_LEN);
#endif
}

/**
 * @brief Pack a mad_seed message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param seed New Seed
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_seed_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint8_t seed)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_SEED_LEN];
	_mav_put_uint8_t(buf, 0, seed);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_SEED_LEN);
#else
	mavlink_mad_seed_t packet;
	packet.seed = seed;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_SEED_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_SEED;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_SEED_LEN, MAVLINK_MSG_ID_MAD_SEED_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_SEED_LEN);
#endif
}

/**
 * @brief Encode a mad_seed struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mad_seed C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_seed_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mad_seed_t* mad_seed)
{
	return mavlink_msg_mad_seed_pack(system_id, component_id, msg, mad_seed->seed);
}

/**
 * @brief Encode a mad_seed struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mad_seed C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_seed_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mad_seed_t* mad_seed)
{
	return mavlink_msg_mad_seed_pack_chan(system_id, component_id, chan, msg, mad_seed->seed);
}

/**
 * @brief Send a mad_seed message
 * @param chan MAVLink channel to send the message
 *
 * @param seed New Seed
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mad_seed_send(mavlink_channel_t chan, uint8_t seed)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_SEED_LEN];
	_mav_put_uint8_t(buf, 0, seed);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_SEED, buf, MAVLINK_MSG_ID_MAD_SEED_LEN, MAVLINK_MSG_ID_MAD_SEED_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_SEED, buf, MAVLINK_MSG_ID_MAD_SEED_LEN);
#endif
#else
	mavlink_mad_seed_t packet;
	packet.seed = seed;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_SEED, (const char *)&packet, MAVLINK_MSG_ID_MAD_SEED_LEN, MAVLINK_MSG_ID_MAD_SEED_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_SEED, (const char *)&packet, MAVLINK_MSG_ID_MAD_SEED_LEN);
#endif
#endif
}

#if MAVLINK_MSG_ID_MAD_SEED_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mad_seed_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t seed)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_uint8_t(buf, 0, seed);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_SEED, buf, MAVLINK_MSG_ID_MAD_SEED_LEN, MAVLINK_MSG_ID_MAD_SEED_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_SEED, buf, MAVLINK_MSG_ID_MAD_SEED_LEN);
#endif
#else
	mavlink_mad_seed_t *packet = (mavlink_mad_seed_t *)msgbuf;
	packet->seed = seed;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_SEED, (const char *)packet, MAVLINK_MSG_ID_MAD_SEED_LEN, MAVLINK_MSG_ID_MAD_SEED_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_SEED, (const char *)packet, MAVLINK_MSG_ID_MAD_SEED_LEN);
#endif
#endif
}
#endif

#endif

// MESSAGE MAD_SEED UNPACKING


/**
 * @brief Get field seed from mad_seed message
 *
 * @return New Seed
 */
static inline uint8_t mavlink_msg_mad_seed_get_seed(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a mad_seed message into a struct
 *
 * @param msg The message to decode
 * @param mad_seed C-struct to decode the message contents into
 */
static inline void mavlink_msg_mad_seed_decode(const mavlink_message_t* msg, mavlink_mad_seed_t* mad_seed)
{
#if MAVLINK_NEED_BYTE_SWAP
	mad_seed->seed = mavlink_msg_mad_seed_get_seed(msg);
#else
	memcpy(mad_seed, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_MAD_SEED_LEN);
#endif
}
