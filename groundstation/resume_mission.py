from pymavlink import mavutil

# First get an instance of the API endpoint
api = local_connect()
# get our vehicle - when running with mavproxy it only knows about one vehicle (for now)
v = api.get_vehicles()[0]

msg = v.message_factory.mad_resume_mission_encode(0, 0)

print "Resuming MAD Mission"
v.send_mavlink(msg)
