/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

/*
 * control_mad.pde - init and run calls for multi agent drone flight mode
 */

/*
 *  While in the auto flight mode, navigation or do/now commands can be run.
 *  Code in this file implements the navigation commands
 */

#include "control_mad.h"

static bool mad_arming = false;

static void mad_loiter_run();
static void mad_takeoff_run();
static void mad_land_run();
static void mad_nav_run();
static void mad_nav_altitude_run();

static bool mad_init(bool ignore_checks)
{
    if (GPS_ok() || ignore_checks) {
        set_auto_armed(true);

        poshold_init(ignore_checks);
        mad_mode = Mad_Loiter;
        mission.stop();
        wp_nav.wp_and_spline_init();

        gcs_send_text_P(SEVERITY_LOW, PSTR("MAD Flight mode enabled"));

        return true;
    }
    return false;
}

// mad_run - runs the auto controller
//      should be called at 100hz or more
//      relies on run_autopilot being called at 10hz which handles decision making and non-navigation related commands
static void mad_run()
{
    // call the correct auto controller
    switch (mad_mode) {

    case Mad_Loiter:
        mad_loiter_run();
        break;

    case Mad_TakeOff:
        mad_takeoff_run();
        break;

    case Mad_Land:
        mad_land_run();
        break;

    case Mad_Nav:
        mad_nav_run();
        break;
    case Mad_Nav_Altitude:
        mad_nav_altitude_run();
        break;
    }
}

// mad_takeoff_start - initialises waypoint controller to implement take-off
// based on auto_takeoff_start
static bool mad_takeoff_start(float final_alt)
{

    if (mad_mode == Mad_TakeOff) {
        Vector3f destination = wp_nav.get_wp_destination();
        if (destination.z == final_alt)
            return true;
    }

    mad_mode = Mad_TakeOff;

    // initialise wpnav destination
    Vector3f target_pos = inertial_nav.get_position();
    target_pos.z = final_alt;
    wp_nav.set_wp_destination(target_pos);

    // initialise yaw
    set_auto_yaw_mode(AUTO_YAW_HOLD);

    if (!motors.armed()) {
        if (!mad_arm_motors())
            return false;
    }

    // tell motors to do a slow start
    motors.slow_start(true);

    return true;
}

// mad_takeoff_run - takeoff in mad mode
//      called by mad_run at 100hz or more
// based on guided_takeoff_run
static void mad_takeoff_run()
{
    if (!motors.armed())
        return;

    // process pilot's yaw input
    float target_yaw_rate = 0;
    if (!failsafe.radio) {
        // get pilot's desired yaw rate
        target_yaw_rate = get_pilot_desired_yaw_rate(g.rc_4.control_in);
    }

    // run waypoint controller
    wp_nav.update_wpnav();

    // call z-axis position controller (wpnav should have already updated it's alt target)
    pos_control.update_z_controller();

    // roll & pitch from waypoint controller, yaw rate from pilot
    attitude_control.angle_ef_roll_pitch_rate_ef_yaw(wp_nav.get_roll(), wp_nav.get_pitch(), target_yaw_rate);
}

// mad_nav_start - initialises waypoint controller to implement flying to a particular destination
static bool mad_nav_start(const Vector3f& destination)
{
    if (ap.land_complete || !motors.armed())
        return false;

    if (mad_mode == Mad_Nav) {
        Vector3f currentDestination = wp_nav.get_wp_destination();
        if (currentDestination == destination)
            return true;
    }

    mad_mode = Mad_Nav;

    // initialise wpnav
    wp_nav.set_wp_destination(destination);

    // initialise yaw
    // To-Do: reset the yaw only when the previous navigation command is not a WP.  this would allow removing the special check for ROI
    if (auto_yaw_mode != AUTO_YAW_ROI) {
        set_auto_yaw_mode(get_default_auto_yaw_mode(false));
    }

    return true;


}

