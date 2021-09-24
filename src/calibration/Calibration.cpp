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
const int imageWidth = 752;                         //摄像头的分辨率
const int imageHeight = 480;
const int boardWidth = 9;                           //横向的角点数目
const int boardHeight = 6;                          //纵向的角点数据
const int boardCorner = boardWidth * boardHeight;   //总的角点数据
const int squareSize = 25;                          //标定板黑白格子的大小 单位mm
const Size imageSize = Size(imageWidth, imageHeight);

const Size boardSize = Size(boardWidth, boardHeight);
Mat intrinsicL;                            //相机内参数
Mat distortion_coeffL;                     //相机畸变参数
vector<Mat> rvecsL;                        //旋转向量
vector<Mat> tvecsL;                        //平移向量
vector<vector<Point2f>> cornersL;          //各个图像找到的角点的集合 和objRealPoint 一一对应

Mat intrinsicR;                            //相机内参数
Mat distortion_coeffR;                     //相机畸变参数
vector<Mat> rvecsR;                        //旋转向量
vector<Mat> tvecsR;                        //平移向量
vector<vector<Point2f>> cornersR;          //各个图像找到的角点的集合 和objRealPoint 一一对应

vector<vector<Point3f>> objRealPoint; //各副图像的角点的实际物理坐标集合

vector<Point2f> cornerL; //某一副图像找到的角点
vector<Point2f> cornerR; //某一副图像找到的角点

Mat R, T, E, F;                                         //R 旋转矢量 T平移矢量 E本征矩阵 F基础矩阵  
Mat Rl, Rr, Pl, Pr, Q;                                  //校正旋转矩阵R，投影矩阵P 重投影矩阵Q (下面有具体的含义解释）   
Mat mapLx, mapLy, mapRx, mapRy;                         //映射表  
Rect validROIL, validROIR;                              //图像校正之后，会对图像进行裁剪，这里的validROI就是指裁剪之后的区域  

//相机内参矩阵
Mat cameraMatrixL;
Mat distCoeffL;
Mat cameraMatrixR ;
Mat distCoeffR ;

Calibration::Calibration()
{
}

/*计算标定板上模块的实际物理坐标*/
static void calRealPoint(vector<vector<Point3f>>& obj, int boardwidth, int boardheight, int imgNumber, int squaresize)
{
	//  Mat imgpoint(boardheight, boardwidth, CV_32FC3,Scalar(0,0,0));  
	vector<Point3f> imgpoint;
	for (int rowIndex = 0; rowIndex < boardheight; rowIndex++)
	{
		for (int colIndex = 0; colIndex < boardwidth; colIndex++)
		{
			//  imgpoint.at<Vec3f>(rowIndex, colIndex) = Vec3f(rowIndex * squaresize, colIndex*squaresize, 0);  
			imgpoint.push_back(Point3f(rowIndex * squaresize, colIndex * squaresize, 0));
		}
	}
	for (int imgIndex = 0; imgIndex < imgNumber; imgIndex++)
	{
		obj.push_back(imgpoint);
	}
}

