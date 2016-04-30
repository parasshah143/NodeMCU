# Copyright (c) 2010,2011 Roger Light <roger@atchoo.org>
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
# 3. Neither the name of mosquitto nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

import mosquitto, os, urlparse
import os
import time
import subprocess
import RPi.GPIO as GPIO  

from ubidots import ApiClient
global mailflag
mailflag = 0

api = ApiClient("88a38af294d0281ee1153b650ad13ce8314833f4")

test_variable = api.get_variable("56b047d67625425379ae8359")
test_variable1 = api.get_variable("56fbc61a7625426d53b624fc")

from datetime import datetime
fo = open("/home/pi/data.txt","rw+")
print "Name of the file: ", fo.name

# to use Raspberry Pi board pin numbers  
GPIO.setmode(GPIO.BOARD)  
# set up GPIO output channel  
GPIO.setup(11, GPIO.OUT)  


# Define event callbacks
def on_connect(mosq, obj, rc):
	print("rc: " + str(rc))

def on_message(mosq, obj, msg):
#    fo = open("data.txt","rw+")
	print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))
	fo.write(msg.payload)
	fo.write("  ")
	n = datetime.now()
	fo.write("%s"%n)
	fo.write("\r\n")
	
	if msg.payload == "detected_1" and msg.topic == "touch":
		print 'detected_1'
		test_value = 50
		test_variable.save_value({'value':test_value})
		print('Value is %d' %test_value)
		GPIO.output(11,GPIO.HIGH)  	
	elif msg.payload == 'detected_0':
		test_value = 25
		test_variable.save_value({'value':test_value})
		print('Value is %d' %test_value)
		GPIO.output(11,GPIO.LOW)  
	
	if msg.topic == "ldr":
		value = msg.payload
		test_value1 = value
		test_variable1.save_value({'value':test_value1})
	else:
		print 'No LDR'
	fo.flush()
def on_publish(mosq, obj, mid):
	print("mid: " + str(mid))

def on_subscribe(mosq, obj, mid, granted_qos):
	print("Subscribed: " + str(mid) + " " + str(granted_qos))

def on_log(mosq, obj, level, string):
	print(string)

mqttc = mosquitto.Mosquitto()
# Assign event callbacks
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe

# Uncomment to enable debug messages
mqttc.on_log = on_log

# Parse CLOUDMQTT_URL (or fallback to localhost)
url_str = os.environ.get('CLOUDMQTT_URL', 'http://192.168.1.107:1883')
url = urlparse.urlparse(url_str)

# Connect
mqttc.username_pw_set(url.username, url.password)
mqttc.connect(url.hostname, url.port)

# Start subscribe, with QoS level 0
mqttc.subscribe("touch", 0)
mqttc.subscribe("ldr", 0)

# Publish a message
mqttc.publish("topic", "my message")

# Continue the network loop, exit when an error occurs
rc = 0
while rc == 0:
	rc = mqttc.loop()
print("rc: " + str(rc))
GPIO.setwarnings(False)
#fo.close()
