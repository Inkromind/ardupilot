// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

static uint8_t mas_test_flight_state = 0;

static AP_Mission::Mission_Command cmd = {};

static uint8_t mas_test_flight_start()
{
    mas_test_flight_state = 0;
    mission.stop();
    mission.clear();
    gcs_send_text_P(SEVERITY_HIGH,PSTR("Starting test flight."));
    if (set_mode(AUTO))
    {
        gcs_send_text_P(SEVERITY_HIGH,PSTR("Auto mode started."));
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

static void mas_loiter(int8_t sec) {
    auto_loiter_start();
    cmd.id = MAV_CMD_CONDITION_DELAY;
    cmd.content.delay.seconds = sec;
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
    if (mas_test_flight_state > 0) {
        if (control_mode != AUTO) {
            gcs_send_text_P(SEVERITY_HIGH,PSTR("No longer in AUTO flightmode. Cancelling mission."));
            mas_test_flight_state = 0;
            return;
        }
    }
    switch (mas_test_flight_state) {
        case 1:
            mas_loiter(10);
            hal.console->printf_P(PSTR("started delay command\n"));
            gcs_send_text_P(SEVERITY_HIGH,PSTR("Started delay command"));
            mas_test_flight_state = 2;
            break;
        case 2:
            if (verify_command(cmd)) {
                gcs_send_text_P(SEVERITY_HIGH,PSTR("Delay command completed. Arming engines..."));
                init_arm_motors();
                mas_test_flight_state = 3;
            }
            break;
        case 3:
            if (motors.armed()) {
                gcs_send_text_P(SEVERITY_HIGH,PSTR("Engines armed, taking off to 20m..."));
                set_auto_armed(true);
                mas_test_flight_state = 4;

                mas_takeoff(20);
            }
            break;
        case 4:
            if (verify_command(cmd)) {
                gcs_send_text_P(SEVERITY_HIGH,PSTR("Takeoff completed. Holding position for 20s..."));
                mas_test_flight_state = 5;

                mas_loiter(20);
            }
            break;
        case 5:
            if (verify_command(cmd)) {
                gcs_send_text_P(SEVERITY_HIGH,PSTR("Moving forward 50m"));
                mas_test_flight_state = 6;
                mas_forward(50);
            }
            break;
        case 6:
            if (verify_command(cmd)) {
                gcs_send_text_P(SEVERITY_HIGH,PSTR("Moving right 50m"));
                mas_test_flight_state = 7;
                mas_right(50);
            }
            break;
        case 7:
            if (verify_command(cmd)) {
                gcs_send_text_P(SEVERITY_HIGH,PSTR("Descending 10m"));
                mas_test_flight_state = 8;
                mas_down(10);
            }
            break;
        case 8:
            if (verify_command(cmd)) {
                gcs_send_text_P(SEVERITY_HIGH,PSTR("Moving back 50m"));
                mas_test_flight_state = 9;
                mas_back(50);
            }
            break;
        case 9:
            if (verify_command(cmd)) {
                gcs_send_text_P(SEVERITY_HIGH,PSTR("Ascending 35m"));
                mas_test_flight_state = 10;
                mas_up(35);
            }
            break;
        case 10:
            if (verify_command(cmd)) {
                gcs_send_text_P(SEVERITY_HIGH,PSTR("Moving left 50m"));
                mas_test_flight_state = 11;
                mas_left(50);
            }
            break;
        case 11:
            if (verify_command(cmd)) {
                gcs_send_text_P(SEVERITY_HIGH,PSTR("Starting landing."));
                mas_test_flight_state = 12;
                mas_land();
            }
            break;
        case 12:
            if (verify_command(cmd)) {
                gcs_send_text_P(SEVERITY_HIGH,PSTR("Landed, test flight completed"));
                mas_test_flight_state = 0;
            }
            break;
        default:
            break;
    }
}


