// MESSAGE MAD_LOGGING_REPLY PACKING

#define MAVLINK_MSG_ID_MAD_LOGGING_REPLY 211

typedef struct __mavlink_mad_logging_reply_t
{
 float flight_levels; ///< Summed flight levels
 float distance; ///< Distance travelled
 uint16_t retries; ///< Total reservation retries
 uint16_t rounds; ///< Total reservation rounds
 uint16_t res_failures; ///< Total failed reservations
 uint16_t res_succes; ///< Total succesful reservations
 uint16_t returns; ///< Total return to starts
 uint16_t lands; ///< Total emergency landings
 uint16_t pack_completed; ///< Total completed packages
 uint16_t pack_failed; ///< Total failed packages
} mavlink_mad_logging_reply_t;

#define MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN 24
#define MAVLINK_MSG_ID_211_LEN 24

#define MAVLINK_MSG_ID_MAD_LOGGING_REPLY_CRC 0
#define MAVLINK_MSG_ID_211_CRC 0



#define MAVLINK_MESSAGE_INFO_MAD_LOGGING_REPLY { \
	"MAD_LOGGING_REPLY", \
	10, \
	{  { "flight_levels", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_mad_logging_reply_t, flight_levels) }, \
         { "distance", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_mad_logging_reply_t, distance) }, \
         { "retries", NULL, MAVLINK_TYPE_UINT16_T, 0, 8, offsetof(mavlink_mad_logging_reply_t, retries) }, \
         { "rounds", NULL, MAVLINK_TYPE_UINT16_T, 0, 10, offsetof(mavlink_mad_logging_reply_t, rounds) }, \
         { "res_failures", NULL, MAVLINK_TYPE_UINT16_T, 0, 12, offsetof(mavlink_mad_logging_reply_t, res_failures) }, \
         { "res_succes", NULL, MAVLINK_TYPE_UINT16_T, 0, 14, offsetof(mavlink_mad_logging_reply_t, res_succes) }, \
         { "returns", NULL, MAVLINK_TYPE_UINT16_T, 0, 16, offsetof(mavlink_mad_logging_reply_t, returns) }, \
         { "lands", NULL, MAVLINK_TYPE_UINT16_T, 0, 18, offsetof(mavlink_mad_logging_reply_t, lands) }, \
         { "pack_completed", NULL, MAVLINK_TYPE_UINT16_T, 0, 20, offsetof(mavlink_mad_logging_reply_t, pack_completed) }, \
         { "pack_failed", NULL, MAVLINK_TYPE_UINT16_T, 0, 22, offsetof(mavlink_mad_logging_reply_t, pack_failed) }, \
         } \
}


