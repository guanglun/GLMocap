#include "VisionProcess.h"
#include "CameraPointMap.h"

#include <QElapsedTimer>

VisionProcess::VisionProcess(PX4Thread *px4, QObject *parent)
{
    this->px4 = px4;
    mm = new MarksManagement();
}

void VisionProcess::init(int camNum)
{
    this->camNum = camNum;
}

static int getIndex(double input[], int size, double search)
{
    for (int i = 0; i < size; i++)
    {
        if (input[i] == search)
        {
            return i;
        }
    }
    return -1;
}

static void eraseData(vector<double> *rerrSort, double eraseData)
{
    for (int i = 0; i < rerrSort->size(); i++)
    {
        if ((*rerrSort)[i] == eraseData)
        {
            rerrSort->erase(rerrSort->begin() + i);
            break;
        }
    }
}

static void removeOther(int camNum, MatrixXi map, int indexResult[], vector<double> *rerrSort, double rerr[])
{
    for (int pm = 0; pm < map.rows(); pm++)
    {
        for (int cm = 0; cm < camNum; cm++)
        {
            if (indexResult[0] != pm)
            {
                if (map.row(indexResult[0])(cm) == map.row(pm)(cm))
                {
                    //DBG("rerrSort size %d remove pm : %d",rerrSort->size(),pm);
                    //rerrSort->erase(rerrSort->begin() + pm);
                    eraseData(rerrSort, rerr[pm]);
                    break;
                }
            }
        }
    }
}

int VisionProcess::checkVPointSize(void)
{
    if (camResult[0].vPoint.size() == 0)
        return -1;

    for (int i = 0; i < camNum - 1; i++)
    {
        if (camResult[i].vPoint.size() != camResult[i + 1].vPoint.size())
        {
            //DBG("Error Return, vPoint size not match");
            return -1;
        }
    }
    return camResult[0].vPoint.size();
}

