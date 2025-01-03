#include "ImagePreview.h"

ImagePreview::ImagePreview() {
    glGenTextures(1, &imageTexture);
}

void ImagePreview::load(const std::string imagePath, double scale) {

    cv::Mat image = imread(imagePath, cv::IMREAD_COLOR);

    cv::cvtColor(image, image, cv::COLOR_BGR2RGBA);

    cv::resize(image, imageData, cv::Size(), scale, scale);

    size = imageData.size();

    glBindTexture(GL_TEXTURE_2D, imageTexture);
//
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

//
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageData.cols, imageData.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData.ptr());
}

void ImagePreview::layout() {
    glBindTexture(GL_TEXTURE_2D, imageTexture);
    ImGui::Image((void*)(intptr_t)imageTexture, ImVec2(imageData.cols, imageData.rows));
}

/*void ImagePreview::close() {*/
/*    glDeleteTextures(1, &imageTexture);*/
/*}*/

ImagePreview::~ImagePreview() {
    glDeleteTextures(1, &imageTexture);
}


