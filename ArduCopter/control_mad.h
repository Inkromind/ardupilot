/*
 * control_mad.h
 *
 *  Created on: 9-mrt.-2015
 *      Author: Arne
 */

#ifndef CONTROL_MAD_H_
#define CONTROL_MAD_H_

static bool mad_takeoff_start(float final_alt);

static bool mad_land_start(void);

static bool mad_loiter_start(bool ignore_checks);

static bool mad_nav_start(const Vector3f& destination);

static bool mad_nav_altitude_start(float target_altitude);

static bool mad_arm_motors(void);

static bool mad_disarm_motors(void);

enum MadMode {
    Mad_Loiter,
    Mad_TakeOff,
    Mad_Land,
    Mad_Nav,
    Mad_Nav_Altitude,
};

static MadMode mad_mode;

#endif /* CONTROL_MAD_H_ */