/**
 * @brief Pack a mad_logging_reply message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param retries Total reservation retries
 * @param rounds Total reservation rounds
 * @param res_failures Total failed reservations
 * @param res_succes Total succesful reservations
 * @param flight_levels Summed flight levels
 * @param returns Total return to starts
 * @param lands Total emergency landings
 * @param pack_completed Total completed packages
 * @param pack_failed Total failed packages
 * @param distance Distance travelled
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_logging_reply_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint16_t retries, uint16_t rounds, uint16_t res_failures, uint16_t res_succes, float flight_levels, uint16_t returns, uint16_t lands, uint16_t pack_completed, uint16_t pack_failed, float distance)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN];
	_mav_put_float(buf, 0, flight_levels);
	_mav_put_float(buf, 4, distance);
	_mav_put_uint16_t(buf, 8, retries);
	_mav_put_uint16_t(buf, 10, rounds);
	_mav_put_uint16_t(buf, 12, res_failures);
	_mav_put_uint16_t(buf, 14, res_succes);
	_mav_put_uint16_t(buf, 16, returns);
	_mav_put_uint16_t(buf, 18, lands);
	_mav_put_uint16_t(buf, 20, pack_completed);
	_mav_put_uint16_t(buf, 22, pack_failed);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN);
#else
	mavlink_mad_logging_reply_t packet;
	packet.flight_levels = flight_levels;
	packet.distance = distance;
	packet.retries = retries;
	packet.rounds = rounds;
	packet.res_failures = res_failures;
	packet.res_succes = res_succes;
	packet.returns = returns;
	packet.lands = lands;
	packet.pack_completed = pack_completed;
	packet.pack_failed = pack_failed;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_LOGGING_REPLY;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN);
#endif
}

/**
 * @brief Pack a mad_logging_reply message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param retries Total reservation retries
 * @param rounds Total reservation rounds
 * @param res_failures Total failed reservations
 * @param res_succes Total succesful reservations
 * @param flight_levels Summed flight levels
 * @param returns Total return to starts
 * @param lands Total emergency landings
 * @param pack_completed Total completed packages
 * @param pack_failed Total failed packages
 * @param distance Distance travelled
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mad_logging_reply_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint16_t retries,uint16_t rounds,uint16_t res_failures,uint16_t res_succes,float flight_levels,uint16_t returns,uint16_t lands,uint16_t pack_completed,uint16_t pack_failed,float distance)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN];
	_mav_put_float(buf, 0, flight_levels);
	_mav_put_float(buf, 4, distance);
	_mav_put_uint16_t(buf, 8, retries);
	_mav_put_uint16_t(buf, 10, rounds);
	_mav_put_uint16_t(buf, 12, res_failures);
	_mav_put_uint16_t(buf, 14, res_succes);
	_mav_put_uint16_t(buf, 16, returns);
	_mav_put_uint16_t(buf, 18, lands);
	_mav_put_uint16_t(buf, 20, pack_completed);
	_mav_put_uint16_t(buf, 22, pack_failed);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN);
#else
	mavlink_mad_logging_reply_t packet;
	packet.flight_levels = flight_levels;
	packet.distance = distance;
	packet.retries = retries;
	packet.rounds = rounds;
	packet.res_failures = res_failures;
	packet.res_succes = res_succes;
	packet.returns = returns;
	packet.lands = lands;
	packet.pack_completed = pack_completed;
	packet.pack_failed = pack_failed;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAD_LOGGING_REPLY;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN);
#endif
}

/**
 * @brief Encode a mad_logging_reply struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mad_logging_reply C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_logging_reply_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mad_logging_reply_t* mad_logging_reply)
{
	return mavlink_msg_mad_logging_reply_pack(system_id, component_id, msg, mad_logging_reply->retries, mad_logging_reply->rounds, mad_logging_reply->res_failures, mad_logging_reply->res_succes, mad_logging_reply->flight_levels, mad_logging_reply->returns, mad_logging_reply->lands, mad_logging_reply->pack_completed, mad_logging_reply->pack_failed, mad_logging_reply->distance);
}

/**
 * @brief Encode a mad_logging_reply struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param mad_logging_reply C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mad_logging_reply_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_mad_logging_reply_t* mad_logging_reply)
{
	return mavlink_msg_mad_logging_reply_pack_chan(system_id, component_id, chan, msg, mad_logging_reply->retries, mad_logging_reply->rounds, mad_logging_reply->res_failures, mad_logging_reply->res_succes, mad_logging_reply->flight_levels, mad_logging_reply->returns, mad_logging_reply->lands, mad_logging_reply->pack_completed, mad_logging_reply->pack_failed, mad_logging_reply->distance);
}

/**
 * @brief Send a mad_logging_reply message
 * @param chan MAVLink channel to send the message
 *
 * @param retries Total reservation retries
 * @param rounds Total reservation rounds
 * @param res_failures Total failed reservations
 * @param res_succes Total succesful reservations
 * @param flight_levels Summed flight levels
 * @param returns Total return to starts
 * @param lands Total emergency landings
 * @param pack_completed Total completed packages
 * @param pack_failed Total failed packages
 * @param distance Distance travelled
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mad_logging_reply_send(mavlink_channel_t chan, uint16_t retries, uint16_t rounds, uint16_t res_failures, uint16_t res_succes, float flight_levels, uint16_t returns, uint16_t lands, uint16_t pack_completed, uint16_t pack_failed, float distance)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN];
	_mav_put_float(buf, 0, flight_levels);
	_mav_put_float(buf, 4, distance);
	_mav_put_uint16_t(buf, 8, retries);
	_mav_put_uint16_t(buf, 10, rounds);
	_mav_put_uint16_t(buf, 12, res_failures);
	_mav_put_uint16_t(buf, 14, res_succes);
	_mav_put_uint16_t(buf, 16, returns);
	_mav_put_uint16_t(buf, 18, lands);
	_mav_put_uint16_t(buf, 20, pack_completed);
	_mav_put_uint16_t(buf, 22, pack_failed);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_LOGGING_REPLY, buf, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_LOGGING_REPLY, buf, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN);
#endif
#else
	mavlink_mad_logging_reply_t packet;
	packet.flight_levels = flight_levels;
	packet.distance = distance;
	packet.retries = retries;
	packet.rounds = rounds;
	packet.res_failures = res_failures;
	packet.res_succes = res_succes;
	packet.returns = returns;
	packet.lands = lands;
	packet.pack_completed = pack_completed;
	packet.pack_failed = pack_failed;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_LOGGING_REPLY, (const char *)&packet, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_LOGGING_REPLY, (const char *)&packet, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN);
#endif
#endif
}

#if MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_mad_logging_reply_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint16_t retries, uint16_t rounds, uint16_t res_failures, uint16_t res_succes, float flight_levels, uint16_t returns, uint16_t lands, uint16_t pack_completed, uint16_t pack_failed, float distance)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_float(buf, 0, flight_levels);
	_mav_put_float(buf, 4, distance);
	_mav_put_uint16_t(buf, 8, retries);
	_mav_put_uint16_t(buf, 10, rounds);
	_mav_put_uint16_t(buf, 12, res_failures);
	_mav_put_uint16_t(buf, 14, res_succes);
	_mav_put_uint16_t(buf, 16, returns);
	_mav_put_uint16_t(buf, 18, lands);
	_mav_put_uint16_t(buf, 20, pack_completed);
	_mav_put_uint16_t(buf, 22, pack_failed);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_LOGGING_REPLY, buf, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_LOGGING_REPLY, buf, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN);
#endif
#else
	mavlink_mad_logging_reply_t *packet = (mavlink_mad_logging_reply_t *)msgbuf;
	packet->flight_levels = flight_levels;
	packet->distance = distance;
	packet->retries = retries;
	packet->rounds = rounds;
	packet->res_failures = res_failures;
	packet->res_succes = res_succes;
	packet->returns = returns;
	packet->lands = lands;
	packet->pack_completed = pack_completed;
	packet->pack_failed = pack_failed;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_LOGGING_REPLY, (const char *)packet, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAD_LOGGING_REPLY, (const char *)packet, MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN);
#endif
#endif
}
#endif

#endif

// MESSAGE MAD_LOGGING_REPLY UNPACKING


/**
 * @brief Get field retries from mad_logging_reply message
 *
 * @return Total reservation retries
 */
