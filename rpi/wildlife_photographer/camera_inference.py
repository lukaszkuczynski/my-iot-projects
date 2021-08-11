
SAVED_MODEL_PATH = "./model_mobilenet_bee.tflite"

import numpy as np
from tflite_runtime.interpreter import Interpreter
from io import BytesIO
from time import sleep
from picamera import PiCamera
from PIL import Image
import re

interpreter = Interpreter(SAVED_MODEL_PATH)
interpreter.allocate_tensors()

coco_labels = {}

with open("./coco_labels.txt") as fread:
    lines = fread.read().split('\n')
    for line in lines: 
        number = re.split("\s+", line)[0]
        text = re.split("\s+", line)[1:]
        coco_labels[int(number)] = text

def draw_text(text):
    print(text)

def take_pic():
    # Create an in-memory stream
    stream = BytesIO()
    draw_text("Taking pic..")
    with PiCamera() as camera:
      camera.start_preview()
      sleep(2)
      camera.capture(stream, 'jpeg')
      stream.seek(0)
      image = Image.open(stream)
      camera.close()
      return image

def interpret_results_coco(output_details):
    classes = interpreter.get_tensor(output_details[1]['index'])[0] 
    scores = interpreter.get_tensor(output_details[2]['index'])[0]
    print(classes.astype(int))
    class_names = [coco_labels[i] for i in classes.astype(int)]
    #print(list(zip(classes,scores)))
    print(list(zip(class_names,scores)))
 

def run_inference():
  input_details = interpreter.get_input_details() 
  output_details = interpreter.get_output_details()
  input_shape = input_details[0]['shape']
  image_original = take_pic()
  im_resized = image_original.resize((320,320))
  input_data = np.array(im_resized, np.float32)
  interpreter.set_tensor(input_details[0]['index'], [input_data])
  interpreter.invoke()
  interpret_results_coco(output_details)

if __name__ == '__main__':
  while(1):
    run_inference()
    sleep(1)


