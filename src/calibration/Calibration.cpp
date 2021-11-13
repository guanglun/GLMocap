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
#include <QTime>

using namespace std;
using namespace cv;

extern struct VISION_PARAM vision_param;

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
vector<Mat> intrinsics, distortion_coeffs;

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
Vector3d TS[CAM_NUM_MAX];

vector<vector<Point2f>> corners; //各个图像找到的角点的集合 和objRealPoint 一一对应
vector<float> vrms;

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
            imgpoint.push_back(Point3f(colIndex * squaresize, rowIndex * squaresize, 0));
        }
    }
    for (int imgIndex = 0; imgIndex < imgNumber; imgIndex++)
    {
        obj.push_back(imgpoint);
    }
}

QStringList Calibration::checkFile(QString path)
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
            msg("calibr check not found " + dirPath + ",exit");
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
                    msg("calibr check error,files.size < 1,exit");
                }
            }
            else
            {
                for (int ii = 0; ii < files.size(); ii++)
                {
                    if (EasyTool::isFileExist(dirPath + "/" + files.at(ii)) == false)
                    {
                        msg("calibr check not found " + dirPath + "/" + files.at(ii) + ",exit");
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
    Matrix33d cameraMatrix;
    Mat img;
    Matrix43d PSTMP;
    Matrix43d RTS43d;

    vector<QMap<uint8_t, vector<Point2f>>> camcorners;

    //指定亚像素计算迭代标注
    cv::TermCriteria criteria = cv::TermCriteria(
        cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS,
        300, 0.01);

    msg("Calibration Start");

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
                msg("calibr not not found " + imgPath + ",exit");
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

                    corners.insert(ii, corner);
                    msg("findChessboardCorners " + imgPath + " success " + QString::number(corner.size()));

                    // if(i == 0)
                    // {
                    //     drawChessboardCorners(img, boardSize, corner, isFind);
                    //     namedWindow(string((const char *)imgPath.toLocal8Bit()));
                    //     imshow(string((const char *)imgPath.toLocal8Bit()), img);
                    // }
                }
                else
                {
                    msg("findChessboardCorners " + imgPath + " fail");
                }
            }
        }
        camcorners.push_back(corners);
    }

    msg("camcorners size " + QString::number(camcorners.size()));

    if (camcorners.size() != setting->camNumber)
    {
        return;
    }

    for (int i = 0; i < camcorners.size(); i++)
    {
        msg("corners " + QString::number(i) + " : " + QString::number(camcorners.at(i).size()));
    }

    intrinsics.clear();
    distortion_coeffs.clear();

    for (int i = 0; i < camcorners.size(); i++)
    {
        goodFrameCount = camcorners.at(i).size();
        /*计算实际的校正点的三维坐标*/
        objRealPoint.clear();
        calRealPoint(objRealPoint, boardWidth, boardHeight, goodFrameCount, squareSize);
        corners.clear();
        for (int ii = 0; ii < files.size(); ii++)
        {
            if (camcorners.at(i).contains(ii))
            {
                corners.push_back(camcorners.at(i).find(ii).value());
            }
        }
        Mat intrinsic, distortion_coeff;
        vector<Mat> rvecs; //旋转向量
        vector<Mat> tvecs; //平移向量
        Matrix33d cami;
        Matrix<double, 1, 5> camk;

        double err = calibrateCamera(objRealPoint, corners, imageSize, intrinsic, distortion_coeff, rvecs, tvecs);

        cv2eigen(intrinsic, cami);
        cv2eigen(distortion_coeff, camk);
        //msg("corners size" + QString::number(i) + ": \r\n" + QString::number(corners.size()));
        msg("intrinsic" + QString::number(i) + ": \r\n" + EasyTool::MatToString(cami));
        msg("distortion_coeff" + QString::number(i) + ": \r\n" + EasyTool::MatToString(camk));
        msg("cailbr : camera" + QString::number(i) + " error : " + QString::number(err));

        intrinsics.push_back(intrinsic);
        distortion_coeffs.push_back(distortion_coeff);
    }

    vrms.clear();
    for (int i = 0; i < camcorners.size() - 1; i++)
    {
        cornersL.clear();
        cornersR.clear();
        for (int ii = 0; ii < files.size(); ii++)
        {
            if (camcorners.at(0).contains(ii) && camcorners.at(i + 1).contains(ii))
            {
                cornersL.push_back(camcorners.at(0).find(ii).value());
                cornersR.push_back(camcorners.at(i + 1).find(ii).value());
            }
        }

        msg("cailbr : camera" + QString::number(0) + " to camera" + QString::number(i + 1) + " start");
        goodFrameCount = cornersL.size();

        /*计算实际的校正点的三维坐标*/
        objRealPoint.clear();
        calRealPoint(objRealPoint, boardWidth, boardHeight, goodFrameCount, squareSize);

        QTime time;
        time.start();

        // calibrateCamera(objRealPoint, cornersL, Size(boardWidth, boardHeight), intrinsicL, distortion_coeffL, rvecsL, tvecsL, 0);
        // calibrateCamera(objRealPoint, cornersR, Size(boardWidth, boardHeight), intrinsicR, distortion_coeffR, rvecsR, tvecsR, 0);

        //标定摄像头
        float rms = stereoCalibrate(objRealPoint, cornersL, cornersR,
                                    intrinsics.at(0), distortion_coeffs.at(0),
                                    intrinsics.at(i + 1), distortion_coeffs.at(i + 1),
                                    imageSize, R, T, E, F,
                                    CALIB_FIX_INTRINSIC,
                                    TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 1000, 1e-20));

        // float rms = stereoCalibrate(objRealPoint, cornersL, cornersR,
        //                             intrinsics.at(0), distortion_coeffs.at(0),
        //                             intrinsics.at(i + 1), distortion_coeffs.at(i + 1),
        //                             Size(imageWidth, imageHeight), R, T, E, F,
        //                             0,
        //                             TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 1000, 1e-20));

        vrms.push_back(rms);

        //立体校正的时候需要两幅图像共面并且行对准 以使得立体匹配更加的可靠
        //使得两幅图像共面的方法就是把两个摄像头的图像投影到一个公共成像面上，这样每幅图像从本图像平面投影到公共图像平面都需要一个旋转矩阵R
        //stereoRectify 这个函数计算的就是从图像平面投影都公共成像平面的旋转矩阵Rl,Rr。 Rl,Rr即为左右相机平面行对准的校正旋转矩阵。
        //左相机经过Rl旋转，右相机经过Rr旋转之后，两幅图像就已经共面并且行对准了。
        //其中Pl,Pr为两个相机的投影矩阵，其作用是将3D点的坐标转换到图像的2D点的坐标:P*[X Y Z 1]' =[x y w]
        //Q矩阵为重投影矩阵，即矩阵Q可以把2维平面(图像平面)上的点投影到3维空间的点:Q*[x y d 1] = [X Y Z W]。其中d为左右两幅图像的时差

        // stereoRectify(
        // intrinsics.at(0), distortion_coeffs.at(0),
        // intrinsics.at(i + 1), distortion_coeffs.at(i + 1), imageSize, R, T, Rl, Rr, Pl, Pr, Q,
        //               CALIB_ZERO_DISPARITY, -1, imageSize, &validROIL, &validROIR);

        // cout << "R: " << R << endl;
        // cout << "T: " << T << endl;
        // cout << "Pl: " << Pl << endl;
        // cout << "Pr: " << Pr << endl;
        // cout << "intrinsics: " << intrinsics.at(i) << endl;
        // cout << "distortion_coeffs: " << distortion_coeffs.at(i) << endl;

        if (i == 0)
        {
            RS[0] << 1, 0, 0,
                0, 1, 0,
                0, 0, 1;
            TS[0] << (double)0.0, (double)0.0, (double)0.0;
            RS[0].transposeInPlace();
            RTS43d = EasyTool::getRT43d(RS[0], TS[0]);
            cv2eigen(intrinsics.at(0), cameraMatrix);
            PS[0] = (RTS43d * cameraMatrix.transpose()).transpose();
        }

        cv2eigen(R, RS[i + 1]);
        cv2eigen(T, TS[i + 1]);
        RS[i + 1].transposeInPlace();

        RTS43d = EasyTool::getRT43d(RS[i + 1], TS[i + 1]);
        cv2eigen(intrinsics.at(i + 1), cameraMatrix);
        PS[i + 1] = (RTS43d * cameraMatrix.transpose()).transpose();

        if (i == 0)
        {
            vision_param.R[i] << RS[i];
            vision_param.T[i] << TS[i];
            vision_param.P[i] << PS[i];

            msg("P" + QString::number(i) + ": \r\n" + EasyTool::MatToString(vision_param.P[i]));
            msg("R" + QString::number(i) + ": \r\n" + EasyTool::MatToString(vision_param.R[i]));
            msg("T" + QString::number(i) + ": \r\n" + EasyTool::MatToString(vision_param.T[i]));

            // cout << std::setprecision(16) << "P" << i << ": " << vision_param.P[i] << endl;
            // cout << std::setprecision(16) << "R" << i << ": " << vision_param.R[i] << endl;
            // cout << std::setprecision(16) << "T" << i << ": " << vision_param.T[i] << endl;
        }

        vision_param.R[i + 1] << RS[i + 1];
        vision_param.T[i + 1] << TS[i + 1];
        vision_param.P[i + 1] << PS[i + 1];

        msg("P" + QString::number(i + 1) + ": \r\n" + EasyTool::MatToString(vision_param.P[i + 1]));
        msg("R" + QString::number(i + 1) + ": \r\n" + EasyTool::MatToString(vision_param.R[i + 1]));
        msg("T" + QString::number(i + 1) + ": \r\n" + EasyTool::MatToString(vision_param.T[i + 1]));

        // cout << std::setprecision(16) << "P" << i + 1 << ": " << vision_param.P[i + 1] << endl;
        // cout << std::setprecision(16) << "R" << i + 1 << ": " << vision_param.R[i + 1] << endl;
        // cout << std::setprecision(16) << "T" << i + 1 << ": " << vision_param.T[i + 1] << endl;

        msg("take time " + QString::number(time.elapsed() / 1000.0) + "s");
        msg("Stereo Calibration done with RMS error = " + QString::number(rms, 'f', 6));
    }

    for (int i = 0; i < camcorners.size() - 1; i++)
    {
        msg(QString::number(0) + " to " + QString::number(i + 1) + " rms error = " + QString::number(vrms.at(i), 'f', 6));
    }

    msg("Calibration Exit");
}

void Calibration::msg(QString msg)
{
    mlog->show(msg);
    emit logSignal(msg);
}
