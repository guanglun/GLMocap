#include "VisionProcess.h"

VisionProcess::VisionProcess(QObject *parent)
{
}

void VisionProcess::init(int camNum)
{
    this->camNum = camNum;
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

            vision_param.CamNum = camNum;
            vision_param.ptNum = 1;

            for (int i = 0; i < camNum; i++)
            {
                if (camResult[i].pointNum <= 0)
                {
                    vision_param.idx.row(0)(i) = 0;
                }
                else
                {
                    foundNum++;
                    vision_param.idx.row(0)(i) = 1;
                    vision_param.xy[0].col(i)(0) = camResult[i].x[0];
                    vision_param.xy[0].col(i)(1) = camResult[i].y[0];
                }
            }

            //vision_param.xy[0].col(0)(0) = 10;

            // qint64 t1, t2;
            // t1 = QDateTime::currentDateTime().toMSecsSinceEpoch();

            if (foundNum >= 3)
            {
                multipleViewTriangulation.triangulation();
            }


            // t2 = QDateTime::currentDateTime().toMSecsSinceEpoch();
            // mlog->show(" diff " + QString::number(t2 - t1));
        }
    }
}
