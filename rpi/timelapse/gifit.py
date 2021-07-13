path = "/home/pi/Pictures/timelapse/*"
outpath = "/home/pi/Pictures/timelapsegif.gif"

import glob
from PIL import Image

max_files = 3050

imgs = sorted(glob.glob(path))
selected_imgs = [i for i in imgs][:max_files:5]

images = [Image.open(imgpath).resize((400,300)) for imgpath in selected_imgs]
first_frame = images[0]
first_frame.save(outpath, format='GIF', append_images=images, loop=0, save_all=True)
  
