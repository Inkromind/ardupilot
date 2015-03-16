#!/bin/bash

read -p "Using VirtualBox Shared Folders? (y/n)" RESP
if [[ $REPLY =~ ^[Yy]$ ]]; then
    ARDUPILOT_DIR="ardupilot"
else
    ARDUPILOT_DIR="sf_ardupilot"
fi


sudo apt-get update
sudo apt-get install -y dos2unix g++-4.7 git-core automake libexpat1-dev
cd
chmod +x $ARDUPILOT_DIR/Tools/scripts/install-prereqs-ubuntu.sh
$ARDUPILOT_DIR/Tools/scripts/install-prereqs-ubuntu.sh -y

./$ARDUPILOT_DIR/update_px4firmware.sh

echo "module load droneapi.module.api" >> ~/.mavinit.scr
git clone git://github.com/tridge/jsbsim.git


cd
cd jsbsim
./autogen.sh
make
echo 'export PATH=$PATH:$HOME/$ARDUPILOT_DIR/Tools/autotest' >>~/.bashrc
echo 'export PATH=$PATH:/usr/local/lib/python2.7/dist-packages/MavProxy' >>~/.bashrc
echo 'export PATH=$PATH:/usr/local/lib/python2.7/dist-packages/pymavlink/examples' >>~/.bashrc
echo 'export PATH=$PATH:$HOME/jsbsim/src' >>~/.bashrc
echo 'export PATH=/usr/lib/ccache:$PATH' >>~/.bashrc
export PATH=$PATH:$HOME/$ARDUPILOT_DIR/Tools/autotest
export PATH=$PATH:/usr/local/lib/python2.7/dist-packages/MavProxy
export PATH=$PATH:/usr/local/lib/python2.7/dist-packages/pymavlink/examples
export PATH=$PATH:$HOME/jsbsim/src
export PATH=/usr/lib/ccache:$PATH
. ~/.profile

echo 'ARMING_CHECK 0' >> ~/$ARDUPILOT_DIR/Tools/autotest/copter_params.parm
cd
echo '#! /bin/bash' >>copter.sh
echo 'read -p "Do you want to reset parameters to default? [Do this first run](y/n) " RESP' >>copter.sh
echo 'if [ "$RESP" = "y" ]; then' >>copter.sh
echo 'cd ~/$ARDUPILOT_DIR/ArduCopter' >>copter.sh
echo 'sim_vehicle.sh -v ArduCopter -f quad  -j 4 -w --console --map --aircraft test' >>copter.sh
echo 'else' >>copter.sh
echo 'cd ~/$ARDUPILOT_DIR/ArduCopter' >>copter.sh
echo 'sim_vehicle.sh -v ArduCopter -f quad  -j 4 --console --map --aircraft test' >>copter.sh
echo 'fi'  >>copter.sh
chmod +x copter.sh

cd ~/$ARDUPILOT_DIR/ArduCopter
make configure
make

read -p "SITL installation complete. Now restart Ubuntu, then run ./copter.sh from a terminal"