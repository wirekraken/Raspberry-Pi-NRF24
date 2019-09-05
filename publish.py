import paho.mqtt.client as mqtt
from time import sleep

client = mqtt.Client()
client.username_pw_set('pi', password='123')
client.connect('192.168.43.244', 1883)

#client.loop_forever()

while (True):
    with open('data') as file:
        data = str(file.read())
    client.publish("Python", data)
    print("send : " + data)
    sleep(1)
