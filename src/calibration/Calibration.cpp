#include "Calibration.h"
#include <Eigen/Core>
#include <opencv2/core/eigen.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <QDir>

using namespace std;
using namespace cv;

//https://github.com/CompileSense/StereoCamera
const int imageWidth = 752; //摄像头的分辨率
const int imageHeight = 480;
const int boardWidth = 9;                         //横向的角点数目
const int boardHeight = 6;                        //纵向的角点数据
const int boardCorner = boardWidth * boardHeight; //总的角点数据
const int squareSize = 25;                        //标定板黑白格子的大小 单位mm
const Size imageSize = Size(imageWidth, imageHeight);

const Size boardSize = Size(boardWidth, boardHeight);
Mat intrinsicL;                   //相机内参数
Mat distortion_coeffL;            //相机畸变参数
vector<Mat> rvecsL;               //旋转向量
vector<Mat> tvecsL;               //平移向量
vector<vector<Point2f>> cornersL; //各个图像找到的角点的集合 和objRealPoint 一一对应

Mat intrinsicR;                   //相机内参数
Mat distortion_coeffR;            //相机畸变参数
vector<Mat> rvecsR;               //旋转向量
vector<Mat> tvecsR;               //平移向量
vector<vector<Point2f>> cornersR; //各个图像找到的角点的集合 和objRealPoint 一一对应

vector<vector<Point3f>> objRealPoint; //各副图像的角点的实际物理坐标集合

vector<Point2f> cornerL; //某一副图像找到的角点
vector<Point2f> cornerR; //某一副图像找到的角点

Mat R, T, E, F;                 //R 旋转矢量 T平移矢量 E本征矩阵 F基础矩阵
Mat Rl, Rr, Pl, Pr, Q;          //校正旋转矩阵R，投影矩阵P 重投影矩阵Q (下面有具体的含义解释）
Mat mapLx, mapLy, mapRx, mapRy; //映射表
Rect validROIL, validROIR;      //图像校正之后，会对图像进行裁剪，这里的validROI就是指裁剪之后的区域

//相机内参矩阵
Mat cameraMatrixL;
Mat distCoeffL;
Mat cameraMatrixR;
Mat distCoeffR;

Matrix34d PS[CAM_NUM_MAX];
Matrix33d RS[CAM_NUM_MAX];
RowVector3d TS[CAM_NUM_MAX];

Calibration::Calibration()
{
}

/*计算标定板上模块的实际物理坐标*/
static void calRealPoint(vector<vector<Point3f>> &obj, int boardwidth, int boardheight, int imgNumber, int squaresize)
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

static QStringList checkFile(QString path)
{
    char buff[100];
    QString dirPath;
    QStringList nameFilters;
    nameFilters << "*.png";
    QStringList files;
    for (int i = 0; i < setting->camNumber; i++)
    {
        dirPath = path + "/camera" + QString::number(i);
        if (EasyTool::isDirExist(dirPath) == false)
        {
            mlog->show("calibr check not found " + dirPath + ",exit");
            return files;
        }
        else
        {
            if (i == 0)
            {
                QDir dir(dirPath);
                files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
                if (files.size() < 1)
                {
                    mlog->show("calibr check error,files.size < 1,exit");
                }
            }
            else
            {
                for (int ii = 0; ii < files.size(); ii++)
                {
                    if (EasyTool::isFileExist(dirPath + "/" + files.at(ii)) == false)
                    {
                        mlog->show("calibr check not found " + dirPath + "/" + files.at(ii) + ",exit");
                        return files;
                    }
                }
            }
        }
    }
    return files;
}

