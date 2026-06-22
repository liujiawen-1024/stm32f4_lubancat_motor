# coding: gb2312

import paho.mqtt.client as mqtt
import json
import time

# 相关配置
MQTT_PRODUCT_ID = "RJpPAS792o"
MQTT_PRODUCT_NAME = "Motor_Ctrl"
MQTT_PRODUCT_TOKEN = "version=2018-10-31&res=products%2FRJpPAS792o%2Fdevices%2FMotor_Ctrl&et=1939802874&method=md5&sign=g5TP2NJAqgrt%2BPIhMJqJgw%3D%3D"

MQTT_BROKER = f"mqtts.heclouds.com"
MQTT_PORT = 1883    

CLIENT_ID = f"{MQTT_PRODUCT_NAME}"
USERNAME = f"{MQTT_PRODUCT_ID}"

# Topic
TOPIC_PROPERTY_SET = f"$sys/{MQTT_PRODUCT_ID}/{MQTT_PRODUCT_NAME}/thing/property/set"

# 回调函数
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("连接成功!")
        client.subscribe(TOPIC_PROPERTY_SET)
        print(f"已订阅: {TOPIC_PROPERTY_SET}")

        data_send_speed(client,30)

def on_disconnect(client,userdata,rc):
    print(f"连接中断:{rc}")

# 发送函数
def data_send_speed(client,speed):
    topic = f"$sys/{MQTT_PRODUCT_ID}/{MQTT_PRODUCT_NAME}/thing/property/post"
    payload={
        "id":str(int(time.time())),
        "version":"1.0",
        "params":{
            "speed":{
                "value":speed
            }
        }
    }

    client.publish(topic,json.dumps(payload))
    print(f"上报速度:{speed}%")

# ===== 主程序 =====

# clinet
client = mqtt.Client(client_id=CLIENT_ID,clean_session=True)
client.username_pw_set(USERNAME,MQTT_PRODUCT_TOKEN)

client.on_connect = on_connect
client.on_disconnect = on_disconnect

print("? 正在连接 OneNET...")
print(f"? 将订阅: {TOPIC_PROPERTY_SET}")

try:
    client.connect(MQTT_BROKER, MQTT_PORT, keepalive=120)
    client.loop_forever()
except KeyboardInterrupt:
    print("\n? 退出程序")
except Exception as e:
    print(f"? 错误: {e}")