# Azure_Kinect_Ubuntu
MS Azure Kinect transformation code for Ubuntu

1. transformation.cpp : apply transformation function in capture moment
2. transformation_video.cpp : apply transformation function in continuous frames
3. pre_transformation.cpp : apply transformation function with exist captured images


## For Ubuntu
+ To use, compile with g++ and make object file.
  + g++ -o test transformation.cpp -lk4a 'pkg-config --libs --cflags opencv` (opencv version < 4)


## For Windows
+ Make solution file and add opencv & k4a header in project settings.
