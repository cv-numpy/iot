# pip install adafruit-circuitpython-servokit

from adafruit_servokit import ServoKit
kit = ServoKit(channels=16)

kit.servo[0].angle = 0
kit.servo[0].angle = 180
