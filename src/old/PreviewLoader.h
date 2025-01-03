#pragma once

#include <tuple>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

#include <imgui.h>

#include <glad/glad.h>

class PreviewLoader {

private:

    GLuint imageTextureArray;
    std::vector<ImVec2> imageSizes;
    std::vector<bool> loadStatus;
    std::vector<std::string> filepaths;
    std::vector<cv::Mat> imageData;

    void loadImage(unsigned int i);


public:

    PreviewLoader();


    std::tuple<GLuint*,ImVec2> getImageTexture(unsigned int i);


    ~PreviewLoader();
};
