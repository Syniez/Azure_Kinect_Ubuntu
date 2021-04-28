# Azure_Kinect_Ubuntu
MS Azure Kinect transformation code for Ubuntu

1. transformation.cpp : apply transformation function in capture moment
2. transformation_video.cpp : apply transformation function in continuous frames
3. pre_transformation.cpp : apply transformation function with exist captured images


## For Ubuntu
+ To use, compile with g++ and make object file.
  + g++ -o <file name> transformation.cpp -lk4a 'pkg-config --libs --cflags OpenCV` (OpenCV version < 4)
  + (In my case, i didn't do any steps before using kinect. So connect kinect to USB 3.0 port and use sudo when excute file.)


## For Windows
+ Make solution file and add OpenCV & k4a header in project settings.