int VisionProcess::matchPoint(void)
{

    pointNum = checkVPointSize();

    if (pointNum < 1)
    {
        //DBG("Error Return, pointNum < 1");
        return -1;
    }
    //DBG("pointNum: %d",pointNum);

    CameraPointMap cpMap;
    MatrixXi map = cpMap.getPointMap(camNum, pointNum);
    int indexResult[pointNum];
    //std::cout << map << std::endl;

    MatrixXd xy[map.rows()];
    MatrixXd xyc[map.rows()];
    Vector3d Xr[map.rows()];
    double rerr[map.rows()];
    vector<double> rerrSort;

    vector<GLPoint *> vPoint[camNum];

    MatrixXi idx(map.rows(), camNum);

    for (int i = 0; i < map.rows(); i++)
    {
        for (int cm = 0; cm < camNum; cm++)
        {

            xy[i].resize(2, camNum);
            xyc[i].resize(2, camNum);
            xy[i].col(cm)(0) = camResult[cm].vPoint[map.row(i)(cm)]->x;
            xy[i].col(cm)(1) = camResult[cm].vPoint[map.row(i)(cm)]->y;
            idx.row(i)(cm) = 1;
        }
    }

    QElapsedTimer mstimer; // 定义对象
    mstimer.start();       // 调用函数

    multipleViewTriangulation.getRMS(vision_param.P,
                                     camNum,
                                     xy,
                                     Xr,
                                     map.rows(),
                                     rerr);

    mlog->show("get rms take time " + QString::number((double)mstimer.nsecsElapsed() / (double)1000) + "us");

    std::cout << "===>>>result rms :\r\n";
    for (int i = 0; i < map.rows(); i++)
    {
        std::cout << i << " : " << map.row(i) << " " << rerr[i] << endl;
        rerrSort.push_back(rerr[i]);
    }

    // multipleViewTriangulation.optimal_correction_all(vision_param.P,
    //                                                  camNum,
    //                                                  xy,
    //                                                  xyc,
    //                                                  idx,
    //                                                  rerr, map.rows());
    // std::cout << "===>>>result1:\r\n";
    // for (int i = 0; i < map.rows(); i++)
    // {
    //     std::cout << i << " : " << map.row(i) << " ";
    //     mlog->show(QString::number(rerr[i]));
    //     if (rerr[i] != -1)
    //     {
    //         rerrSort.push_back(rerr[i]);
    //     }
    // }
    if (rerrSort.size() < pointNum)
    {
        DBG("rerrSort size Error, Return");
        return -1;
    }

    sort(rerrSort.begin(), rerrSort.end());

    //for debug
    for (int pm; pm < rerrSort.size(); pm++)
    {
        DBG("sort index pm : %d \t%f", getIndex(rerr, map.rows(), rerrSort[pm]), rerrSort[pm]);
    }

    for (int pm; pm < pointNum; pm++)
    {
        indexResult[pm] = getIndex(rerr, map.rows(), rerrSort[pm]);
        DBG("index pm : %d", indexResult[pm]);
        if (indexResult[pm] == -1)
        {
            DBG("Error Return, indexResult[%d] == -1", pm);
            return -1;
        }

        removeOther(camNum, map, &indexResult[pm], &rerrSort, rerr);
    }
    std::cout << "===>>>result2:\r\n";
    for (int pm = 0; pm < pointNum; pm++)
    {
        int index = getIndex(rerr, map.rows(), rerrSort[pm]);
        std::cout << index << " : " << map.row(index) << " ";
        mlog->show(QString::number(rerr[index]));

        for (int cm = 0; cm < camNum; cm++)
        {
            camResult[cm].vPoint[map.row(index)(cm)]->id = pm;
            vPoint[cm].push_back(new GLPoint(
                camResult[cm].vPoint[map.row(index)(cm)]->state,
                camResult[cm].vPoint[map.row(index)(cm)]->imageIndex,
                camResult[cm].vPoint[map.row(index)(cm)]->x,
                camResult[cm].vPoint[map.row(index)(cm)]->y,
                pm));

            std::cout << "\r\n\t" << camResult[cm].vPoint[map.row(index)(cm)]->x << "\t" << camResult[cm].vPoint[map.row(index)(cm)]->y << "\r\n";
        }
    }

    // for (int cm = 0; cm < camNum; cm++)
    // {
    //     camResult[cm].vPoint.clear();
    //     for (int pm = 0; pm < pointNum; pm++)
    //     {

    //         camResult[cm].vPoint.push_back(new GLPoint(
    //             vPoint[cm][pm]->state,
    //             vPoint[cm][pm]->imageIndex,
    //             vPoint[cm][pm]->x,
    //             vPoint[cm][pm]->y,
    //             pm));
    //     }
    // }

    if (calGNDstate == CAL_START)
    {
        if (calibrateGND(vPoint) < 0)
        {
            return -1;
        }
        else
        {
            calGNDstate = CAL_OK;
        }
    }
    else if (findDroneState == FIND_MODULE_START)
    {

        if (findDroneModule(vPoint) < 0)
        {
            return -1;
        }
        else
        {
            findDroneState = FIND_MODULE_OK;
        }
    }

    return 0;
}

void swapid(int *id1, int *id2)
{
    int id = *id2;
    *id2 = *id1;
    *id1 = id;
}

void swapVPoint(GLPoint *p1, GLPoint *p2)
{
    swapid(&p1->id, &p2->id);
    swap(p1, p2);
}

