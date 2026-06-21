import serial
import time

ser = serial.Serial('/dev/ttyS8', 115200, timeout=1)

def send_data(data,max_retries=3,timeout=1):

    for attempt in range(max_retries):
        ser.write(data)
        print(f"send data: {data}, test retries: {attempt + 1}")

        response = ser.readline()

        if response:
            print(f"Received response: {response}")
            return True
        else:
            print("No response, retrying...")
            time.sleep(0.1) 

    print("Failed to receive response after maximum retries.")
    return False

data_to_send = b'Hello, UART!'
send_data(data_to_send)
ser.close()