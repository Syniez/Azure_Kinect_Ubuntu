#include <iostream>
#include <k4a/k4a.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    // open device
    k4a_device_t device = NULL;
    k4a_device_open(K4A_DEVICE_DEFAULT, &device);

    // read saved image
    Mat rgb = imread("./orig.png");
    cvtColor(rgb, rgb, cv::COLOR_BGR2BGRA);
    Mat depth = imread("/data/2020년도 IITP씨앗_ 영상공유/20201102_시나리오2/2. 키넥트 카메라_카메라번호_촬영회차/depth/16bit/kinect_1_take_1/0001.png", 2);

    k4a_image_t ref;
    k4a_image_create(K4A_IMAGE_FORMAT_COLOR_BGRA32, rgb.cols, rgb.rows, rgb.cols * 4 * (int)sizeof(uint8_t), &ref);
    memcpy(k4a_image_get_buffer(ref), &rgb.ptr<cv::Vec4b>(0)[0], rgb.rows * rgb.cols * sizeof(cv::Vec4b));

    k4a_image_t tar;
    k4a_image_create(K4A_IMAGE_FORMAT_DEPTH16, depth.cols, depth.rows, depth.cols * 1 * (int)sizeof(uint16_t), &tar);
    memcpy(k4a_image_get_buffer(tar), &depth.ptr<cv::Vec2b>(0)[0], depth.rows * depth.cols * sizeof(cv::Vec2b));

    // make calibration handle
    k4a_calibration_t calib;
    k4a_device_get_calibration(device, K4A_DEPTH_MODE_NFOV_UNBINNED, K4A_COLOR_RESOLUTION_1080P, &calib);

    // geometry transformation
    k4a_transformation_t transformation;
    transformation = k4a_transformation_create(&calib);
    k4a_image_t result;
    k4a_image_create(K4A_IMAGE_FORMAT_DEPTH16, rgb.cols, rgb.rows, rgb.cols*sizeof(uint16_t), &result);
    k4a_transformation_depth_image_to_color_camera(transformation, tar, result);

    uint16_t* depth_trans = (uint16_t*)(void*)k4a_image_get_buffer(result);
    cv::Mat depth_res = cv::Mat(k4a_image_get_height_pixels(ref), k4a_image_get_width_pixels(ref), CV_16UC1, depth_trans);
    imwrite("./xzxzx.png", depth_res);

    k4a_device_close(device);
    return 0;
}