from pymavlink import mavutil

# First get an instance of the API endpoint
api = local_connect()
# get our vehicle - when running with mavproxy it only knows about one vehicle (for now)
v = api.get_vehicles()[0]

print('-- New Package --')
id = int(raw_input('Package ID: '))
pickupX = int(raw_input('Pickup Location - X Coordinate: '))
pickupY = int(raw_input('Pickup Location - Y Coordinate: '))
deliveryX = int(raw_input('Delivery Location - X Coordinate: '))
deliveryY = int(raw_input('Delivery Location - Y Coordinate: '))

msg = v.message_factory.mad_request_pacakge_encode(id, pickupX, pickupX, deliveryX, deliveryY)

print("Requesting Package #{} to be moved from <{},{}> to <{},{}>".format(id, pickupX, pickupX, deliveryX, deliveryY))
v.send_mavlink(msg)