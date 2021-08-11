import time
import glob
import os
import re
from PIL import Image
from tflite_runtime.interpreter import Interpreter
import numpy as np
from object_detection_utils import draw_boxes
import telegram_send

PATTERN_TO_WATCH="/var/lib/motion/*.jpg"
SAVED_MODEL_PATH = "./model_mobilenet_bee.tflite"
SAVED_MODEL_PATH = "./trailertruck.tflite"
ALTERED_IMAGE_OUTPUT = "/home/pi/Pictures/wildlife/"
TRAINED_MODEL_IMAGE_SIZE = (384,384)
OBSERVED_LABELS = ["big"]

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

def send_message_if_needed(results, filepath, labels_to_trigger):
    results_filtered = [res for res in results if res[0] in labels_to_trigger]
    if results_filtered:
      print("Triggering for %s." % (labels_to_trigger))
      serializable_results = [str(res) for res in results_filtered]
      telegram_send.send(messages=["Found for file at %s" % filepath])
      telegram_send.send(messages=[serializable_results])
    else:
      print("no trigger found.")

def interpret_results_custom(output_details, image, filepath, threshold, label_map):
    boxes = interpreter.get_tensor(output_details[0]['index'])[0]
    classes = interpreter.get_tensor(output_details[1]['index'])[0] 
    scores = interpreter.get_tensor(output_details[2]['index'])[0]
    class_names = [label_map[i] for i in classes.astype(int)]    
    aggregated_results = list(zip(class_names,scores))
    aggregated_meaningful_results = [res for res in aggregated_results if res[1] > threshold]
    altered_image_path = os.path.join(ALTERED_IMAGE_OUTPUT, os.path.basename(filepath))
    altered_image_array = draw_boxes(image, boxes, class_names, scores, max_boxes=10, min_score=threshold)
    altered_image = Image.fromarray(altered_image_array)
    altered_image.save(altered_image_path)
    return aggregated_meaningful_results 

def handle_new_file(filepath):
    print("New file detected '%s'" % filepath)
    input_details = interpreter.get_input_details() 
    output_details = interpreter.get_output_details()
    input_shape = input_details[0]['shape']
    image_original = Image.open(filepath)
    im_resized = image_original.resize(TRAINED_MODEL_IMAGE_SIZE)
    input_data = np.array(im_resized, np.uint8)
    interpreter.set_tensor(input_details[0]['index'], [input_data])
    interpreter.invoke()
    label_map = {0: 'big', 1: 'bigtrailer'}
    res = interpret_results_custom(output_details, im_resized, filepath, 0.5, label_map)
    send_message_if_needed(res, filepath, OBSERVED_LABELS)
    print(res)

while True:
  files = glob.glob(PATTERN_TO_WATCH)
  files.sort(key=os.path.getmtime, reverse=True)
  newest_file = files[0]
  if newest_file != previous_file:
    previous_file = newest_file
    # todo save and retrieve from disk?
    handle_new_file(newest_file)
  time.sleep(1)
  
