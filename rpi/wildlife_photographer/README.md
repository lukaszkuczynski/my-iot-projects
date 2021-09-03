# Wildlife photographer

The beginning of this project were to allow for WildLife photographer machine. However, I was not able to collect wildlife photos as it happens rarely, so I prepared a model recognizing trucks on photos. But you will be able to use it with any Tensorflow Lite model of your choice.

## Collect images
I have used Raspberry Pi 3 with a camera and Motion running on top of it. It was collecting images whenever "anything" changed in front of the camera. Then I have manually chosen images containing object belonging to some classes (in my case trucks and trucks with a trailer).

## Prepare images
Before a training can be done the model has to be fed with object detection boundaries. It was done manually with the help of MakeSense web application. The boxes were then exported in Pascal VOC format.

## Train a model
Training a model was done by taking and adapting the regular tf model maker Colab intro notebook. Before the training was done images and annotations were saved on my Google Drive. The output of it was a tflite file that was used in predictions on a RPi computer.

## Run the inference
The tflite model was copied and put on a machine with the code that is available on that repo. It is also using Motion project, so whenever new files are collected the pictures are analyzed with the model saved locally. If new objects are detected the Telegram message is sent. Telegram was configured Telegram using info [on that Medium article](https://medium.com/@robertbracco1/how-to-write-a-telegram-bot-to-send-messages-with-python-bcdf45d0a580).
