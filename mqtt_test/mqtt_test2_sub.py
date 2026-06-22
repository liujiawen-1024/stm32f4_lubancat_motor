# coding: gb2312

import paho.mqtt.client as mqtt
import json

# 相关配置
MQTT_PRODUCT_ID = "RJpPAS792o"
MQTT_PRODUCT_NAME = "Motor_Ctrl"
MQTT_PRODUCT_TOKEN = "version=2018-10-31&res=products%2FRJpPAS792o%2Fdevices%2FMotor_Ctrl&et=1939802874&method=md5&sign=g5TP2NJAqgrt%2BPIhMJqJgw%3D%3D"

MQTT_BROKER = f"mqtts.heclouds.com"
MQTT_PORT = 1883    

CLIENT_ID = f"{MQTT_PRODUCT_NAME}"
USERNAME = f"{MQTT_PRODUCT_ID}"

# =====Topic=====
# 订阅：接收云端下发的属性设置指令
TOPIC_PROPERTY_SET = f"$sys/{MQTT_PRODUCT_ID}/{MQTT_PRODUCT_NAME}/thing/property/set"

# 回调函数
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("连接成功!")
        client.subscribe(TOPIC_PROPERTY_SET)
        print(f"已订阅: {TOPIC_PROPERTY_SET}")

def on_disconnect(client,userdata,rc):
    print(f"连接中断:{rc}")

def on_message(client,userdata,msg):
    # 收到消息的回调函数
    print("\n" + "=" * 50)
    print("? 收到云端消息")
    print(f"   Topic: {msg.topic}")
    print(f"   Payload: {msg.payload.decode('utf-8')}")
    print("=" * 50)

    try:
        # 解析数据
        data = json.loads(msg.payload.decode('utf-8'))

        # 检查是否有 params 字段
        if 'params' in data:
            params = data['params']
            print(f"解析参数:{params}")

            # 处理不同的控制指令

            # 1，速度控制
            if 'speed' in params:
                seppd = params['speed']
                print(f"设置电机速度为:{seppd}%")

            # 2，启停控制
            if 'status' in params:
                status = params['status']
                if status == 1:
                    print(f"电机启动")
                elif status == 0:
                    print(f"电机关闭")
                else:
                    print(f"电机状态异常，已强制关闭")
                    status = 0

            # 3，方向控制
            if 'direction' in params:
                direction = params['direction']
                if 'direction' == 1:
                    print(f"前进")
                elif 'direction == 0':
                    print(f"后退")
                else:
                    print(f"方向状态异常，已强制关闭")
                    status = 0
            
            response_payload ={
                "id":data.get("id","1"),
                "code":200,
                "message":"success"
            }
            response_topic = f"$sys/{MQTT_PRODUCT_ID}/{MQTT_PRODUCT_NAME}/thing/property/set_reply"
            client.publish(response_topic,json.dumps(response_payload),qos=1)
            print(f"已回复云端:{response_payload}")

        else:
            print("没有params字段")
    except json.JSONDecodeError as e:
        print(f"JSON解析错误:{e}")
    except Exception as e:
        print(f"处理信息出错:{e}")

# 主程序
client = mqtt.Client(client_id=CLIENT_ID,clean_session=True)
client.username_pw_set(USERNAME,MQTT_PRODUCT_TOKEN)

client.on_connect = on_connect
client.on_disconnect = on_disconnect
client.on_message = on_message

print("? 正在连接 OneNET...")
print(f"? 将订阅: {TOPIC_PROPERTY_SET}")
print("等待云端指令...\n")

try:
    client.connect(MQTT_BROKER, MQTT_PORT, keepalive=120)
    client.loop_forever()
except KeyboardInterrupt:
    print("\n? 退出程序")
except Exception as e:
    print(f"? 错误: {e}")