int VisionProcess::findDroneModule(vector<GLPoint *> *vPoint)
{
    if (pointNum != 3)
    {
        DBG("calibrateGND Error Return, pointNum != 3");
        return -1;
    }

    Vector3d *Xr = triangulation();
    vector<double> dis;

    dis.push_back(multipleViewTriangulation.distance3d(
        cv::Point3d(Xr[0](0, 0), Xr[0](1, 0), Xr[0](2, 0)),
        cv::Point3d(Xr[1](0, 0), Xr[1](1, 0), Xr[1](2, 0))));

    dis.push_back(multipleViewTriangulation.distance3d(
        cv::Point3d(Xr[0](0, 0), Xr[0](1, 0), Xr[0](2, 0)),
        cv::Point3d(Xr[2](0, 0), Xr[2](1, 0), Xr[2](2, 0))));

    dis.push_back(multipleViewTriangulation.distance3d(
        cv::Point3d(Xr[1](0, 0), Xr[1](1, 0), Xr[1](2, 0)),
        cv::Point3d(Xr[2](0, 0), Xr[2](1, 0), Xr[2](2, 0))));

    mlog->show("dis: " + QString::number(dis[0]) + " " + QString::number(dis[1]) + " " + QString::number(dis[2]));

    if (dis[2] < dis[1] && dis[1] < dis[0])
    {
        for (int cm = 0; cm < camNum; cm++)
            swapVPoint(vPoint[cm][1], vPoint[cm][2]);
    }
    else if (dis[1] < dis[0] && dis[0] < dis[2])
    {
        for (int cm = 0; cm < camNum; cm++)
            swapVPoint(vPoint[cm][1], vPoint[cm][0]);
    }
    else if (dis[0] < dis[2] && dis[2] < dis[1])
    {
        for (int cm = 0; cm < camNum; cm++)
            swapVPoint(vPoint[cm][0], vPoint[cm][2]);
    }
    else if (dis[0] < dis[1] && dis[1] < dis[2])
    {
        for (int cm = 0; cm < camNum; cm++)
        {
            swapVPoint(vPoint[cm][0], vPoint[cm][2]);
            swapVPoint(vPoint[cm][1], vPoint[cm][0]);
        }
    }
    else if (dis[1] < dis[2] && dis[2] < dis[0])
    {
        for (int cm = 0; cm < camNum; cm++)
        {
            swapVPoint(vPoint[cm][0], vPoint[cm][1]);
            swapVPoint(vPoint[cm][0], vPoint[cm][2]);
        }
    }

    for (int cm = 0; cm < camNum; cm++)
    {
        for (int pm = 0; pm < pointNum; pm++)
        {
            camResult[cm].vPoint[pm]->state = vPoint[cm][pm]->state;
            camResult[cm].vPoint[pm]->imageIndex = vPoint[cm][pm]->imageIndex;
            camResult[cm].vPoint[pm]->x = vPoint[cm][pm]->x;
            camResult[cm].vPoint[pm]->y = vPoint[cm][pm]->y;
            camResult[cm].vPoint[pm]->id = vPoint[cm][pm]->id;
        }
        vPoint[cm].clear();
    }

    dis.clear();
    return 0;
}

