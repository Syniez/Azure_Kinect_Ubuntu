# Azure_Kinect_Ubuntu
MS Azure Kinect transformation code for Ubuntu

1. transformation.cpp : apply transformation function in capture moment
2. pre_transformation.cpp : apply transformation function with exist captured images

+ for use, compile with g++ and make object file.
  + g++ -o test transformation.cpp -lk4a 'pkg-config --libs --cflags opencv`


