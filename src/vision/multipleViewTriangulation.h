#ifndef MULTIPLEVIEWTRANGULATION_H
#define MULTIPLEVIEWTRANGULATION_H

#include "workspace.h"
#include <QObject>

#include <Eigen/Dense>


using namespace Eigen;

typedef Matrix<double,3,3> Matrix33d;
typedef Matrix<double,3,4> Matrix34d;
typedef Matrix<double,3,9> Matrix39d;
typedef Matrix<double,6,1> Vector6d;
typedef Matrix<double,6,3> Matrix63d;
typedef Matrix<double,6,6> Matrix6d;
typedef Matrix<double,9,1> Vector9d;
typedef Matrix<double,9,9> Matrix9d;
typedef DiagonalMatrix<double,9> DiagMatrix9d;

#define Default_f0  1.0
#define PT_NUM       1
#define CAM_NUM_ALL   2

#define PT_NUM_MAX      36
#define CAM_NUM_MAX     36

#define ARC_TO_DEG 57.29577951308238
#define DEG_TO_ARC 0.0174532925199433

struct VISION_PARAM{
    int CamNum;
    Matrix34d   P[CAM_NUM_MAX];
    Matrix33d   R[CAM_NUM_MAX];
    RowVector3d T[CAM_NUM_MAX];
    MatrixXd    xy[PT_NUM_MAX];
    // Vector3d    Xr[PT_NUM_MAX];
};

extern struct VISION_PARAM vision_param;

class MultipleViewTriangulation : public QObject
{
    Q_OBJECT
private:
    char positionFlag[4] = {0,0,0,0};

    //MatrixXd xy[PT_NUM_MAX];
    Vector3d Xr[PT_NUM_MAX];
public:

    MultipleViewTriangulation();

    // triangulation from all cameras and points
    bool triangulation_all(const Matrix34d Prj[],
                          int CamNum,
                          MatrixXd x[],
                          Vector3d Xr[],
                          int PtNum,
                          const MatrixXi& idx,
                          double f0 = Default_f0);

    static Matrix3d eulerAnglesToRotationMatrix(Vector3d &theta);
    static bool isRotationMatirx(Matrix3d R);
    static Vector3d rotationMatrixToEulerAngles(Matrix3d &R);
private slots:
    void positionSlot(int camIndex, double x,double y);
signals:
    void onXYZSignals(double x,double y,double z);
};

#endif