int VisionProcess::calibrateGND(vector<GLPoint *> *vPoint)
{
    if (pointNum != 3)
    {
        DBG("calibrateGND Error Return, pointNum != 3");
        return -1;
    }

    Vector3d *Xr = triangulation();
    vector<double> dis;

    dis.push_back(multipleViewTriangulation.distance3d(
        cv::Point3d(Xr[0](0, 0), Xr[0](1, 0), Xr[0](2, 0)),
        cv::Point3d(Xr[1](0, 0), Xr[1](1, 0), Xr[1](2, 0))));

    dis.push_back(multipleViewTriangulation.distance3d(
        cv::Point3d(Xr[0](0, 0), Xr[0](1, 0), Xr[0](2, 0)),
        cv::Point3d(Xr[2](0, 0), Xr[2](1, 0), Xr[2](2, 0))));

    dis.push_back(multipleViewTriangulation.distance3d(
        cv::Point3d(Xr[1](0, 0), Xr[1](1, 0), Xr[1](2, 0)),
        cv::Point3d(Xr[2](0, 0), Xr[2](1, 0), Xr[2](2, 0))));

    mlog->show("dis: " + QString::number(dis[0]) + " " + QString::number(dis[1]) + " " + QString::number(dis[2]));

    if (dis[2] < dis[1] && dis[1] < dis[0])
    {
        for (int cm = 0; cm < camNum; cm++)
            swapVPoint(vPoint[cm][1], vPoint[cm][2]);
    }
    else if (dis[1] < dis[0] && dis[0] < dis[2])
    {
        for (int cm = 0; cm < camNum; cm++)
            swapVPoint(vPoint[cm][1], vPoint[cm][0]);
    }
    else if (dis[0] < dis[2] && dis[2] < dis[1])
    {
        for (int cm = 0; cm < camNum; cm++)
            swapVPoint(vPoint[cm][0], vPoint[cm][2]);
    }
    else if (dis[0] < dis[1] && dis[1] < dis[2])
    {
        for (int cm = 0; cm < camNum; cm++)
        {
            swapVPoint(vPoint[cm][0], vPoint[cm][2]);
            swapVPoint(vPoint[cm][1], vPoint[cm][0]);
        }
    }
    else if (dis[1] < dis[2] && dis[2] < dis[0])
    {
        for (int cm = 0; cm < camNum; cm++)
        {
            swapVPoint(vPoint[cm][0], vPoint[cm][1]);
            swapVPoint(vPoint[cm][0], vPoint[cm][2]);
        }
    }

    for (int cm = 0; cm < camNum; cm++)
    {
        for (int pm = 0; pm < pointNum; pm++)
        {
            camResult[cm].vPoint[pm]->state = vPoint[cm][pm]->state;
            camResult[cm].vPoint[pm]->imageIndex = vPoint[cm][pm]->imageIndex;
            camResult[cm].vPoint[pm]->x = vPoint[cm][pm]->x;
            camResult[cm].vPoint[pm]->y = vPoint[cm][pm]->y;
            camResult[cm].vPoint[pm]->id = vPoint[cm][pm]->id;
        }
        vPoint[cm].clear();
    }

    Xr = triangulation();

    for (int cm = 0; cm < pointNum; cm++)
    {
        mlog->show(
            QString::number(cm) + " : " +
            QString::number(Xr[cm](0, 0)) + " " +
            QString::number(Xr[cm](1, 0)) + " " +
            QString::number(Xr[cm](2, 0)));
    }

    // mlog->show("rel : " +
    //         QString::number(tar[0]) + " " +
    //         QString::number(tar[1]) + " " +
    //         QString::number(tar[2]));

    // double tar[3] = {
    //     Xr[0](0, 0)-Xr[1](0, 0),
    //     Xr[0](1, 0)-Xr[1](1, 0),
    //     Xr[0](2, 0)-Xr[1](2, 0),
    // };

    // mlog->show("tar : " +
    //         QString::number(tar[0]) + " " +
    //         QString::number(tar[1]) + " " +
    //         QString::number(tar[2]));

    //https://zhuanlan.zhihu.com/p/93563218

    // Eigen::Vector3d rvec (tar[0], tar[1], tar[2]);
    // double n_norm = rvec.norm();
    // Eigen::AngleAxisd rotation_vector (n_norm, rvec/n_norm);

    // vision_param.RGND = rotation_vector.toRotationMatrix();

    // std::cout << vision_param.RGND << std::endl;

    // //2.1 旋转矩阵转换为欧拉角
    // //ZYX顺序，即先绕x轴roll,再绕y轴pitch,最后绕z轴yaw,0表示X轴,1表示Y轴,2表示Z轴
    // vision_param.eulerAngles = vision_param.RGND.eulerAngles(0, 1, 2);
    // std::cout << vision_param.eulerAngles << endl;

    // vision_param.TGND << Xr[0](0, 0), Xr[0](1, 0), Xr[0](2, 0);

    std::vector<cv::Point3f> srcPoints;
    std::vector<cv::Point3f> dstPoints;

    srcPoints.push_back(cv::Point3f(140, 0, 0));
    srcPoints.push_back(cv::Point3f(0, 0, 0));
    srcPoints.push_back(cv::Point3f(0, -80, 0));

    for (int cm = 0; cm < pointNum; cm++)
    {
        dstPoints.push_back(cv::Point3f(Xr[cm](0, 0), Xr[cm](1, 0), Xr[cm](2, 0)));
    }

    cv::Mat RT = MultipleViewTriangulation::Get3DR_TransMatrix(srcPoints, dstPoints).inv();

    vision_param.RGND << RT.at<double>(0, 0), RT.at<double>(0, 1), RT.at<double>(0, 2),
        RT.at<double>(1, 0), RT.at<double>(1, 1), RT.at<double>(1, 2),
        RT.at<double>(2, 0), RT.at<double>(2, 1), RT.at<double>(2, 2);

    vision_param.TGND << Xr[1](0, 0), Xr[1](1, 0), Xr[1](2, 0); //RT.at<double>(0, 3),RT.at<double>(1, 3),RT.at<double>(2, 3);

    Matrix44d RTGND;
    RTGND << vision_param.RGND(0, 0), vision_param.RGND(1, 0), vision_param.RGND(2, 0), vision_param.TGND(0, 0),
        vision_param.RGND(0, 1), vision_param.RGND(1, 1), vision_param.RGND(2, 1), vision_param.TGND(1, 0),
        vision_param.RGND(0, 2), vision_param.RGND(1, 2), vision_param.RGND(2, 2), vision_param.TGND(2, 0),
        0, 0, 0, 1;

    vision_param.RTGNDINV = RTGND.inverse();

    // std::cout << vision_param.RGND << endl;
    // std::cout << vision_param.TGND << endl;

    vision_param.eulerAngles = vision_param.RGND.transpose().eulerAngles(0, 1, 2);
    //std::cout << vision_param.eulerAngles << endl;

    setting->saveGNDVisionParam();

    dis.clear();

    return 0;
}

