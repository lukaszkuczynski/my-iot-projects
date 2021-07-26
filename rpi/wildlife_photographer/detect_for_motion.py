import time
import glob
import os
import re
from PIL import Image
from tflite_runtime.interpreter import Interpreter
import numpy as np

PATTERN_TO_WATCH="/var/lib/motion/*.jpg"
SAVED_MODEL_PATH = "./ssd_mobilenet.tflite"

interpreter = Interpreter(SAVED_MODEL_PATH)
interpreter.allocate_tensors()

previous_file = None

coco_labels = {}

with open("./coco_labels.txt") as fread:
    lines = fread.read().split('\n')
    for line in lines: 
        print(line)
        number = re.split("\s+", line)[0]
        text = re.split("\s+", line)[1:]
        coco_labels[int(number)] = text

def interpret_results_coco(output_details):
    boxes = interpreter.get_tensor(output_details[0]['index'])[0] 
    print(boxes)
    classes = interpreter.get_tensor(output_details[1]['index'])[0] 
    scores = interpreter.get_tensor(output_details[2]['index'])[0]
    class_names = [coco_labels[i] for i in classes.astype(int)]
    print(list(zip(class_names,scores)))

def handle_new_file(filepath):
    print("New file detected '%s'" % filepath)
    input_details = interpreter.get_input_details() 
    output_details = interpreter.get_output_details()
    input_shape = input_details[0]['shape']
    image_original = Image.open(filepath)
    im_resized = image_original.resize((300,300))
    input_data = np.array(im_resized, np.uint8)
    interpreter.set_tensor(input_details[0]['index'], [input_data])
    interpreter.invoke()
    interpret_results_coco(output_details)
    

while True:
  files = glob.glob(PATTERN_TO_WATCH)
  files.sort(key=os.path.getmtime, reverse=True)
  newest_file = files[0]
  if newest_file != previous_file:
    previous_file = newest_file
    # todo save and retrieve from disk?
    handle_new_file(newest_file)
  time.sleep(1)
  
