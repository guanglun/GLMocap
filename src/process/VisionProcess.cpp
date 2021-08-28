#include "VisionProcess.h"
#include "CameraPointMap.h"

VisionProcess::VisionProcess(QObject *parent)
{
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

static void removeOther(int camNum, MatrixXi map, int indexResult[], vector<double> *rerrSort,double rerr[])
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
                    eraseData(rerrSort,rerr[pm]);
                    break;
                }
            }
        }
    }
}

int VisionProcess::matchPoint(void)
{
    for (int i = 0; i < camNum - 1; i++)
    {
        if (camResult[i].vPoint.size() != camResult[i + 1].vPoint.size())
        {
            //DBG("Error Return, vPoint size not match");
            return -1;
        }
    }
    pintNum = camResult[0].vPoint.size();

    if (pintNum < 1)
    {
        //DBG("Error Return, pintNum < 1");
        return -1;
    }
    //DBG("pintNum: %d",pintNum);

    CameraPointMap cpMap;
    MatrixXi map = cpMap.getPointMap(camNum, pintNum);
    int indexResult[pintNum];
    //std::cout << map << std::endl;

    MatrixXd xy[map.rows()];
    MatrixXd xyc[map.rows()];
    double rerr[map.rows()];
    vector<double> rerrSort;

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

    multipleViewTriangulation.optimal_correction_all(vision_param.P,
                                                     camNum,
                                                     xy,
                                                     xyc,
                                                     idx,
                                                     rerr, map.rows());
    //std::cout << "===>>>result1:\r\n";
    for (int i = 0; i < map.rows(); i++)
    {
        //std::cout << i << " : " << map.row(i) << " ";
        //mlog->show(QString::number(rerr[i]));
        if(rerr[i] != -1)
        {
            rerrSort.push_back(rerr[i]);
        }
        
    }
    if(rerrSort.size() < pintNum)
    {
        //DBG("rerrSort size Error, Return");
        return -1;
    }

    sort(rerrSort.begin(), rerrSort.end());
    for (int pm; pm < pintNum; pm++)
    {
        indexResult[pm] = getIndex(rerr, map.rows(), rerrSort[pm]);
        DBG("\r\nindex pm : %d",indexResult[pm]);
        if (indexResult[pm] == -1)
        {
            DBG("Error Return, indexResult[%d] == -1",pm);
            return -1;
        }

        removeOther(camNum, map, &indexResult[pm], &rerrSort,rerr);
    }
    std::cout << "===>>>result2:\r\n";
    for (int pm; pm < pintNum; pm++)
    {
        int index = getIndex(rerr, map.rows(), rerrSort[pm]);
        std::cout << index << " : " << map.row(index) << " ";
        mlog->show(QString::number(rerr[index]));

        for(int cm = 0;cm < camNum;cm++)
        {
            camResult[cm].vPoint[map.row(index)(cm)]->id = pm;
        }
        
    }

    return 0;
    
}

void VisionProcess::positionSlot(CAMERA_RESULT result)
{
    bool check = true;
    int foundNum = 0;

    camResult[result.camIndex] = result;

    // mlog->show("index: " + QString::number(result.camIndex) +
    //            " ptnum: " + QString::number(result.pointNum) +
    //            " time: " + QString::number(result.time) +
    //            " diff: " + QString::number(result.time - lastTime[result.camIndex]));
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
        //mlog->show("===>>>" + QString::number(camResult[0].time) + " " + QString::number(camResult[0].time - lastAllTime));
        lastAllTime = camResult[0].time;

        if (isCapImage == true)
        {
            if (camResult[0].time - saveLastTime >= 500)
            {
                for (int i = 0; i < camNum; i++)
                {
                    QString filePath = camResult[i].path + "/" + QString::number(camResult[0].time) + ".png";
                    //mlog->show("save "+filePath);
                    camResult[i].image.save(filePath);
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

            // for (int i = 0; i < camNum; i++)
            // {
            //     if (camResult[i].pointNum <= 0)
            //     {
            //         vision_param.idx.row(0)(i) = 0;
            //     }
            //     else
            //     {
            //         foundNum++;
            //         vision_param.idx.row(0)(i) = 1;
            //         vision_param.xy[0].col(i)(0) = camResult[i].x[0];
            //         vision_param.xy[0].col(i)(1) = camResult[i].y[0];
            //     }
            // }

            //vision_param.xy[0].col(0)(0) = 10;

            if(matchState == MATCH_IDLE)
            {
                qint64 t1, t2;
                t1 = QDateTime::currentDateTime().toMSecsSinceEpoch();
                matchState = MATCH_ING;
                if(matchPoint() == 0)
                {
                    matchState = MATCH_OK;
                }else{
                    matchState = MATCH_IDLE;
                }
                // if (foundNum >= 3)
                // {
                //     multipleViewTriangulation.triangulation();
                // }

                t2 = QDateTime::currentDateTime().toMSecsSinceEpoch();
                //mlog->show(" diff " + QString::number(t2 - t1));
            }

        }
    }
}