void VisionProcess::positionSlot(CAMERA_RESULT result)
{
    bool isFirst = true;
    bool check = true;
    int foundNum = 0;

    camResult[result.camIndex] = result;

    mlog->show("index: " + QString::number(result.camIndex) +
               " ptnum: " + QString::number(result.pointNum) +
               " time: " + QString::number(result.time) +
               " diff: " + QString::number(result.time - lastTime[result.camIndex]));

    lastTime[result.camIndex] = result.time;

    if (camNum < 2)
        return;

    for (int i = 0; i < (camNum - 1); i++)
    {
        if (qAbs(camResult[0].time - camResult[i + 1].time) > 4)
        {
            check = false;
            break;
        }
    }

    if (check == true)
    {
        mm->catchDone(camNum);
    }

    return;

    if (check == true)
    {
        //mlog->show("===>>>" + QString::number(camResult[0].time) + " " + QString::number(camResult[0].time - lastAllTime));
        lastAllTime = camResult[0].time;

        if (isCapImage == true)
        {
            if (camResult[0].time - saveLastTime >= 1000)
            {
                for (int i = 0; i < camNum; i++)
                {
                    QString filePath = camResult[i].path + "/" + QString::number(camResult[0].time) + ".png";

                    camResult[i].image.save(filePath);
                    mlog->show("save " + filePath);
                }
                saveLastTime = camResult[0].time;
            }
        }
        else
        {
            count++;
            // for (int i = 0; i < camNum; i++)
            // {
            //     if (camResult[i].pointNum <= 0)
            //     {
            //         return;
            //     }
            // }

            // vision_param.CamNum = camNum;
            // vision_param.ptNum = 1;

            //vision_param.xy[0].col(0)(0) = 10;

            if (matchState == MATCH_START)
            {
                qint64 t1, t2;
                t1 = QDateTime::currentDateTime().toMSecsSinceEpoch();
                matchState = MATCH_ING;
                if (matchPoint() == 0)
                {
                    matchState = MATCH_OK;
                }
                else
                {
                    matchState = MATCH_START;
                }
                // if (foundNum >= 3)
                // {
                //     multipleViewTriangulation.triangulation();
                // }

                t2 = QDateTime::currentDateTime().toMSecsSinceEpoch();
                //mlog->show(" diff " + QString::number(t2 - t1));
            }
            else if (matchState == MATCH_OK)
            {
                if (calGNDstate == CAL_OK && findDroneState != FIND_MODULE_OK)
                {
                    Vector3d *Xr = triangulation();
                    for (int pm = 0; pm < vision_param.ptNum; pm++)
                    {
                        Matrix<double, 4, 1> PT;
                        PT << Xr[pm](0, 0), Xr[pm](1, 0), Xr[pm](2, 0), 1;
                        PT = vision_param.RTGNDINV * PT;
                        Xr[pm] << PT(0, 0), PT(1, 0), PT(2, 0);
                    }
                    // mlog->show("pos : " +
                    //            QString::number(Xr[0](0, 0)) + " " +
                    //            QString::number(Xr[0](1, 0)) + " " +
                    //            QString::number(Xr[0](2, 0)));

                    emit onXYZSignals(Xr, &eulerAnglesDrone, vision_param.ptNum);
                }
                else if (findDroneState == FIND_MODULE_OK)
                {
                    Vector3d *Xr = triangulation();
                    for (int pm = 0; pm < vision_param.ptNum; pm++)
                    {
                        Matrix<double, 4, 1> PT;
                        PT << Xr[pm](0, 0), Xr[pm](1, 0), Xr[pm](2, 0), 1;
                        PT = vision_param.RTGNDINV * PT;
                        Xr[pm] << PT(0, 0), PT(1, 0), PT(2, 0);
                    }

                    // mlog->show("pis : " +
                    //            QString::number(Xr[0](0, 0)) + " " +
                    //            QString::number(Xr[0](1, 0)) + " " +
                    //            QString::number(Xr[0](2, 0)) + " " +
                    //            QString::number(Xr[1](0, 0)) + " " +
                    //            QString::number(Xr[1](1, 0)) + " " +
                    //            QString::number(Xr[1](2, 0)) + " " +
                    //            QString::number(Xr[2](0, 0)) + " " +
                    //            QString::number(Xr[2](1, 0)) + " " +
                    //            QString::number(Xr[2](2, 0)));

                    std::vector<cv::Point3f> srcPoints;
                    std::vector<cv::Point3f> dstPoints;

                    srcPoints.push_back(cv::Point3f(80, 0, 0));
                    srcPoints.push_back(cv::Point3f(0, 0, 0));
                    srcPoints.push_back(cv::Point3f(0, -60, 0));

                    for (int cm = 0; cm < pointNum; cm++)
                    {
                        dstPoints.push_back(cv::Point3f(Xr[cm](0, 0), Xr[cm](1, 0), Xr[cm](2, 0)));
                    }

                    cv::Mat RT = MultipleViewTriangulation::Get3DR_TransMatrix(srcPoints, dstPoints); //.inv();

                    Matrix3d R_Drone;
                    Vector3d T_Drone;

                    R_Drone << RT.at<double>(0, 0), RT.at<double>(0, 1), RT.at<double>(0, 2),
                        RT.at<double>(1, 0), RT.at<double>(1, 1), RT.at<double>(1, 2),
                        RT.at<double>(2, 0), RT.at<double>(2, 1), RT.at<double>(2, 2);
                    R_Drone = R_Drone.transpose().eval();
                    //eulerAnglesDrone = R_Drone.eulerAngles(2, 1, 0);// * 180 / M_PI;

                    eulerAnglesDrone = MultipleViewTriangulation::rotationMatrixToEulerAngles(R_Drone);

                    T_Drone << Xr[1](0, 0), Xr[1](1, 0), Xr[1](2, 0);

                    px4->setPos(Xr[1](0, 0) / 1000, -Xr[1](1, 0) / 1000, -Xr[1](2, 0) / 1000,
                                -eulerAnglesDrone[0], eulerAnglesDrone[1], eulerAnglesDrone[2]);

                    // mlog->show("pos : " +
                    //            QString::number(T_Drone(0, 0), 'f', 2) + "\t" +
                    //            QString::number(T_Drone(1, 0), 'f', 2) + "\t" +
                    //            QString::number(T_Drone(2, 0), 'f', 2) + "\t" +
                    //            QString::number(eulerAnglesDrone[0], 'f', 2) + "\t" +
                    //            QString::number(eulerAnglesDrone[1], 'f', 2) + "\t" +
                    //            QString::number(eulerAnglesDrone[2], 'f', 2));

                    emit onXYZSignals(Xr, &eulerAnglesDrone, vision_param.ptNum);
                }
                else
                {
                    onMatching();
                }
            }
            else
            {
                onMatching();
            }
        }
    }
}

