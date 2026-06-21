import paho.mqtt.client as mqtt

MQTT_PRODUCT_ID = "RJpPAS792o"
MQTT_PRODUCT_NAME = "Motor_Ctrl"
MQTT_PRODUCT_TOKEN = "version=2018-10-31&res=products%2FRJpPAS792o%2Fdevices%2FMotor_Ctrl&et=1939802874&method=md5&sign=g5TP2NJAqgrt%2BPIhMJqJgw%3D%3D"

MQTT_BROKER = f"mqtts.heclouds.com"
MQTT_PORT = 1883    

CLIENT_ID = f"{MQTT_PRODUCT_NAME}"
USERNAME = f"{MQTT_PRODUCT_ID}"

client = mqtt.Client(client_id=CLIENT_ID,clean_session=True)
client.username_pw_set(USERNAME,MQTT_PRODUCT_TOKEN)

def on_connect(client,userdata,flags,rc):
    if rc == 0:
        print("connect onenet ok!")
    else:
        print(f"connect onenet error,error id:{rc}")

def on_disconnect(client,userdata,rc):
    print(f"connect stop:{rc}")

client.on_connect = on_connect
client.on_disconnect = on_disconnect

print(f"connecting......")

try:
    client.connect(MQTT_BROKER,MQTT_PORT,keepalive=120)
    client.loop_forever()
except KeyboardInterrupt:
    print("\nexit")
except Exception as e:
    print(f"error:{e}")