// mad_nav_run - runs the mad nav controller
//      called by mad_run at 100hz or more
// based on auto_nav_run
static void mad_nav_run()
{
    if (ap.land_complete)
        return;

    // process pilot's yaw input
    float target_yaw_rate = 0;
    if (!failsafe.radio) {
        // get pilot's desired yaw rate
        target_yaw_rate = get_pilot_desired_yaw_rate(g.rc_4.control_in);
        if (target_yaw_rate != 0) {
            set_auto_yaw_mode(AUTO_YAW_HOLD);
        }
    }

    // run waypoint controller
    wp_nav.update_wpnav();

    // call z-axis position controller (wpnav should have already updated it's alt target)
    pos_control.update_z_controller();

    // call attitude controller
    if (auto_yaw_mode == AUTO_YAW_HOLD) {
        // roll & pitch from waypoint controller, yaw rate from pilot
        attitude_control.angle_ef_roll_pitch_rate_ef_yaw(wp_nav.get_roll(), wp_nav.get_pitch(), target_yaw_rate);
    }else{
        // roll, pitch from waypoint controller, yaw heading from auto_heading()
        attitude_control.angle_ef_roll_pitch_yaw(wp_nav.get_roll(), wp_nav.get_pitch(), get_auto_heading(),true);
    }

}

static bool mad_nav_altitude_start(float targetAltitude)
{
    if (ap.land_complete || !motors.armed())
        return false;

    if (mad_mode == Mad_Nav_Altitude) {
        Vector3f destination = wp_nav.get_wp_destination();
        if (destination.z == targetAltitude)
            return true;
    }

    Vector3f target_pos = inertial_nav.get_position();
    target_pos.z = targetAltitude;
    mad_mode = Mad_Nav_Altitude;

    // initialise wpnav
    wp_nav.set_wp_destination(target_pos);

    // initialise yaw
    // To-Do: reset the yaw only when the previous navigation command is not a WP.  this would allow removing the special check for ROI
    if (auto_yaw_mode != AUTO_YAW_ROI) {
        set_auto_yaw_mode(get_default_auto_yaw_mode(false));
    }

    return true;

}

static void mad_nav_altitude_run() {

    if (ap.land_complete)
            return;

    // process pilot's yaw input
    float target_yaw_rate = 0;
    if (!failsafe.radio) {
        // get pilot's desired yaw rate
        target_yaw_rate = get_pilot_desired_yaw_rate(g.rc_4.control_in);
    }

    // run waypoint controller
    wp_nav.update_wpnav();

    // call z-axis position controller (wpnav should have already updated it's alt target)
    pos_control.update_z_controller();

    // roll & pitch from waypoint controller, yaw rate from pilot
    attitude_control.angle_ef_roll_pitch_rate_ef_yaw(wp_nav.get_roll(), wp_nav.get_pitch(), target_yaw_rate);
}



// mad_land_start - initialises controller to implement a landing
static bool mad_land_start()
{
    if (mad_mode == Mad_Land)
        return true;
    if (ap.land_complete)
        return true;
    mad_mode = Mad_Land;
    return land_init(false);

}

// mad_land_run - lands in auto mode
//      called by mad_run at 100hz or more
static void mad_land_run()
{
    land_run();
}


static bool mad_loiter_start() {
    if (mad_mode == Mad_Loiter)
        return true;
    if (ap.land_complete)
        return false;
    if (poshold_init(false)) {
        mad_mode = Mad_Loiter;
        return true;
    }
    return false;
}

static void mad_loiter_run() {
    poshold_run();
}

static bool mad_arm_motors() {
    if (mad_arming)
        return false;

    if (motors.armed())
        return true;

    mad_arming = true;
    pre_arm_checks(true);
    if (ap.pre_arm_check && arm_checks(true)) {
        if (init_arm_motors()) {
            set_auto_armed(true);
            mad_arming = false;
            return true;
        } else {
            mad_arming = false;
            return false;
        }
    } else{
        mad_arming = false;
        return false;
    }
}

static bool mad_disarm_motors() {
    if (mad_arming)
        return false;

    if (!motors.armed())
        return true;

    if (!ap.land_complete)
        return false;

    mad_arming = true;
    init_disarm_motors();
    mad_arming = false;
    return true;
}
