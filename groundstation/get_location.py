from droneapi.lib import VehicleMode
from pymavlink import mavutil
import sys

def mavrx_debug_handler(message):
    """A demo of receiving raw mavlink messages"""
    print "Received", message

# First get an instance of the API endpoint
api = local_connect()
# get our vehicle - when running with mavproxy it only knows about one vehicle (for now)
v = api.get_vehicles()[0]

msg = v.message_factory.mad_get_current_location_encode(0, 0)

print "Getting Location"

v.send_mavlink(msg)