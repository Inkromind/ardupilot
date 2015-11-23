set /p "comPort=Enter COM port: COM"
set /p "outPort=Enter out port: "
python C:\Python27\Lib\site-packages\MAVProxy\mavproxy.py --master=com%comPort% --baudrate=57600 --out=127.0.0.1:%outPort% --load-module=mad --load-module=madrelay --load-module=droneapi.module.api