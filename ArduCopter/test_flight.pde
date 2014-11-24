// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

static uint8_t mas_test_flight_state = 0;

static AP_Mission::Mission_Command cmd = {};

static uint8_t mas_test_flight_start()
{
    mas_test_flight_state = 0;
    mission.stop();
    mission.clear();
    if (set_mode(AUTO))
    {
        mas_test_flight_state = 1;
        return MAV_RESULT_ACCEPTED;
    }
    return MAV_RESULT_FAILED;
}

static Vector3f mas_get_target(int8_t x, int8_t y, int8_t z)
{
    Vector3f target_pos = inertial_nav.get_position();
    target_pos.x += x*100;
    target_pos.y += y*100;
    target_pos.z += z*100;

    return target_pos;
}

static Location mas_vector_to_location(Vector3f vec)
{
    const struct Location &temp_home = ahrs.get_home();
    Location loc = {};
    loc.lat = (vec.x / LATLON_TO_CM) + temp_home.lat;
    loc.lng = (vec.y / LATLON_TO_CM) + temp_home.lng;
    loc.alt = vec.z;
    return loc;
}

static void mas_do_nav(Location loc) {
    cmd.id = MAV_CMD_NAV_WAYPOINT;
    cmd.content.location = loc;
    start_command(cmd);
}

static void mas_forward(int8_t x) {
    mas_do_nav(mas_vector_to_location(mas_get_target(x, 0, 0)));
}

static void mas_right(int8_t y) {
    mas_do_nav(mas_vector_to_location(mas_get_target(0, y, 0)));
}

static void mas_left(int8_t y) {
    mas_do_nav(mas_vector_to_location(mas_get_target(0, -y, 0)));
}

static void mas_back(int8_t x) {
    mas_do_nav(mas_vector_to_location(mas_get_target(-x, 0, 0)));
}

static void mas_up(int8_t z) {
    mas_do_nav(mas_vector_to_location(mas_get_target(0, 0, z)));
}

static void mas_down(int8_t z) {
    mas_do_nav(mas_vector_to_location(mas_get_target(0, 0, -z)));
}

static void mas_takeoff(int8_t alt) {
    cmd.id = MAV_CMD_NAV_TAKEOFF;
    cmd.content.location.alt = alt*100;
    start_command(cmd);
}

static void mas_land() {
    cmd.id = MAV_CMD_NAV_LAND;
    start_command(cmd);
}





static void mas_test_flight_loop()
{
    switch (mas_test_flight_state) {
        case 1:
            cmd.id = MAV_CMD_CONDITION_DELAY;
            cmd.content.delay.seconds = 20;
            start_command(cmd);
            hal.console->printf_P(PSTR("started delay command\n"));
            mas_test_flight_state = 2;
            break;
        case 2:
            if (verify_command(cmd)) {
                init_arm_motors();
                mas_test_flight_state = 3;
                hal.console->printf_P(PSTR("delay command completed. Taking off to 2m...\n"));
                mas_takeoff(2);
            }
            break;
        case 3:
            if (verify_command(cmd)) {
                mas_test_flight_state = 4;
                hal.console->printf_P(PSTR("Reached target altitude... Moving forward 2m\n"));
                mas_forward(2);
            }
            break;
        case 4:
            if (verify_command(cmd)) {
                mas_test_flight_state = 5;
                hal.console->printf_P(PSTR("Reached location 1... Moving right 1m\n"));
                mas_right(1);
            }
            break;
        case 5:
            if (verify_command(cmd)) {
                mas_test_flight_state = 6;
                hal.console->printf_P(PSTR("Reached location 2... Descending 1m\n"));
                mas_down(1);
            }
            break;
        case 6:
            if (verify_command(cmd)) {
                mas_test_flight_state = 7;
                hal.console->printf_P(PSTR("Reached target altitude 2... Moving back 2m\n"));
                mas_back(2);
            }
            break;
        case 7:
            if (verify_command(cmd)) {
                mas_test_flight_state = 8;
                hal.console->printf_P(PSTR("Reached location 3... Ascending 1m\n"));
                mas_up(1);
            }
            break;
        case 8:
            if (verify_command(cmd)) {
                mas_test_flight_state = 9;
                hal.console->printf_P(PSTR("Reached target altitude 3... Moving left 1m\n"));
                mas_left(1);
            }
            break;
        case 9:
            if (verify_command(cmd)) {
                mas_test_flight_state = 10;
                hal.console->printf_P(PSTR("Reached start location... Landing\n"));
                mas_land();
            }
            break;
        case 10:
            if (verify_command(cmd)) {
                mas_test_flight_state = 11;
                hal.console->printf_P(PSTR("Landed. Test flight complete\n"));
            }
            break;
        default:
            break;
    }
}


