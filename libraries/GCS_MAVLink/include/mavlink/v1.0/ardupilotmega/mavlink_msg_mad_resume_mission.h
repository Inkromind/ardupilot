// MESSAGE MAD_RESUME_MISSION PACKING

#define MAVLINK_MSG_ID_MAD_RESUME_MISSION 192

typedef struct __mavlink_mad_resume_mission_t
{
 uint8_t target_system; ///< System which should execute the command
 uint8_t target_component; ///< Component which should execute the command, 0 for all components
} mavlink_mad_resume_mission_t;

#define MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN 2
#define MAVLINK_MSG_ID_192_LEN 2

#define MAVLINK_MSG_ID_MAD_RESUME_MISSION_CRC 196
#define MAVLINK_MSG_ID_192_CRC 196



#define MAVLINK_MESSAGE_INFO_MAD_RESUME_MISSION { \
	"MAD_RESUME_MISSION", \
	2, \
	{  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_mad_resume_mission_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_mad_resume_mission_t, target_component) }, \
         } \
}


/**
 * @brief Pack a mad_resume_mission message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system System which should execute the command
 * @param target_component Component which should execute the command, 0 for all components
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_resume_mission_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN];
	_mav_put_uint8_t(buf, 0, target_system);
	_mav_put_uint8_t(buf, 1, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN);
#else
	mavlink_mad_resume_mission_t packet;
	packet.target_system = target_system;
	packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_RESUME_MISSION;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN, MAVLINK_MSG_ID_MAD_RESUME_MISSION_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN);
#endif
}

/**
 * @brief Pack a mad_resume_mission message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system System which should execute the command
 * @param target_component Component which should execute the command, 0 for all components
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_resume_mission_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint8_t target_system,uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN];
	_mav_put_uint8_t(buf, 0, target_system);
	_mav_put_uint8_t(buf, 1, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN);
#else
	mavlink_mad_resume_mission_t packet;
	packet.target_system = target_system;
	packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_RESUME_MISSION;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN, MAVLINK_MSG_ID_MAD_RESUME_MISSION_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN);
#endif
}

/**
 * @brief Encode a mad_resume_mission struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mad_resume_mission C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_resume_mission_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mad_resume_mission_t* mad_resume_mission)
{
	return mavlink_msg_mad_resume_mission_pack(system_id, component_id, msg, mad_resume_mission->target_system, mad_resume_mission->target_component);
}

/**
 * @brief Encode a mad_resume_mission struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mad_resume_mission C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_resume_mission_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mad_resume_mission_t* mad_resume_mission)
{
	return mavlink_msg_mad_resume_mission_pack_chan(system_id, component_id, chan, msg, mad_resume_mission->target_system, mad_resume_mission->target_component);
}

/**
 * @brief Send a mad_resume_mission message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system System which should execute the command
 * @param target_component Component which should execute the command, 0 for all components
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mad_resume_mission_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN];
	_mav_put_uint8_t(buf, 0, target_system);
	_mav_put_uint8_t(buf, 1, target_component);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_RESUME_MISSION, buf, MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN, MAVLINK_MSG_ID_MAD_RESUME_MISSION_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_RESUME_MISSION, buf, MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN);
#endif
#else
	mavlink_mad_resume_mission_t packet;
	packet.target_system = target_system;
	packet.target_component = target_component;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_RESUME_MISSION, (const char *)&packet, MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN, MAVLINK_MSG_ID_MAD_RESUME_MISSION_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_RESUME_MISSION, (const char *)&packet, MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN);
#endif
#endif
}

#if MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mad_resume_mission_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_uint8_t(buf, 0, target_system);
	_mav_put_uint8_t(buf, 1, target_component);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_RESUME_MISSION, buf, MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN, MAVLINK_MSG_ID_MAD_RESUME_MISSION_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_RESUME_MISSION, buf, MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN);
#endif
#else
	mavlink_mad_resume_mission_t *packet = (mavlink_mad_resume_mission_t *)msgbuf;
	packet->target_system = target_system;
	packet->target_component = target_component;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_RESUME_MISSION, (const char *)packet, MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN, MAVLINK_MSG_ID_MAD_RESUME_MISSION_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_RESUME_MISSION, (const char *)packet, MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN);
#endif
#endif
}
#endif

#endif

// MESSAGE MAD_RESUME_MISSION UNPACKING


/**
 * @brief Get field target_system from mad_resume_mission message
 *
 * @return System which should execute the command
 */
static inline uint8_t mavlink_msg_mad_resume_mission_get_target_system(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field target_component from mad_resume_mission message
 *
 * @return Component which should execute the command, 0 for all components
 */
static inline uint8_t mavlink_msg_mad_resume_mission_get_target_component(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Decode a mad_resume_mission message into a struct
 *
 * @param msg The message to decode
 * @param mad_resume_mission C-struct to decode the message contents into
 */
static inline void mavlink_msg_mad_resume_mission_decode(const mavlink_message_t* msg, mavlink_mad_resume_mission_t* mad_resume_mission)
{
#if MAVLINK_NEED_BYTE_SWAP
	mad_resume_mission->target_system = mavlink_msg_mad_resume_mission_get_target_system(msg);
	mad_resume_mission->target_component = mavlink_msg_mad_resume_mission_get_target_component(msg);
#else
	memcpy(mad_resume_mission, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_MAD_RESUME_MISSION_LEN);
#endif
}