void Calibration::calibrStart(void)
{
    int goodFrameCount = 0;
    Mat imgL,boardimgL;
    Mat imgR,boardimgR;
    for (int i = 0; i < 10; i++)
    {

        char buffL[100];
        snprintf(buffL, sizeof(buffL), "D:\\3.code\\MatlabLIB\\image6\\camera0\\camera0_%d.png", i);
        std::string buffAsStdStrL = buffL;

        imgL = imread(buffAsStdStrL, IMREAD_GRAYSCALE);
        imgL.copyTo(boardimgL);

        char buffR[100];
        snprintf(buffR, sizeof(buffR), "D:\\3.code\\MatlabLIB\\image6\\camera1\\camera1_%d.png", i);
        std::string buffAsStdStrR = buffR;

        imgR = imread(buffAsStdStrR, IMREAD_GRAYSCALE);
        imgR.copyTo(boardimgR);

        if (imgL.data && imgR.data)
        {

            bool isFindL = findChessboardCorners(imgL, boardSize, cornerL,
                                                CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FILTER_QUADS);
            bool isFindR = findChessboardCorners(imgR, boardSize, cornerR,
                                                CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FILTER_QUADS);                                                
            if (isFindL && isFindR)
            {
                //指定亚像素计算迭代标注
                cv::TermCriteria criteria = cv::TermCriteria(
                    cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS,
                    40,
                    0.01);
                cornerSubPix(imgL, cornerL, Size(5, 5), Size(-1, -1), criteria);
                //cornerL.erase(cornerL.begin()+4, cornerL.end());
                //在图像上画出角点
                drawChessboardCorners(boardimgL, boardSize, cornerL, isFindL);
                cornersL.push_back(cornerL);



                cornerSubPix(imgR, cornerR, Size(5, 5), Size(-1, -1), criteria);
                //cornerR.erase(cornerR.begin()+4, cornerR.end());
                //在图像上画出角点
                drawChessboardCorners(boardimgR, boardSize, cornerR, isFindR);
                cornersR.push_back(cornerR);

                goodFrameCount++;
                //显示画点后的图像
                namedWindow(buffAsStdStrL);
                imshow(buffAsStdStrL, boardimgL);
            }
        }
    }

    // /*计算实际的校正点的三维坐标*/
    calRealPoint(objRealPoint, boardWidth, boardHeight, goodFrameCount, squareSize);
    // calibrateCamera(objRealPoint, cornersL, Size(boardWidth, boardHeight), intrinsicL, distortion_coeffL, rvecsL, tvecsL, 0);
    // calibrateCamera(objRealPoint, cornersR, Size(boardWidth, boardHeight), intrinsicR, distortion_coeffR, rvecsR, tvecsR, 0);

	//标定摄像头
	//由于左右摄像机分别都经过了单目标定
	//所以在此处选择flag = CALIB_USE_INTRINSIC_GUESS
	double rms = stereoCalibrate(objRealPoint, cornersL, cornersR,
		cameraMatrixL, distCoeffL,
		cameraMatrixR, distCoeffR,
		Size(imageWidth, imageHeight), R, T, E, F,
		0,
		TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 100, 1e-5));

	cout << "Stereo Calibration done with RMS error = " << rms << endl;


	//立体校正的时候需要两幅图像共面并且行对准 以使得立体匹配更加的可靠
	//使得两幅图像共面的方法就是把两个摄像头的图像投影到一个公共成像面上，这样每幅图像从本图像平面投影到公共图像平面都需要一个旋转矩阵R
	//stereoRectify 这个函数计算的就是从图像平面投影都公共成像平面的旋转矩阵Rl,Rr。 Rl,Rr即为左右相机平面行对准的校正旋转矩阵。
	//左相机经过Rl旋转，右相机经过Rr旋转之后，两幅图像就已经共面并且行对准了。
	//其中Pl,Pr为两个相机的投影矩阵，其作用是将3D点的坐标转换到图像的2D点的坐标:P*[X Y Z 1]' =[x y w]
	//Q矩阵为重投影矩阵，即矩阵Q可以把2维平面(图像平面)上的点投影到3维空间的点:Q*[x y d 1] = [X Y Z W]。其中d为左右两幅图像的时差

	stereoRectify(cameraMatrixL, distCoeffL, cameraMatrixR, distCoeffR, imageSize, R, T, Rl, Rr, Pl, Pr, Q,
		CALIB_ZERO_DISPARITY, -1, imageSize, &validROIL, &validROIR);
    

    cout << "R: " << R << endl;
    cout << "T: " << T << endl;
    cout << "Pl: " << Pl << endl;
    cout << "Pr: " << Pr << endl;

    // cout << intrinsic << endl;
    // cout << distortion_coeff << endl;

    // Mat cImage;
    // img = imread("D:\\3.code\\MatlabLIB\\image6\\camera0\\camera0_8.png", IMREAD_GRAYSCALE);
    // undistort(img, cImage, intrinsic, distortion_coeff);

    // namedWindow("out");
    // imshow("out", cImage);
}
