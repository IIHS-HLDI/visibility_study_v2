#include "Image.h"

/*#define STB_IMAGE_IMPLEMENTATION*/
#include <stb_image.h>

Image::Image() {

    height = 0;
    width = 0;
}

ImageLoadStatus Image::load(const std::string filePath) {

	if (!fileExists(filePath)) {
		//Throw error can't find file
		return ImageLoadStatus::INVALID_PATH;
	}
    // Load from file
    stbi_set_flip_vertically_on_load(true);
    int imageWidth = 0;
    int imageHeight = 0;
    unsigned char* imageData = 
        stbi_load(filePath.c_str(), &imageWidth, &imageHeight, NULL, 4);

    if (imageData == NULL) {
		return ImageLoadStatus::INVALID_FILE;
    }

    width = imageWidth;
    height = imageHeight;

    // Create a OpenGL texture identifier
    //
    /*GLuint image_texture;*/
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    glTexImage2D(GL_TEXTURE_2D, 
            0, GL_RGBA, width, height, 
            0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    stbi_image_free(imageData);

	return ImageLoadStatus::GOOD;
}

bool Image::fileExists(const std::string& filePath) {
	struct stat buffer;
	return (stat(filePath.c_str(), &buffer) == 0);
}

GLuint Image::getTexture() {
    return this->texture;
}

int Image::getWidth() {
	return width;
}

int Image::getHeight() {
	return height;
}

bool Image::isLoaded() {
    return imageLoaded;
}

Image::~Image() {
	glDeleteTextures(1, &texture);
}
