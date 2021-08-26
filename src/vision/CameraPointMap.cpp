#include "CameraPointMap.h"

CameraPointMap::~CameraPointMap()
{
    free(ii);
}

CameraPointMap::CameraPointMap()
{
}

MatrixXi CameraPointMap::getPointMap(int camNum,int pointNum)
{
    if(ii != nullptr)
    {
        free(ii);
    }
    ii = (int *)malloc(camNum*8);
    colCount = -1;
    countRow = 0;
    per.resize(pow(pointNum,camNum),camNum);
    //std::cout << per << std::endl;
    getMap(camNum,pointNum);
    return per;
}

void CameraPointMap::getMap(int row,int col)
{
    colCount++;
    for(ii[colCount]=0;ii[colCount]<col;ii[colCount]++)
    {
        if(colCount + 1 < row)
            getMap(row,col);
        else
        {
            //std::cout << countRow << "  : ";
            for(int i=0;i<row;i++)
            {
                //std::cout << ii[i] << " ";
                per.row(countRow)(i) = ii[i];
            }
            countRow++;
            
            //std::cout << "\r\n";
        }
            
    }
    colCount--;
}