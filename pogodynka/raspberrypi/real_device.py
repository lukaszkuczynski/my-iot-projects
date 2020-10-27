# Copyright (c) Microsoft. All rights reserved.
# Licensed under the MIT license. See LICENSE file in the project root for full license information.

import time
from bme280 import readBME280All
from datetime import datetime
import paho.mqtt.publish as publish


# Using the Python Device SDK for IoT Hub:
#   https://github.com/Azure/azure-iot-sdk-python
# The sample connects to a device-specific MQTT endpoint on your IoT Hub.
from azure.iot.device import IoTHubDeviceClient, Message

# The device connection string to authenticate the device with your IoT hub.
# Using the Azure CLI:
# az iot hub device-identity show-connection-string --hub-name {YourIoTHubName} --device-id MyNodeDevice --output table
import os
iot_key = os.getenv("IOT_DEVICE_KEY", "")
if not iot_key:
    raise Exception("Define IOT_DEVICE_KEY variable first")
CONNECTION_STRING = iot_key

# Define the JSON message to send to IoT Hub.
TEMPERATURE = 20.0
HUMIDITY = 60
MSG_TXT = '{{"temperature": {temperature},"humidity": {humidity}, "pressure": {pressure}}}'

def iothub_client_init():
    # Create an IoT Hub client
    client = IoTHubDeviceClient.create_from_connection_string(CONNECTION_STRING)
    return client

def iothub_client_telemetry_sample_run():

    try:
            client = iothub_client_init()

            # Build the message with simulated telemetry values.
            temperature,pressure,humidity = readBME280All()
            humidity = round(humidity, 2)
            msg_txt_formatted = MSG_TXT.format(temperature=temperature, humidity=humidity, pressure=pressure)
            message = Message(msg_txt_formatted, content_encoding='utf-8', content_type='application/json')

            # Add a custom application property to the message.
            # An IoT hub can filter on these properties without access to the message body.
            if temperature > 30:
              message.custom_properties["temperatureAlert"] = "true"
            else:
              message.custom_properties["temperatureAlert"] = "false"

            # Send the message.
            print( "Sending message: {}".format(message) )
            client.send_message(message)
            publish.multiple([
                {'topic': "balkon/temperature", 'payload': temperature},
                {'topic': "balkon/humidity", 'payload': humidity}
                ], hostname="192.168.0.138")
            print ( "Message successfully sent" )

    except KeyboardInterrupt:
        print ( "IoTHubClient sample stopped" )

if __name__ == '__main__':
    print ( "IoT Hub real device" )
    print ( "it's %s" % datetime.now() )
    iothub_client_telemetry_sample_run()
