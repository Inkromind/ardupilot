#!/bin/bash

sudo apt-get update
sudo apt-get install -y dos2unix git-core automake libexpat1-dev
cd
chmod +x ardupilot/Tools/scripts/install-prereqs-ubuntu.sh
ardupilot/Tools/scripts/install-prereqs-ubuntu.sh -y

cd
exportline="export PATH=/usr/lib/ccache:$PATH";
if ! grep -Fxq "$exportline" ~/.bashrc ; then
    echo $exportline >> ~/.bashrc
    $exportline
    . ~/.profile
fi

./ardupilot/update_px4firmware.sh

cd ~/ardupilot/ArduCopter
make configure