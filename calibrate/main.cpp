#include <opencv2/opencv.hpp>

#include <iostream>

using namespace std;
using namespace cv;

vector<Point3f> getGoalSpacePoints(Size s) {
    vector<Point3f> goalSpacePoints;
    for (int i = 0; i < s.height; i++) {
        for (int j = 0; j < s.width; j++) {
            goalSpacePoints.push_back(Point3f(i, j, 0));
        }
    }
    return goalSpacePoints;
}

int main(int argc, char **argv) {
    Size patternSize(8, 6);
    vector<vector<Point2f> > allCorners;
    vector<vector<Point3f> > allGoalSpacePoints;
    Size imageSize;

    if (argc > 1) {
        int numImages = argc - 1;
        for (int i = 0; i < numImages; i++) {
            Mat image, grayScale;
            image = imread(argv[i + 1], CV_LOAD_IMAGE_COLOR);
            cvtColor(image, grayScale, COLOR_BGR2GRAY);
            imageSize = image.size();

            if (!image.data) {
                printf("No image data for image %d \n", i);
                return -1;
            }

            vector<Point2f> corners;

            bool patternfound = findChessboardCorners(image, patternSize, corners,
                                                      CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE
                                                      | CALIB_CB_FAST_CHECK);

            if (patternfound) {
                allCorners.push_back(corners);
                allGoalSpacePoints.push_back(getGoalSpacePoints(patternSize));
            }
        }
    } else {
        VideoCapture cap(0); // open the default camera
        cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
        if (!cap.isOpened()) {  // check if we succeeded
            return -1;
        }

        namedWindow("Capture", 1);
        while (allCorners.size() < 20) {
            Mat frame;
            cap >> frame; // get a new frame from camera
            imageSize = frame.size();
            imshow("Capture", frame);
            if (waitKey(30) % 256 == 32) { // space key
                vector<Point2f> corners;

                bool patternfound = findChessboardCorners(frame, patternSize, corners,
                                                          CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE
                                                          | CALIB_CB_FAST_CHECK);

                if (patternfound) {
                    cerr << "Pattern found." << endl;
                    allCorners.push_back(corners);
                    allGoalSpacePoints.push_back(getGoalSpacePoints(patternSize));
                }
            }
        }
    }

    Mat cameraMatrix, distortionCoeffs;
    vector<Mat> rvecs, tvecs;
    double error = calibrateCamera(allGoalSpacePoints, allCorners, imageSize,
                                   cameraMatrix, distortionCoeffs, rvecs, tvecs);

    double fovx, fovy, focalLength, aspectRatio;
    Point2d principalPoint;
    calibrationMatrixValues(cameraMatrix, imageSize, 0.0f, 0.0f, fovx, fovy, focalLength, principalPoint, aspectRatio);

    cout << fovx
         << "\n" << fovy
         << "\n" << focalLength
         << "\n" << cameraMatrix.at<double>(0, 2) << ", " << cameraMatrix.at<double>(1, 2)
         << "\n" << cameraMatrix.at<double>(0, 0)
         << "\n" << cameraMatrix.at<double>(1, 1)
         << "\n" << distortionCoeffs.at<double>(0, 0)
         << "\n" << distortionCoeffs.at<double>(0, 1)
         << "\n" << distortionCoeffs.at<double>(0, 2)
         << "\n" << distortionCoeffs.at<double>(0, 3)
         << "\n" << distortionCoeffs.at<double>(0, 4)
         << endl;

    for (int i = 0; i < allCorners.size(); i++) {
        double totalErrSquare = 0;
        for (int j = 0; j < allGoalSpacePoints[i].size(); j++) {
            Mat objectPoints(1, 3, CV_64FC1);
            objectPoints.at<double>(0, 0) = allGoalSpacePoints[i][j].x;
            objectPoints.at<double>(0, 1) = allGoalSpacePoints[i][j].y;
            objectPoints.at<double>(0, 2) = allGoalSpacePoints[i][j].z;

            Mat transformed;
            projectPoints(objectPoints, rvecs[i], tvecs[i], cameraMatrix, distortionCoeffs, transformed);

            Mat guess(2, 1, CV_64FC1);
            guess.at<double>(0, 0) = transformed.at<double>(0, 0);
            guess.at<double>(1, 0) = transformed.at<double>(0, 1);

            Mat c = Mat(allCorners[i][j]);
            Mat corner(guess.size(), CV_64FC1);
            c.convertTo(corner, CV_64FC1);

            Mat errorVec = corner - guess;
            double error = norm(errorVec);
            totalErrSquare += error * error;
        }
        double average = sqrt(totalErrSquare / allGoalSpacePoints[i].size());
        cerr << "Total square" << i << ": " << totalErrSquare
             << "\nAverage pixel error" << i << ": " << average << endl;
    }

    return 0;

    namedWindow("Capture", 1);
    VideoCapture cap(0);
    while (true) {
        Mat frame, distorted;
        cap >> distorted; // get a new frame from camera
        undistort(distorted, frame, cameraMatrix, distortionCoeffs);
        imshow("Capture", frame);
        if (waitKey(30) >= 0) break;
    }

    return 0;
}
