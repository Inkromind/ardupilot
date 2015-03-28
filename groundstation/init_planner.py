from droneapi.lib import VehicleMode
from pymavlink import mavutil

# First get an instance of the API endpoint
api = local_connect()
# get our vehicle - when running with mavproxy it only knows about one vehicle (for now)
v = api.get_vehicles()[0]

msg = v.message_factory.mad_init_planner_encode(0, 0)

print "Initializing MAD Planner"
v.send_mavlink(msg)
