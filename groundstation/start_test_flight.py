from droneapi.lib import VehicleMode
from pymavlink import mavutil

def mavrx_debug_handler(message):
    """A demo of receiving raw mavlink messages"""
    print "Received", message

# First get an instance of the API endpoint
api = local_connect()
# get our vehicle - when running with mavproxy it only knows about one vehicle (for now)
v = api.get_vehicles()[0]

# Test custom commands
# Note: For mavlink messages that include a target_system & target_component, those values
# can just be filled with zero.  The API will take care of using the correct values
# For instance, from the xml for command_long:
#                Send a command with up to seven parameters to the MAV
#
#                target_system             : System which should execute the command (uint8_t)
#                target_component          : Component which should execute the command, 0 for all components (uint8_t)
#                command                   : Command ID, as defined by MAV_CMD enum. (uint16_t)
#                confirmation              : 0: First transmission of this command. 1-255: Confirmation transmissions (e.g. for kill command) (uint8_t)
#                param1                    : Parameter 1, as defined by MAV_CMD enum. (float)
#                param2                    : Parameter 2, as defined by MAV_CMD enum. (float)
#                param3                    : Parameter 3, as defined by MAV_CMD enum. (float)
#                param4                    : Parameter 4, as defined by MAV_CMD enum. (float)
#                param5                    : Parameter 5, as defined by MAV_CMD enum. (float)
#                param6                    : Parameter 6, as defined by MAV_CMD enum. (float)
#                param7                    : Parameter 7, as defined by MAV_CMD enum. (float)
#msg = v.message_factory.command_long_encode(0, 0,
#                                  mavutil.mavlink.MAV_CMD_CONDITION_YAW, 0,
#                                  0, 0, 0, 0, 1, 0, 0)
#print "Created msg: %s" % msg
#v.send_mavlink(msg)

msg = v.message_factory.mad_init_planner_encode(0, 0)

print "Initializing MAD Planner"
v.send_mavlink(msg)
