#include "Preview.h"

/**
* Constructor for Preview
* 
* Generates opengl texture that exists for the lifetime of Preview
*
*/
Preview::Preview() {

    glGenTextures(1, &imageTexture); // generate a single texture
    loaded = false;
    cols = 0;
    rows = 0;
}


/**
* Load an image preview from the image file and resize it based on scaling factor
* 
* @param imagePath filepath to image file
* @param scale image scaling factor
*/
void Preview::load(const std::string imagePath, double scale) 
{

    // Don't flip the image data
    stbi_set_flip_vertically_on_load(false);

    // reset rows and columns
    cols = 0;
    rows = 0;

    int numChannels;

    // Load image data using stb (this is dynamically allocated memory that must be freed)
    unsigned char* imageData = 
        stbi_load(imagePath.c_str(), &cols, &rows, &numChannels, STBIR_RGB);

    // compute resized image
    //int resizedCols = (int) std::round((double)cols*scale);
    //int resizedRows = (int) std::round((double)rows*scale);
    int resizedCols = cols / 24;
	int resizedRows = rows / 24;

    // Allocate memory for resized image data
    unsigned char* resizedImageData = (unsigned char*) malloc(resizedCols*resizedRows*numChannels);

    // Use resize implementation provided by stb lib
    stbir_resize_uint8_srgb(
            imageData, cols, rows, 0, 
            resizedImageData, resizedCols, resizedRows, 0,
            STBIR_RGB);

    // Bind image texture for this preview
    glBindTexture(GL_TEXTURE_2D, imageTexture);

    // Set MIN and MAG filter to GL_NEAREST for the least amount of distortion in image display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Set pixel unpacking by row length
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    // update row and column count
    cols = resizedCols;
    rows = resizedRows;

    // map image data to the opengl texture
    glTexImage2D(GL_TEXTURE_2D, 
            0, GL_RGB, cols, rows,
            0, GL_RGB, GL_UNSIGNED_BYTE, 
            resizedImageData);

    // Free dynamically allocated memory
    stbi_image_free(imageData);
    stbi_image_free(resizedImageData);

    // Set loaded to true
    loaded = true;

}


/**
* UI Layout function for Preview
* 
* Displays the image preview if it is loaded, and displayes loading text if image preview is not loaded
*/
void Preview::layout() {
    if (loaded) { // if image is loaded

        ImGui::Image(//draw image
            (void*)(intptr_t)imageTexture, // texture
            (ImVec2(cols, rows))); // pixel size of image preview 
    } else {
        ImGui::Text("Image preview loading..."); // Show text 
    }
}

/**
* Preview destructor
* 
* Deletes the image texture
*/
Preview::~Preview() {
    glDeleteTextures(1, &imageTexture); // destroy texture
}
