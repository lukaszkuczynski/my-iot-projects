download:
	aws s3 sync s3://sagemaker-s3-luk/tflite_frozen ~/artifacts/tflite_frozen

build:
	pip install -r requirements.txt

onnx:
	python3 -m tf2onnx.convert --saved-model ~/artifacts/savedmodel/mobilenet_tf2/1/ --opset 11 --output model.onnx --concrete_function 1

oldconvert:
	python3 converttensorrt.py \
		--inputsaved ~/artifacts/tensorflow_savedmodel/1 \
		--output ~/artifacts/tensorrt/avo

#filename="/home/lukjestson/artifacts/tensorflow_savedmodel/1/saved_model.pb"
#/home/lukjestson/artifacts/tensorflow_savedmodel/1/saved_model.pb
#filename="/home/lukjestson/artifacts/tflite_frozen/tflite_trucks_mobilenet_640/saved_model/saved_model.pb"
filename="/home/lukjestson/artifacts/savedmodel/mobilenet_tf1/1/saved_model.pb"

convert:
	python3 /usr/lib/python3.6/dist-packages/uff/bin/convert_to_uff.py $(filename) -o ssd_mobilenet_v2.uff -O NMS -p ./config.py

