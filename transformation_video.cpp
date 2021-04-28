#include <iostream>
#include <string>

#include <k4a/k4a.h>
#include <k4a/k4atypes.h>
#include <k4arecord/record.h>
#include <k4arecord/playback.h>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    cout << "start" << endl;

    // open device
    k4a_device_t device = NULL;
    k4a_device_open(K4A_DEVICE_DEFAULT, &device);
    cout << "Device : " << device << endl;

    // Get the size of the serial number
    size_t serial_size = 0;
    k4a_device_get_serialnum(device, NULL, &serial_size);

    // Allocate memory for the serial, then acquire it
    char* serial = (char*)(malloc(serial_size));
    k4a_device_get_serialnum(device, serial, &serial_size);
    printf("Opened device: %s\n", serial);
    free(serial);

    // Configuration
    k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
    config.camera_fps = K4A_FRAMES_PER_SECOND_30;
    config.color_format = K4A_IMAGE_FORMAT_COLOR_BGRA32;
    config.color_resolution = K4A_COLOR_RESOLUTION_1080P;
    config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
    config.synchronized_images_only = true;

    // Start capture
    k4a_device_start_cameras(device, &config);
    int frame_count = 0;

    while (true) {
        k4a_capture_t capture;
        k4a_image_t color_image, depth_image;
        k4a_device_get_capture(device, &capture, K4A_WAIT_INFINITE);

        // read color image from buffer
        color_image = k4a_capture_get_color_image(capture);
        uint8_t* color_data = (uint8_t*)(void*)k4a_image_get_buffer(color_image);
        cv::Mat color_frame = cv::Mat(k4a_image_get_height_pixels(color_image), k4a_image_get_width_pixels(color_image), CV_8UC4, color_data, cv::Mat::AUTO_STEP);

        // read depth image from buffer
        depth_image = k4a_capture_get_depth_image(capture);
        uint16_t* depth_data = (uint16_t*)(void*)k4a_image_get_buffer(depth_image);
        cv::Mat depth_frame = cv::Mat(k4a_image_get_height_pixels(depth_image), k4a_image_get_width_pixels(depth_image), CV_16UC1, depth_data);

        // calibration
        k4a_calibration_t calib;
        if (K4A_RESULT_SUCCEEDED != k4a_device_get_calibration(device, K4A_DEPTH_MODE_NFOV_UNBINNED, K4A_COLOR_RESOLUTION_1080P, &calib))
            return 0;

        // create transformation handle
        k4a_transformation_t trans;
        trans = k4a_transformation_create(&calib);

        // transformation depth image to rgb image
        k4a_image_t depth_transformed;
        k4a_image_create(K4A_IMAGE_FORMAT_DEPTH16, color_frame.cols, color_frame.rows, color_frame.cols * sizeof(uint16_t), &depth_transformed);
        k4a_transformation_depth_image_to_color_camera(trans, depth_image, depth_transformed);

        uint16_t* depth_trans = (uint16_t*)(void*)k4a_image_get_buffer(depth_transformed);
        cv::Mat depth_res = cv::Mat(k4a_image_get_height_pixels(color_image), k4a_image_get_width_pixels(color_image), CV_16UC1, depth_trans);

        // write images
        imwrite(cv::format("./result/rgb/%04d.png", frame_count), color_frame);
        imwrite(cv::format("./result/depth/%04d.png", frame_count), depth_res);

        cout << frame_count << "th frame processed." << endl;
        frame_count += 1;
    }

    // Release camera 
    k4a_device_stop_cameras(device);
    k4a_device_close(device);

    return 0;
}
