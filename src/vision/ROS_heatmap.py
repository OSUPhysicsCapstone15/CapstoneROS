import numpy as np
np.random.seed(1337)  # for reproducibility

from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Convolution2D, MaxPooling2D
from keras.utils import np_utils
from keras import backend as K
from keras.models import model_from_json
from keras.preprocessing.image import ImageDataGenerator
import keras.preprocessing.image as kimg
from matplotlib import pyplot as plt
import argparse
import cv2
import theano
import os


# Now load json and create model
json_file = open("bestmodel.json", 'r')
loaded_model_json = json_file.read()
json_file.close()
print('test')
model = model_from_json(loaded_model_json)
#print(model.shape())

print('this is working')
#
# load weights into new model
model.load_weights("best_model.h5")
print("Loaded model from disk")
model.compile(loss='categorical_crossentropy',
              optimizer='adadelta',
              metrics=['accuracy'])

# These parameters need to be the same as for *validation* part of training
img_width, img_height = 150, 150
test_datagen = ImageDataGenerator(rescale=1./255)

#Take picture:

# Camera 0 is the integrated web cam on my netbook
#camera_port = -1
 
#Number of frames to throw away while the camera adjusts to light levels
#ramp_frames = 30
 
# Now we can initialize the camera capture object with the cv2.VideoCapture class.
# All it needs is the index to a camera port.
#camera = cv2.VideoCapture(camera_port)
 
# Captures a single image from the camera and returns it in PIL format
#def get_image():
 # read is the easiest way to get a full image out of a VideoCapture object.
# retval, im = camera.read()
# return im
 
# Ramp the camera - these frames will be discarded and are only used to allow v4l2
# to adjust light levels, if necessary
#for i in xrange(ramp_frames):
# temp = get_image()
#print("Taking image...")
# Take the actual image we want to keep
#camera_capture = get_image()
#file = "webcam_test.jpg"
# A nice feature of the imwrite method is that it will automatically choose the
# correct format based on the file extension you provide. Convenient!
#cv2.imwrite(file, camera_capture)
 
# You'll want to release the camera, otherwise you won't be able to create a new
# capture object until your script exits
#del(camera)

#Format picture and test it
im_original = cv2.resize(cv2.imread('webcam_test.jpg'), (img_width, img_height))#data/train/blob/blob-1235.jpg
#print(np.shape(im_original))
im = im_original.transpose((2,0,1))
#print(np.shape(im))
im = np.expand_dims(im, axis=0)
#print(np.shape(im))
im_converted = cv2.cvtColor(im_original, cv2.COLOR_BGR2RGB)
plt.imshow(im_converted)
plt.savefig("blob-0735.pdf")
prediction = model.predict(im)
print (prediction)
blob_bool=0;

if (prediction<0.2):
    print("I see the blob!")
    blob_bool=1
    
else:
    print("Blob not found.")
    blob_bool=0



def get_activations(model, layer, X_batch):#3 works
    get_activations = K.function([model.layers[3].input, K.learning_phase()], [model.layers[layer].output,])
    activations = get_activations([X_batch,0])
    return activations

if (blob_bool==1):
	im_2 = cv2.resize(cv2.imread('webcam_test_filtered.jpg'), (img_width, img_height))#data/train/blob/blob-1235.jpg
	im1 = im_2.transpose((2,0,1))
	im1 = np.expand_dims(im1, axis=0)
	feat = get_activations(model,2,im1)#2 works
	plt.imshow(feat[0][0][0])
	plt.savefig("heatmap_real.jpg")
	print("Created heatmap")
elif (os.path.isfile('heatmap_real.jpg')):
	os.remove('heatmap_real.jpg')

