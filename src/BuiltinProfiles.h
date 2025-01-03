#pragma once

#include <opencv2/core.hpp>

#include "CameraProfile.h"

/**
 * namespace for builtin data that sholud be packaged with the application
 */
namespace Builtin {

    /**
     * Builtin IIHS camera profiles.
     */
    const CameraProfile CameraProfiles[3] = {

        { // PIXEL 6 PRO 0.7x Lens
            .loaded = true,
            .filePath = "",
            .device = "Pixel 6 Pro",
            .lensDescriptor = "Pixel 6 Pro 0.7x",
            
            .zoomLevel = 0.7f,

            .cameraIntrinsic = (cv::Mat_<double>(3,3) <<
                    1936.8582007165967, 0.0, 1536.0,
                    0.0, 1929.2924268946476, 2040.,
                    0.0, 0.0, 1.),

            .distCoeffs = (cv::Mat_<double>(1, 5) <<
                0.0067359392889204594, -0.017014988901871888,
            0.0010893760791893295, 2.3926476158983655e-06,
            0.0099130147877872568),

            .focalLengthMillimeter = 2.4f,
        },

        { // PIXEL 6 PRO 1x Lens
            .loaded = true,
            .filePath = "",
            .device = "Pixel 6 Pro",
            .lensDescriptor = "Pixel 6 Pro 1x",
            
            .zoomLevel = 1.0f,

            .cameraIntrinsic = (cv::Mat_<double>(3,3) <<
                    2972.940505808368, 0., 1535.5, 0., 2972.940505808368, 2039.5, 0., 0., 1.),

            .distCoeffs = (cv::Mat_<double>(1, 5) <<
                    0.052528784346770552, -0.13319817957921418,
       -0.00095232354087887222, -0.0040170683202363387,
       0.14338001781343532),

            .focalLengthMillimeter = 6.81f,
        },


        { // iPhone 12 Wide Camera
            .loaded = true,
            .filePath = "",
            .device = "iPhone 12",
            .lensDescriptor = "iPhone 12 Wide Camera",
            
            .zoomLevel = 1.0f,

            .cameraIntrinsic = (cv::Mat_<double>(3,3) <<
                3103.2527774727992, 0., 1511.5, 
                0., 3103.2527774727992, 2015.5, 
                0., 0., 1.),

            .distCoeffs = (cv::Mat_<double>(1, 5) <<
                0.14092290787761164, -0.26487858915435086,
                0.0075272825148241422, -0.0018871717720509976,
                0.24104829039664616),

            .focalLengthMillimeter = 26.0f,
        }};
}
