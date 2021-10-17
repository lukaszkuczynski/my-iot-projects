import serial
import argparse
import time

parser = argparse.ArgumentParser()
parser.add_argument("--port", required=True)
parser.add_argument("--baudrate", default=115200)
args = parser.parse_args()
print(args)


ser = serial.Serial(
        port=args.port, 
        baudrate = args.baudrate,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1
)
counter=0
on_off_state=True

while 1:
    on_off_state = not on_off_state
    command="led="+("on" if on_off_state else "off")
    # command="ledstatus"
    command_encoded = (command+"\n").encode("ascii")
    ser.write(command_encoded)
    x=ser.readline()
    time.sleep(1)
    print(x)
