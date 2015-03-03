#!/bin/bash

sudo apt-get update
sudo apt-get install -y dos2unix git-core automake libexpat1-dev
cd
chmod +x ardupilot/Tools/scripts/install-prereqs-ubuntu.sh
ardupilot/Tools/scripts/install-prereqs-ubuntu.sh -y

cd
echo 'export PATH=/usr/lib/ccache:$PATH' >>~/.bashrc
export PATH=/usr/lib/ccache:$PATH
. ~/.profile

cd ~/ardupilot/ArduCopter
make configure
make

read -p "ArduPilot installation complete."