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
    if (argc < 2) {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    int numImages = argc - 1;
    Size patternSize(8, 6);
    vector<vector<Point2f> > allCorners;
    vector<vector<Point3f> > allGoalSpacePoints;
    Size imageSize;

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

        bool patternfound = findChessboardCorners(grayScale, patternSize, corners,
                                                  CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE
                                                  | CALIB_CB_FAST_CHECK);

        if (patternfound) {
            //cornerSubPix(grayScale, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
            allCorners.push_back(corners);
            allGoalSpacePoints.push_back(getGoalSpacePoints(patternSize));
        }
    }

    Mat cameraMatrix, distortionCoeffs;
    vector<Mat> rvecs, tvecs;
    double error = calibrateCamera(allGoalSpacePoints, allCorners, imageSize,
                                   cameraMatrix, distortionCoeffs, rvecs, tvecs, CV_CALIB_RATIONAL_MODEL);
    cout << "Width: " << distortionCoeffs.size().width << " Height: " << distortionCoeffs.size().height << endl;
    cout << "Calculated error: " << error << " from " << numImages << " images." << endl;
    cout << "Matrix" << endl;
    cout << cameraMatrix.at<double>(0, 0) << "\t" << cameraMatrix.at<double>(0, 1) << "\t"
         << cameraMatrix.at<double>(0, 2) << "\n"
         << cameraMatrix.at<double>(1, 0) << "\t" << cameraMatrix.at<double>(1, 1) << "\t"
         << cameraMatrix.at<double>(1, 2) << "\n"
         << cameraMatrix.at<double>(1, 0) << "\t" << cameraMatrix.at<double>(2, 1) << "\t"
         << cameraMatrix.at<double>(2, 2) << endl;

    double fovx, fovy, focalLength, aspectRatio;
    Point2d principalPoint;
    calibrationMatrixValues(cameraMatrix, imageSize, 0.0f, 0.0f, fovx, fovy, focalLength, principalPoint, aspectRatio);

    cout << "fovx: " << fovx
         << "\nfovy: " << fovy
         << "\nfocalLength: " << focalLength
         << "\npp: " << principalPoint.x << ", " << principalPoint.y
         << "\nfx: " << cameraMatrix.at<double>(0, 0)
         << "\nfy: " << cameraMatrix.at<double>(1, 1)
         << "\nk1: " << distortionCoeffs.at<double>(0, 0)
         << "\nk2: " << distortionCoeffs.at<double>(0, 1)
         << "\np1: " << distortionCoeffs.at<double>(0, 2)
         << "\np2: " << distortionCoeffs.at<double>(0, 3)
         << endl;

    //return 0;

    for (int i = 1; i < argc; i++) {
        Mat image = imread(argv[i], CV_LOAD_IMAGE_COLOR);
        Mat undistorted;

        undistort(image, undistorted, cameraMatrix, distortionCoeffs);

        namedWindow("Display Image", WINDOW_AUTOSIZE);
        imshow("Display Image", undistorted);

        waitKey(0);
    }

    return 0;
}