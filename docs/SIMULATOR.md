# SITL Instructions

## VM
User: ardupilot

Password: password

## Building
The source code is located in ~/ardupilut and is linked to [my repository, develop branch](https://github.com/Inkromind/ardupilot/tree/develop).
To get the latest code, simply open a terminal in this directory and pull.

To build the code, go to ~/ardupilot/ArduCopter and type

```
make px4-v2
```

This can take several minutes depending on whether it also has to rebuild the PX4Firmware.

If the code does not compile or something goes wrong during execution,
clean the PX4-firmware before building:

```
make px4-clean
```

## Running the simulator
When the code is compiled, run ```~/copter.sh```. Once it says
__STABILIZE> Received 434 parameters__
(number can be different) the simulator has fully start up with a console and a (broken) map.
To view the actual map, in the map window go to View > Service and select __GoogleMap__.

Next, open a new terminal window and go to ~/ardupilot/groundstation.
Start up MAVProxy to be able to communicate with the drone from outside the simulator:

```
mavproxy.py --master=127.0.0.1:14551
```

Optionally, you can include --console and/or --map to get similar windows as in the simulator.

To start the test flight, type the following inside the MAVProxy terminal:

```
rc 3 1600
api start start_test_flight.py
```

The script will ask some input to show its ability to read additional input and then send
a MAVLink message with ID 50001 to the drone which will start the test flight.

You can see the output in the console and the map window in the simulator.
