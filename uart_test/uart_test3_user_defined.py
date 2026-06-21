import serial

ser = serial.Serial('/dev/ttyS8', 115200, timeout=1)

DATAHEAD    = b'\xAA'
DATATRAIL   = b'\x55'
WRITEFLAG   = b'\x01'
READFLAG    = b'\x00'

def send_data(data):
    checksum = 0
    for byte in data:
        checksum ^= byte

    data_length = len(data).to_bytes(1, 'big')
    packet = DATAHEAD + WRITEFLAG + data_length + data+ bytes([checksum]) + DATATRAIL
    ser.write(packet)
    print(f"data: {data}")
    print(f"checksum: 0x{checksum:02X}")
    print(f"packet: {packet.hex()}")

send_data(b'123')

ser.close()