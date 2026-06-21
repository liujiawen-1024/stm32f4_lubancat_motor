import serial

ser = serial.Serial('/dev/ttyS8', 115200)

def remove_data():
    data = ser.readline()
    for i in range(len(data)):  
        print(chr(data[i]))

remove_data()
ser.close()