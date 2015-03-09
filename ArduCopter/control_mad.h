/*
 * control_mad.h
 *
 *  Created on: 9-mrt.-2015
 *      Author: Arne
 */

#ifndef CONTROL_MAD_H_
#define CONTROL_MAD_H_

static bool mad_takeoff_start(float final_alt);

static bool mad_land_start();

static bool mad_loiter_start();

static bool mad_nav_start(const Vector3f& destination);

static bool mad_arm_motors();

#endif /* CONTROL_MAD_H_ */
