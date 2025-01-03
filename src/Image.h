#pragma once

#include <sys/stat.h>

#include <glad/glad.h>

#include <glm/vec2.hpp>

/*#include <opencv2/opencv.hpp>*/

// #include <opencv2/core/core.hpp>
// #include <opencv2/imgcodecs/imgcodecs.hpp>
// #include <opencv2/imgproc/imgproc.hpp>

#include <string>

/*#include "ImageLoaders.h"*/

enum ImageLoadStatus : uint8_t {
    GOOD,
    INVALID_PATH,
    INVALID_FILE,
};

class Image
{
private:
	std::string filePath;

    GLuint texture;

    int width;
    int height;

    bool imageLoaded = false;

public:

	Image();
    static inline bool fileExists(const std::string& filePath);

    ImageLoadStatus load(const std::string filepath);

    bool isLoaded();

    GLuint getTexture();

	int getWidth();
	
	int getHeight();

    ~Image();
};

