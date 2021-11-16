#include "MarksManagement.h"

using namespace cv;

MarksManagement::MarksManagement()
{

}

void MarksManagement::inputPoint(int index,vector<Point2f> pt)
{
    freePoint[index].clear();
    for(int i=0;i<pt.size();i++)
        freePoint[index].push_back(new MarkPoint(pt.at(i).x,pt.at(i).y));
}

void MarksManagement::catchDone(int camNum)
{
    this->camNum = camNum;

    DBG("%d %d %d %d\r\n",freePoint[0].size(),freePoint[1].size(),freePoint[2].size(),freePoint[3].size());


}