static inline uint16_t mavlink_msg_mad_logging_reply_get_retries(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint16_t(msg,  8);
}

/**
 * @brief Get field rounds from mad_logging_reply message
 *
 * @return Total reservation rounds
 */
static inline uint16_t mavlink_msg_mad_logging_reply_get_rounds(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint16_t(msg,  10);
}

/**
 * @brief Get field res_failures from mad_logging_reply message
 *
 * @return Total failed reservations
 */
static inline uint16_t mavlink_msg_mad_logging_reply_get_res_failures(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint16_t(msg,  12);
}

/**
 * @brief Get field res_succes from mad_logging_reply message
 *
 * @return Total succesful reservations
 */
static inline uint16_t mavlink_msg_mad_logging_reply_get_res_succes(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint16_t(msg,  14);
}

/**
 * @brief Get field flight_levels from mad_logging_reply message
 *
 * @return Summed flight levels
 */
static inline float mavlink_msg_mad_logging_reply_get_flight_levels(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field returns from mad_logging_reply message
 *
 * @return Total return to starts
 */
static inline uint16_t mavlink_msg_mad_logging_reply_get_returns(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint16_t(msg,  16);
}

/**
 * @brief Get field lands from mad_logging_reply message
 *
 * @return Total emergency landings
 */
static inline uint16_t mavlink_msg_mad_logging_reply_get_lands(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint16_t(msg,  18);
}

/**
 * @brief Get field pack_completed from mad_logging_reply message
 *
 * @return Total completed packages
 */
static inline uint16_t mavlink_msg_mad_logging_reply_get_pack_completed(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint16_t(msg,  20);
}

/**
 * @brief Get field pack_failed from mad_logging_reply message
 *
 * @return Total failed packages
 */
static inline uint16_t mavlink_msg_mad_logging_reply_get_pack_failed(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint16_t(msg,  22);
}

/**
 * @brief Get field distance from mad_logging_reply message
 *
 * @return Distance travelled
 */
static inline float mavlink_msg_mad_logging_reply_get_distance(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Decode a mad_logging_reply message into a struct
 *
 * @param msg The message to decode
 * @param mad_logging_reply C-struct to decode the message contents into
 */
static inline void mavlink_msg_mad_logging_reply_decode(const mavlink_message_t* msg, mavlink_mad_logging_reply_t* mad_logging_reply)
{
#if MAVLINK_NEED_BYTE_SWAP
	mad_logging_reply->flight_levels = mavlink_msg_mad_logging_reply_get_flight_levels(msg);
	mad_logging_reply->distance = mavlink_msg_mad_logging_reply_get_distance(msg);
	mad_logging_reply->retries = mavlink_msg_mad_logging_reply_get_retries(msg);
	mad_logging_reply->rounds = mavlink_msg_mad_logging_reply_get_rounds(msg);
	mad_logging_reply->res_failures = mavlink_msg_mad_logging_reply_get_res_failures(msg);
	mad_logging_reply->res_succes = mavlink_msg_mad_logging_reply_get_res_succes(msg);
	mad_logging_reply->returns = mavlink_msg_mad_logging_reply_get_returns(msg);
	mad_logging_reply->lands = mavlink_msg_mad_logging_reply_get_lands(msg);
	mad_logging_reply->pack_completed = mavlink_msg_mad_logging_reply_get_pack_completed(msg);
	mad_logging_reply->pack_failed = mavlink_msg_mad_logging_reply_get_pack_failed(msg);
#else
	memcpy(mad_logging_reply, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_MAD_LOGGING_REPLY_LEN);
#endif
}
