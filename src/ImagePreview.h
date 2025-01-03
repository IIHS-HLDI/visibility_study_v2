#pragma once

#include <imgui.h>
#include <glad/glad.h>

#include <opencv2/opencv.hpp>
// #include <opencv2/core/core.hpp>
// #include <opencv2/imgcodecs/imgcodecs.hpp>
// #include <opencv2/imgproc/imgproc.hpp>

class ImagePreview {

private:

    cv::Size size;
    cv::Mat imageData; 
    GLuint imageTexture;


public:
    ImagePreview();

    void load(const std::string imagePath, double scale);

    void layout();

    /*void close();*/

    ~ImagePreview();

};
