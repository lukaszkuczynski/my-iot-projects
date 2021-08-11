import time
import glob
import os
import re
from PIL import Image
from tflite_runtime.interpreter import Interpreter
import numpy as np
from object_detection_utils import draw_boxes
import sys

PATTERN_TO_WATCH="/var/lib/motion/*.jpg"
SAVED_MODEL_PATH = "./model_mobilenet_bee.tflite"
ALTERED_IMAGE_OUTPUT = "/home/pi/Pictures/wildlife/"

interpreter = Interpreter(SAVED_MODEL_PATH)
interpreter.allocate_tensors()

previous_file = None

coco_labels = {}

with open("./coco_labels.txt") as fread:
    lines = fread.read().split('\n')
    for line in lines: 
        number = re.split("\s+", line)[0]
        text = ' '.join(re.split("\s+", line)[1:])
        coco_labels[int(number)] = text

def interpret_results_coco(output_details, im_resized, filepath):
    boxes = interpreter.get_tensor(output_details[0]['index'])[0] 
    classes = interpreter.get_tensor(output_details[1]['index'])[0] 
    scores = interpreter.get_tensor(output_details[2]['index'])[0]
    class_names = [coco_labels[i] for i in classes.astype(int)]    
    print(list(zip(class_names,scores)))
    altered_image_path = os.path.join(ALTERED_IMAGE_OUTPUT, os.path.basename(filepath))
    altered_image_array = draw_boxes(im_resized, boxes, class_names, scores)
    altered_image = Image.fromarray(altered_image_array)
    altered_image.save(altered_image_path)
    

def handle_new_file(filepath):
    print("New file detected '%s'" % filepath)
    input_details = interpreter.get_input_details() 
    output_details = interpreter.get_output_details()
    input_shape = input_details[0]['shape']
    image_original = Image.open(filepath)
    im_resized = image_original.resize((320,320))
    input_data = np.array(im_resized, np.float32)
    interpreter.set_tensor(input_details[0]['index'], [input_data])
    interpreter.invoke()
    interpret_results_coco(output_details, im_resized, filepath)
    
if __name__ == '__main__':
  filename = sys.argv[1]
  handle_new_file(filename)
  
