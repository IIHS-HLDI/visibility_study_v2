#pragma once


#include <opencv2/opencv.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


/**
* Stores the data for a physical measurement grid and its virtual counterpart
*
*/
typedef struct _GridConfiguration {

    float height = 15; // height of the grid (grid y dimension) in meters
    float width = 8.5; // width of the grid (grid x dimension) in meters
    int divsX = 17; // Number of grid divisions along the x direction
    int divsY = 30;  // number of grid divisions along the y direction

    int markerIntervalX = 5; // interval to delineate grid along the x direction
    int markerIntervalY = 5; // interval to delineate grid along the y direction

    bool flipX; // Flip grid along the x axis with respect to origin
    bool flipY; // flip grid along th y axis with respect to origin

    // x Offset of the grid corner from the origin
    float xOffset;

    // y Offset of the grid corner from the origin
    float yOffset;

    glm::vec2 topLeft = glm::vec2(-100.0f, 100.0f); // top left point

    glm::vec2 topRight = glm::vec2(100.0f, 100.0f); // top right point

    glm::vec2 botLeft = glm::vec2(-100.0f, -100.0f); // bottom left point

    glm::vec2 botRight = glm::vec2(100.0f, -100.0f); // bottom right point

    cv::Mat pM; // matrix from perspective to 
    cv::Mat pM_inv;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(height),
            CEREAL_NVP(width),
            CEREAL_NVP(divsX),
            CEREAL_NVP(divsY),
            CEREAL_NVP(markerIntervalX),
            CEREAL_NVP(markerIntervalY),
            CEREAL_NVP(flipX),
            CEREAL_NVP(flipY),
            CEREAL_NVP(xOffset),
            CEREAL_NVP(yOffset),
            CEREAL_NVP(topLeft.x),
            CEREAL_NVP(topLeft.y),
            CEREAL_NVP(topRight.x),
            CEREAL_NVP(topRight.y),
            CEREAL_NVP(botLeft.x),
            CEREAL_NVP(botLeft.y),
            CEREAL_NVP(botRight.x),
            CEREAL_NVP(botRight.y));

    }


    glm::vec2 gridToWorldTransform(cv::Point2f input) {

        float f_flipX = 1;
        float f_flipY = 1;
        if (flipX) {
            f_flipX = -1;
        }
        if (flipY) {
            f_flipY = -1;
        }

        return glm::vec2((input.x - botLeft.x)*f_flipX + xOffset, 
                (input.y - botLeft.y)*f_flipY + yOffset);
    }

    float distance(glm::vec2 u, glm::vec2 v) {
        glm::vec2 diffVec = u - v;

        return sqrt(pow(diffVec.x, 2) + pow(diffVec.y, 2));
        /*return glm::abs(u-v);*/

    }

    bool grabTopLeft(glm::vec2 mousePos) {
        return distance(this->topLeft, mousePos) <= 50.0;
    }

    bool grabTopRight(glm::vec2 mousePos) {
        return distance(this->topRight, mousePos) <= 50.0;
    }
    
    bool grabBotLeft(glm::vec2 mousePos) {
        return distance(this->botLeft, mousePos) <= 50.0;
    }

    bool grabBotRight(glm::vec2 mousePos) {
        return distance(this->botRight, mousePos) <= 50.0;
    }

    int grab(glm::vec2 mousePos) {
        if (grabBotRight(mousePos)) {
            return 0;
        } else if (grabBotLeft(mousePos)) {
            return 1;
        } else if (grabTopRight(mousePos)) {
            return 2;
        } else if (grabTopLeft(mousePos)) {
            return 3;
        }
        return -1;
    }

    void nudgePosition(int i, double deltaX, double deltaY) {
        if (i == 0) {
            botLeft.x += deltaX;
            botLeft.y += deltaY;
        } else if (i == 1) {
            topLeft.x += deltaX;
            topLeft.y += deltaY;
        } else if (i == 2) {
            topRight.x += deltaX;
            topRight.y += deltaY;
        } else if (i == 3) {
            botRight.x += deltaX;
            botRight.y += deltaY;
        }
    }

    void setPosition(int i, glm::vec2 newPos) {
        if (i == 0) {
            botRight.x = newPos.x;
            botRight.y = newPos.y;
        } else if (i == 1) {
            botLeft.x = newPos.x;
            botLeft.y = newPos.y;
        } else if (i == 2) {
            topRight.x = newPos.x;
            topRight.y = newPos.y;
        } else if (i == 3) {
            topLeft.x = newPos.x;
            topLeft.y = newPos.y;
        }
    }

    void updatePerspectiveMatrices() {

        std::vector<cv::Point2f> src_shape(4);
		std::vector<cv::Point2f> dst_shape(4);

        // Take source shape as the quadrilateral defined in the perspective view
		src_shape[0] = cv::Point2f(botLeft.x, botLeft.y);
		src_shape[1] = cv::Point2f(topLeft.x, topLeft.y);
		src_shape[2] = cv::Point2f(topRight.x, topRight.y);
		src_shape[3] = cv::Point2f(botRight.x, botRight.y);

        // convert destination shape to centimeters
        //
		dst_shape[0] = cv::Point2f(botLeft.x, botLeft.y);
		dst_shape[1] = cv::Point2f(botLeft.x, botLeft.y + (this->height)*100);
		dst_shape[2] = cv::Point2f(botLeft.x + (this->width)*100, botLeft.y + (this->height) * 100);
		dst_shape[3] = cv::Point2f(botLeft.x + (this->width)*100, botLeft.y);

		this->pM = cv::getPerspectiveTransform(dst_shape, src_shape);
		this->pM_inv = cv::getPerspectiveTransform(src_shape, dst_shape);
    }

    

} GridConfiguration;