void Calibration::calibrStart(QString path)
{
    int goodFrameCount = 0;
    Mat imgL, boardimgL;
    Mat imgR, boardimgR;

    Mat img;

    vector<QMap<uint8_t, vector<Point2f>>> camcorners;

    //指定亚像素计算迭代标注
    cv::TermCriteria criteria = cv::TermCriteria(
        cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS,
        40,
        0.01);

    QStringList files = checkFile(path);
    if (files.size() == 0)
    {
        return;
    }

    for (int i = 0; i < setting->camNumber; i++)
    //for (int i = 0; i < 1; i++)
    {
        QMap<uint8_t, vector<Point2f>> corners;
        for (int ii = 0; ii < files.size(); ii++)
        {
            QString imgPath = path + "/camera" + QString::number(i) + "/" + files.at(ii);
            if (EasyTool::isFileExist(imgPath) == false)
            {
                mlog->show("calibr not not found " + imgPath + ",exit");
                return;
            }
            img = imread(string((const char *)imgPath.toLocal8Bit()), IMREAD_GRAYSCALE);
            if (img.data)
            {
                vector<Point2f> corner;

                bool isFind = findChessboardCornersSB(img, boardSize, corner,
                                                      CALIB_CB_NORMALIZE_IMAGE | CALIB_CB_EXHAUSTIVE | CALIB_CB_ACCURACY);
                if (isFind)
                {
                    //cornerSubPix(img, corner, Size(5, 5), Size(-1, -1), criteria);
                    //在图像上画出角点
                    //drawChessboardCorners(img, boardSize, corner, isFind);
                    corners.insert(ii, corner);
                    mlog->show("findChessboardCorners " + imgPath + " success " + QString::number(corner.size()));
                    // namedWindow(string((const char *)imgPath.toLocal8Bit()));
                    // imshow(string((const char *)imgPath.toLocal8Bit()), img);
                }
                else
                {
                    mlog->show("findChessboardCorners " + imgPath + " fail");
                }
            }
        }
        camcorners.push_back(corners);
    }

    mlog->show("camcorners size " + QString::number(camcorners.size()));

    if (camcorners.size() != setting->camNumber)
    {
        return;
    }

    for (int i = 0; i < camcorners.size(); i++)
    {
        mlog->show("corners " + QString::number(i) + " : " + QString::number(camcorners.at(i).size()));
    }

    for (int i = 0; i < camcorners.size() - 1; i++)
    {
        cornersL.clear();
        cornersR.clear();
        for (int ii = 0; ii < files.size(); ii++)
        {
            if (camcorners.at(i).contains(ii) && camcorners.at(i + 1).contains(ii))
            {
                cornersL.push_back(camcorners.at(i).find(ii).value());
                cornersR.push_back(camcorners.at(i + 1).find(ii).value());
            }
        }

        mlog->show("cailbr : camera" + QString::number(i) + " to camera" + QString::number(i + 1));
        goodFrameCount = cornersL.size();

        /*计算实际的校正点的三维坐标*/
        objRealPoint.clear();
        calRealPoint(objRealPoint, boardWidth, boardHeight, goodFrameCount, squareSize);

        // calibrateCamera(objRealPoint, cornersL, Size(boardWidth, boardHeight), intrinsicL, distortion_coeffL, rvecsL, tvecsL, 0);
        // calibrateCamera(objRealPoint, cornersR, Size(boardWidth, boardHeight), intrinsicR, distortion_coeffR, rvecsR, tvecsR, 0);

        //标定摄像头
        float rms = stereoCalibrate(objRealPoint, cornersL, cornersR,
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

        if (i == 0)
        {
            RS[0] << 1, 0, 0,
                0, 1, 0,
                0, 0, 1;
            TS[0] << (double)0.0, (double)0.0, (double)0.0;
            cv2eigen(Pl, PS[i]);
        }

        cv2eigen(R, RS[i+1]);
        cv2eigen(T.t() , TS[i+1]);
        cv2eigen(Pr, PS[i+1]);
    }

    for (int i = 0; i < camcorners.size() - 1; i++)
    {
        cout << std::setprecision (16) << "R" << i << ": " << RS[i] << endl;
        cout << std::setprecision (16) << "T" << i << ": " << TS[i] << endl;
        cout << std::setprecision (16) << "P" << i << ": " << PS[i] << endl;
    }

}
