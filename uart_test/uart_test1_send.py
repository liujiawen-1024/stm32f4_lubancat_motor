import serial

DATA = b'gpio_test1'

ser = serial.Serial('/dev/ttyS8', 115200, timeout=1)

def send_data(data):
    ser.write(data)

send_data(DATA)
for i in range(len(DATA)):
    print(chr(DATA[i]))
ser.close()