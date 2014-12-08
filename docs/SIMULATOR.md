# SITL Instructions

## GIT Repositories

* ardupilot:
  * Develop branch: [https://github.com/Inkromind/ardupilot/tree/develop](https://github.com/Inkromind/ardupilot/tree/develop)
    * Based of stable releases of [https://github.com/diydrones/ardupilot](https://github.com/diydrones/ardupilot)
  * simpleExtension-UNSTABLE branch: [https://github.com/Inkromind/ardupilot/tree/simpleExtension-UNSTABLE](https://github.com/Inkromind/ardupilot/tree/simpleExtension-UNSTABLE)
* PX4Firmware:
  * ArduCopter-3.2 branch: [https://github.com/Inkromind/PX4Firmware/tree/ArduCopter-3.2](https://github.com/Inkromind/PX4Firmware/tree/ArduCopter-3.2)
    * Based of stable releases of [https://github.com/diydrones/PX4Firmware/tree/ArduCopter-3.2](https://github.com/diydrones/PX4Firmware/tree/ArduCopter-3.2)
* PX4NuttX:
  * ArduCopter-3.2 branch: [https://github.com/Inkromind/PX4NuttX/tree/ArduCopter-3.2](https://github.com/Inkromind/PX4NuttX/tree/ArduCopter-3.2)
    * Based of stable releases of [https://github.com/diydrones/PX4NuttX](https://github.com/diydrones/PX4NuttX)

## Installing VM

### Premade VM
The premade VM already has the simulator and dependencies installed.

User: ardupilot

Password: password

### Other VM
To install the simulator on the vm, clone the [develop branch of my repository](https://github.com/Inkromind/ardupilot/tree/develop) or
the [simpleExtension-UNSTABLE branch of my repository](https://github.com/Inkromind/ardupilot/tree/simpleExtension-UNSTABLE) into the home directory
of the current user. This should result in the directory ~/ardupilot containing the source.

Next, run the installar located at ```~/ardupilot/sitl_installer.sh```.
This will install all required dependencies and repositories for the the simulator.

## Building the source
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
