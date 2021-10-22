import argparse
parser = argparse.ArgumentParser()
parser.add_argument('--inputsaved', required=True)
parser.add_argument('--output', required=True)
args = parser.parse_args()


import tensorflow as tf
from tensorflow.python.compiler.tensorrt import trt_convert as trt
input_saved_model_dir = args.inputsaved
output_saved_model_dir = args.output
converter = trt.TrtGraphConverterV2(input_saved_model_dir=input_saved_model_dir)
converter.convert()
converter.save(output_saved_model_dir)
