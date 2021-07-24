import time
import glob
import os

PATTERN_TO_WATCH="/var/lib/motion/*.jpg"


previous_file = None

def handle_new_file(filepath):
  print(filepath)

while True:
  files = glob.glob(PATTERN_TO_WATCH)
  files.sort(key=os.path.getmtime, reverse=True)
  newest_file = files[0]
  if newest_file != previous_file:
    previous_file = newest_file
    # todo save and retrieve from disk?
    handle_new_file(newest_file)
  time.sleep(1)
  
