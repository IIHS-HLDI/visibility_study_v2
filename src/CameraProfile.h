#pragma once

// #include <memory>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include "VehicleProject.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define sind(x) (sin(fmod((x),360)*M_PI/180))
#define cosd(x) (cos(fmod((x),360)*M_PI/180))

typedef struct _CameraProfile {

    bool loaded = false;
    std::string filePath; // filepath of camera profile (if not builtin it must be loaded from file)
    std::string device; // Device (phone name), e.g. Pixel 6 Pro or iPhone 12
    std::string lensDescriptor; // Description of the camera lens, e.g. 0.7x Lens or ultrawide lens
    
    float zoomLevel; // zoom level

    cv::Mat cameraIntrinsic; // camera intrinsic matrix

    cv::Mat distCoeffs; // distortion coefficients

    float focalLengthMillimeter; // focal length in millimeters

    /**
    * Loads profile from a file 
    * 
    * @param filePath path to camera profile file
    */
    bool loadProfile(const std::string& filePath) {
        try {
            cv::FileStorage inputFile(filePath, cv::FileStorage::READ);

            inputFile["device"] >> device;
            inputFile["profile_descriptor"] >> lensDescriptor;
            inputFile["camera_matrix"] >> cameraIntrinsic;
            inputFile["dist_coeffs"] >> distCoeffs;
            inputFile["zoom_level"] >> zoomLevel;
            inputFile["focal_length_mm"] >> focalLengthMillimeter;
        } catch (...) {
            return false;
        }

        loaded = true;
        return true;
    }

    /**
     * Undistorts an image based on the camera distortion coefficients
     * 
     * @param sourceImage opencv image data to undistort
     * 
     * @return undistorted image
     */
    cv::Mat undistort(cv::Mat sourceImage) {
        cv::Mat undistortedImage;
        cv::undistort(sourceImage, 
                undistortedImage, 
                cameraIntrinsic,
                distCoeffs);

        return undistortedImage;
    }

    /**
    * Projects uv image points to orthogonal grid
    *
    * @param imagePoints UV points of an image that are with respect to an origin in the top left corner of the image.
    * @param image measurement image data from VehicleProject
    * @param measurementOffsets lens and rig offsets from VehicleProject 
    * @param intersectionHeight height of reference plane, e.g. plane: z=0 or plane z=h for some value of h
    * @param distortion true to account for distortion using cv::undistort, false to skip distortion step
    *
    * @return vector of projected 2d world coordinates 
    */
    std::vector<cv::Point2f> imageToWorldTransform(
            std::vector<cv::Point2f> imagePoints, // uv-image points
            MeasurementImage image,
            MeasurementOffsets measurementOffset, 
            float intersectHeight,
            bool distortion) {

        std::vector<cv::Point2f> undistortedPoints(imagePoints.size()); // allocate vector for undistorted points

        if (distortion) {
            // Use opencv undistortImagePoints
            cv::undistortImagePoints(imagePoints, undistortedPoints,
                    cameraIntrinsic, distCoeffs);
        } else {
            // if distortion handling is off, simply copy points to undistortedPoints;
            undistortedPoints = imagePoints;
        }

        // OpenCV camera frame orients Z as normal to the lens. Our coordinate system has z normal to the ground.
        // This matrix changes from C frame to B frame.
        static cv::Mat rot_BC = (cv::Mat_<double>(3,3) <<
            1., 0., 0.,
            0., 0., 1.,
            0., -1., 0.);

        // This vector represents the center of the measurement rig in x and y and the 
        cv::Mat N_rigCenter = (cv::Mat_<double>(3,1) <<
                measurementOffset.rigPosition.x + measurementOffset.rigRadius, // Measuring the x rig position 
                -measurementOffset.rigPosition.y - measurementOffset.rigRadius,
                measurementOffset.rigPosition.z);


        // A frame is offset from B frame if the rig twist radius and lens offset is nonzero.
        //
        // The following figure 

        // Rig Radius refers to the offset between the phone and the axis of rotation of the rig neck.
        // Lens offset refers to the 
        //
        //           +y ^ (z-direction in C-frame)
        //              |
        //       +y     |
        //         ^    |    __ offset lens
        //         |    |   /
        //         |    |  /
        //         |    | /
        // ============()= ------------> +x  --                   <- Phone with offset lens, (A-frame)
        //         |                          | rig radius
        //         o-------------> +x        --                        <- rig rotation center (B-frame)
        //         |---|
        //          lens offset
        //
        //
        cv::Mat A_frameOffsets = (cv::Mat_<double>(3,1) <<
                measurementOffset.rigRadius,
                measurementOffset.lensOffset,
                0.0);


        // This should always be zero
        static double phi = 0;
        cv::Mat rot_AB = (cv::Mat_<double>(3,3) <<
                cos(phi), -sin(phi), 0.0,
                sin(phi),  cos(phi), 0.0,
                0.0,       0.0,      1.0);

        // We define a fixed coordinate frame N-frame as the front left corner of the vehicle
        //
        // The A-frame rotates with the rig
        // The N-frame is fixed at the front-left corner of the vehicle 
        //
        // The following figure shows a top-down view of the relationship between the A-frame and B-frame
        //
        //
        //
        //          +y
        //          ^
        //          |
        //          |
        //          |
        //          |
        //          |
        //          | 
        //  N-frame o------------> +x __
		//                             |
		//                             |
		//                             |
		//                             |
		//                             |
		//                             |
		//                             |
		//                             |
		//                             |
		//                  y-offset   |
		//                             |
		//                             |                  ____ yaw angle
		//                             |            |    /
		//                             |            |   /
		//                             |            |  /  ^
		//                             |            |_   /
        //                             |            | \ /  +y
        //                             |            |  /
        //                             |            | /
        //                             |            |/
        //                            --  a-frame   o_
        //                                            \_
        //                                              \_
        //                                                 \_> + x
        //        |--------------------------------|
        //                      x-offset

        // 3-rotation to convert from A and N frame
        cv::Mat rot_NA = (cv::Mat_<double>(3,3) <<
            cosd(image.yawAngle), sind(image.yawAngle), 0.0,
            -sind(image.yawAngle), cosd(image.yawAngle), 0.0,
            0.0, 0.0, 1.0);

        // Compute lens position relative to N-frame
        cv::Mat N_lensPosition = (rot_NA * A_frameOffsets) + N_rigCenter;

        // Define normal vector of reference plane
        static cv::Mat znormal = (cv::Mat_<double>(3,1) <<
                0.0, 0.0, 1.0);

        // Define a point on the plane
        // Since we only deal with planes of the form z=h only the intersectHeight needs to be updated 
        cv::Mat pointOnPlane = (cv::Mat_<double>(3,1) <<
                0.0, 0.0, intersectHeight);

        // Allocate vector for world points
        std::vector<cv::Point2f> worldPoints(imagePoints.size());

        for (int i = 0; i < undistortedPoints.size(); i++) {

            // Pixel position in camera coordinate frame (C-frame)
            cv::Mat pixelPos = (cv::Mat_<double>(3,1) <<
                    undistortedPoints[i].x,
                    undistortedPoints[i].y,
                    1);

            //Scale from pixels to centimeters still in C-frame
            cv::Mat r_scaled = (cameraIntrinsic.inv() * pixelPos); (focalLengthMillimeter / 10.0);

            // Express point in b-frame
            cv::Mat point_Bframe = rot_BC * r_scaled;

            // Express point in A-frame
            cv::Mat point_Aframe = (rot_AB * point_Bframe) + A_frameOffsets;

            // Express point in N-frame
            cv::Mat point_Nframe = (rot_NA * point_Aframe) + N_rigCenter;

            cv::Mat rayDirection = point_Nframe - N_lensPosition;

            // Compute lambda parameter
            double lambda = znormal.dot(pointOnPlane - N_lensPosition) / znormal.dot(rayDirection);

            // double lambda = -(znormal.dot(N_lensPosition)) / (znormal.dot(rayDirection)); <---- Simplifies to this for z = 0

            cv::Mat groundPoint;

            // Check validity of lambda
            if (lambda > 0) {
                groundPoint = N_lensPosition + lambda * rayDirection;
            }
            else { // where there is no intersection set to nan
                groundPoint = (cv::Mat_<double>(3,1) <<
                        std::nan("Inf"),
                        std::nan("Inf"),
                        std::nan("Inf"));
            }

            worldPoints[i].x = groundPoint.at<double>(0,0);
            worldPoints[i].y = groundPoint.at<double>(1,0);

        }

        return worldPoints;

    }
    /**/
    

} CameraProfile;
