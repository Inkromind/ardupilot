# SITL Instructions

## GIT Repositories

* ardupilot:
  * Develop branch: [https://github.com/Inkromind/ardupilot/tree/develop](https://github.com/Inkromind/ardupilot/tree/develop)
    * Based of stable releases of [https://github.com/diydrones/ardupilot](https://github.com/diydrones/ardupilot)
  * simpleExtension-UNSTABLE branch: [https://github.com/Inkromind/ardupilot/tree/simpleExtension-UNSTABLE](https://github.com/Inkromind/ardupilot/tree/simpleExtension-UNSTABLE)
* PX4Firmware:
  * Stable releases of [https://github.com/diydrones/PX4Firmware/tree/ArduCopter-3.2](https://github.com/diydrones/PX4Firmware/tree/ArduCopter-3.2)
* PX4NuttX:
  * Stable releases of [https://github.com/diydrones/PX4NuttX](https://github.com/diydrones/PX4NuttX)

## Installing VM

### Premade VM (With SITL)
The premade VM already has the simulator, repositories and dependencies installed.
The following settings were used for the VM:

* User: ardupilot
* Password: password
* 64-bit ubuntu
* 4 CPU's
* 3072MB RAM
* 20GB HDD

### Vagrant (Without SITL)
The included vagrantfile will download and install all the required packages and repositories. This VM does not have the simulator installed and can only be used to compile the source.

### Other VM (With SITL)
When setting up your own vm, try to match the settings of the premade vm above as close as possible.

To install the simulator on the vm, clone the [develop branch of my repository](https://github.com/Inkromind/ardupilot/tree/develop) or
the [simpleExtension-UNSTABLE branch of my repository](https://github.com/Inkromind/ardupilot/tree/simpleExtension-UNSTABLE) into the home directory
of the current user. This should result in the directory ~/ardupilot containing the source.

Next, run the installar located at ```~/ardupilot/sitl_installer.sh```.
This will install all required dependencies and repositories for the the simulator.

__If you set less than 4 CPU's for the vm, you will have to edit ```~/copter.sh```:__

Change ```-j 4``` in both mavlink commands with ```-j X``` where X represents the number of CPU's
you assigned to the vm.
This setting tells the simulator how many CPU's it should use to compile.

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

## Updating PX4Firmware
When a new version of the PX4Firmware is needed, run ```~/ardupilot/update_px4firmware.sh``` from within the VM. This will switch the repositories to the required versions.

## Running the simulator (Not in Vagrant)
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
