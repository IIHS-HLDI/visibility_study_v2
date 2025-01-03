#pragma once

#include <string>
#include <atomic>
#include <cmath>

#include <imgui.h>
#include <glad/glad.h>

#include <stb_image_resize2.h>
#include <stb_image.h>

/*#include <opencv2/opencv.hpp>*/

class Preview {

private:

    // std::atomic<bool> loaded;
    bool loaded;
    /*cv::Mat imageData;*/
    GLuint imageTexture;
    int cols;
    int rows;

public:
    Preview();

    int getCols() {
        return cols;
    }

    int getRows() {
        return rows;
    }

    bool isLoaded() {return loaded;}

    void load(const std::string imagePath, double scale); 

    void layout(); 

    ~Preview();

};