Vector3d *VisionProcess::triangulation(void)
{
    pointNum = checkVPointSize();

    if (pointNum < 1)
    {
        //DBG("Error Return, pointNum < 1");
        return nullptr;
    }

    vision_param.ptNum = pointNum;
    vision_param.CamNum = camNum;

    vision_param.idx.resize(camNum, pointNum);

    for (int pm = 0; pm < pointNum; pm++)
    {
        vision_param.xy[pm].resize(2, camNum);
        for (int cm = 0; cm < camNum; cm++)
        {
            for (int ppm = 0; ppm < pointNum; ppm++)
            {
                if (camResult[cm].vPoint[ppm]->id == pm)
                {
                    vision_param.idx.row(cm)(pm) = 1;
                    vision_param.xy[pm].col(cm)(0) = camResult[cm].vPoint[ppm]->x;
                    vision_param.xy[pm].col(cm)(1) = camResult[cm].vPoint[ppm]->y;
                    break;
                }
            }
        }
    }

    return multipleViewTriangulation.triangulation();
}

int VisionProcess::onMatching(void)
{
    static QElapsedTimer mstimer;
    static CameraPointMap cpMap;

    pointNum = checkVPointSize();

    if (pointNum < 1)
    {
        //DBG("Error Return, pointNum < 1");
        return -1;
    }

    MatrixXi map = cpMap.getPointMap(camNum, pointNum);
    int index[pointNum];

    MatrixXd xy[map.rows()];
    double rerr[map.rows()];
    Vector3d Xr[map.rows()];
    vector<double> rerrSort;

    for (int i = 0; i < map.rows(); i++)
    {
        for (int cm = 0; cm < camNum; cm++)
        {

            xy[i].resize(2, camNum);
            xy[i].col(cm)(0) = camResult[cm].vPoint[map.row(i)(cm)]->x;
            xy[i].col(cm)(1) = camResult[cm].vPoint[map.row(i)(cm)]->y;
        }
    }

    mstimer.start();

    multipleViewTriangulation.getRMS(vision_param.P,
                                     camNum,
                                     xy,
                                     Xr,
                                     map.rows(),
                                     rerr);

    mlog->show("get rms take time " + QString::number((double)mstimer.nsecsElapsed() / (double)1000) + "us");

    std::cout << "===>>>result rms :\r\n";
    for (int i = 0; i < map.rows(); i++)
    {
        //std::cout << i << " : " << map.row(i) << " " << rerr[i] << endl;
        rerrSort.push_back(rerr[i]);
    }

    if (rerrSort.size() < pointNum)
    {
        DBG("rerrSort size Error, Return");
        return -1;
    }

    sort(rerrSort.begin(), rerrSort.end());

    //for debug
    for (int pm; pm < pointNum*2; pm++)
    {
        DBG("sort index pm : %d \t%f", getIndex(rerr, map.rows(), rerrSort[pm]),rerrSort[pm]);
    }

    for (int pm; pm < pointNum; pm++)
    {
        index[pm] = getIndex(rerr, map.rows(), rerrSort[pm]);
        DBG("index pm : %d", index[pm]);
    }

    
    double d1 = multipleViewTriangulation.distance3d(
            cv::Point3d(Xr[index[0]](0, 0), Xr[index[0]](1, 0), Xr[index[0]](2, 0)),
            cv::Point3d(Xr[index[1]](0, 0), Xr[index[1]](1, 0), Xr[index[1]](2, 0)));
    double d2 = multipleViewTriangulation.distance3d(
            cv::Point3d(Xr[index[0]](0, 0), Xr[index[0]](1, 0), Xr[index[0]](2, 0)),
            cv::Point3d(Xr[index[2]](0, 0), Xr[index[2]](1, 0), Xr[index[2]](2, 0)));
    double d3 = multipleViewTriangulation.distance3d(
            cv::Point3d(Xr[index[2]](0, 0), Xr[index[2]](1, 0), Xr[index[2]](2, 0)),
            cv::Point3d(Xr[index[1]](0, 0), Xr[index[1]](1, 0), Xr[index[1]](2, 0)));

    DBG("%f\t%f\t%f\r\n",d1,d2,d3);

    d1 = multipleViewTriangulation.distance3d(
            cv::Point3d(Xr[index[3]](0, 0), Xr[index[3]](1, 0), Xr[index[3]](2, 0)),
            cv::Point3d(Xr[index[4]](0, 0), Xr[index[4]](1, 0), Xr[index[4]](2, 0)));
    d2 = multipleViewTriangulation.distance3d(
            cv::Point3d(Xr[index[3]](0, 0), Xr[index[3]](1, 0), Xr[index[3]](2, 0)),
            cv::Point3d(Xr[index[5]](0, 0), Xr[index[5]](1, 0), Xr[index[5]](2, 0)));
    d3 = multipleViewTriangulation.distance3d(
            cv::Point3d(Xr[index[4]](0, 0), Xr[index[4]](1, 0), Xr[index[4]](2, 0)),
            cv::Point3d(Xr[index[5]](0, 0), Xr[index[5]](1, 0), Xr[index[5]](2, 0)));

    DBG("%f\t%f\t%f\r\n",d1,d2,d3);

    return 0;
}