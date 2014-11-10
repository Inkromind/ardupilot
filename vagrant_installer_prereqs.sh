#!/bin/bash


sudo apt-get update && apt-get dist-upgrade
sudo apt-get install -y dos2unix g++-4.7 automake libexpat1-dev
cd
chmod +x ardupilot/Tools/scripts/install-prereqs-ubuntu.sh
ardupilot/Tools/scripts/install-prereqs-ubuntu.sh

echo 'export PATH=/usr/lib/ccache:$PATH' >>~/.bashrc
export PATH=/usr/lib/ccache:$PATH
. ~/.profile

cd ~/ardupilot/ArduCopter
make configure
make 

read -p "Vagrant installation complete."