#include "Calibration.h"

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//https://github.com/CompileSense/StereoCamera

const int boardWidth = 9;                         //横向的角点数目
const int boardHeight = 6;                        //纵向的角点数据
const int boardCorner = boardWidth * boardHeight; //总的角点数据

const Size boardSize = Size(boardWidth, boardHeight);
vector<Point2f> corner; //某一副图像找到的角点

Calibration::Calibration()
{
}

void Calibration::calibrStart(void)
{
    Mat image = imread("F:\\windows\\Desktop\\temp\\2021-09-14-22-01-56-898\\camera0\\1631628143467.png",IMREAD_GRAYSCALE);

    namedWindow("window");
    imshow("window", image);
    bool isFind = findChessboardCorners(image, boardSize, corner,
    CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FILTER_QUADS);
    if (isFind == true)
    {
        //在图像上画出角点
        drawChessboardCorners(image, boardSize, corner, isFind);
        //显示画点后的图像
        namedWindow("chessboard");
        imshow("chessboard", image);
    }
    waitKey(0);
}
