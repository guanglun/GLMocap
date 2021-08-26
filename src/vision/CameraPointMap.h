#ifndef CAMERAPOINTMAP_H
#define CAMERAPOINTMAP_H

#include "workspace.h"

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>

using namespace Eigen;

class CameraPointMap
{

private:
    int *ii = nullptr;
    int countRow = 0;
    int colCount = -1;
    MatrixXi per;
public:
    CameraPointMap();
    ~CameraPointMap();
    MatrixXi getPointMap(int camNum,int pointNum);
    void getMap(int row,int col);
};

#endif //CAMERAPOINTMAP_H
