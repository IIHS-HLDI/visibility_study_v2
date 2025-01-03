#include "PreviewLoader.h"

PreviewLoader::PreviewLoader(MeasurementConfig measurementConfig) : 
    imageSizes(measurementConfig.images.size()),
    loadStatus(measurementConfig.images.size(), false),
    filepaths(measurementConfig.images.size()),
    imageData(measurementConfig.images.size())
{

    for (size_t i = 0; i < measurementConfig.images.size(); i++) {
        filepaths[i] = measurementConfig.images.imageFilepath;
    }

    glGenTextures(measurementConfig.images.size(), &imageTextureArray);
}

void PreviewLoader::loadImage(unsigned int i) 
{
    cv::Mat image = imread(filepaths[i], cv::IMREAD_COLOR);

    cv::cvtColor(image, image, cv::COLOR_BGR2RGBA);


    cv::resize(image, imageData[i], cv::Size(), 0.04, 0.04, cv::INTER_LINEAR);

    glBindTexture(GL_TEXTURE_2D, imageTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTExParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
            imageData.cols, imageData.rows, 0, 
            GL_RGBA, GL_UNSIGNED_BYTE, imageData[i].ptr());
}


std::tuple<GLuint*, ImVec2> PreviewLoader::getImageTexture(unsigned int i) 
{
    if (loadStatus[i]) {
        return std::make_tuple(imageTextureArray+i, imageSizes[i]);
    } else {
        loadImage(i);
    }
}


PreviewLoader::~PreviewLoader() {
    glDeleteTextures(imageData.size(), &imageTextureArray);
}
