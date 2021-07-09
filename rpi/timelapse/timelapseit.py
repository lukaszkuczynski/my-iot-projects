from time import sleep
from picamera import PiCamera
import os

save_folder = "/home/pi/Pictures/timelapse"
period = 10
preview_time = 2

camera = PiCamera()
camera.resolution = (1920, 1080)

picture_number = 0

while True:
  camera.start_preview()
  sleep(2)
  filename = "%.4d.jpg" % picture_number
  picture_path = os.path.join(save_folder, filename)
  print("Gonna save to '%s'" % picture_path)
  camera.capture(picture_path)
  print("Pic captured")
  picture_number += 1
  sleep(period)
