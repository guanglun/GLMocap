#ifndef MULTIPLEVIEWTRANGULATION_H
#define MULTIPLEVIEWTRANGULATION_H

#include "workspace.h"
#include <QObject>

#include <Eigen/Dense>

using namespace Eigen;

typedef Matrix<double, 3, 3> Matrix33d;
typedef Matrix<double, 3, 4> Matrix34d;
typedef Matrix<double, 3, 9> Matrix39d;
typedef Matrix<double, 6, 1> Vector6d;
typedef Matrix<double, 6, 3> Matrix63d;
typedef Matrix<double, 6, 6> Matrix6d;
typedef Matrix<double, 9, 1> Vector9d;
typedef Matrix<double, 9, 9> Matrix9d;
typedef DiagonalMatrix<double, 9> DiagMatrix9d;

#define PT_NUM 1
#define CAM_NUM_ALL 2

#define PT_NUM_MAX 36
#define CAM_NUM_MAX 36

#define ARC_TO_DEG 57.29577951308238
#define DEG_TO_ARC 0.0174532925199433

// constants for defualt values
#define Default_f0 (1.0)
#define Max_Iteration (12)
#define Convergence_EPS (1e-12)
#define Large_Number (1e99)

struct VISION_PARAM
{
    int CamNum;
    int ptNum;
    Matrix34d P[CAM_NUM_MAX];
    Matrix33d R[CAM_NUM_MAX];
    RowVector3d T[CAM_NUM_MAX];
    MatrixXd xy[PT_NUM_MAX];
    // Matrix<double, PT_NUM_MAX, CAM_NUM_MAX> idx;
    MatrixXi idx;
    // Vector3d    Xr[PT_NUM_MAX];
};

extern struct VISION_PARAM vision_param;

class MultipleViewTriangulation : public QObject
{
    Q_OBJECT
private:
    // constant Vectors and Matrices
    const Vector2d ZeroVec2 = Vector2d::Zero();
    const Vector3d ZeroVec3 = Vector3d::Zero();
    const Vector6d ZeroVec6 = Vector6d::Zero();
    const Matrix6d ZeroMat6 = Matrix6d::Zero();

    char positionFlag[4] = {0, 0, 0, 0};

    //MatrixXd xy[PT_NUM_MAX];
    Vector3d Xr[PT_NUM_MAX];

public:
    MultipleViewTriangulation();

    Vector3d triangulation(
        const Matrix34d Prj[],
        const Vector2d xy[],
        int CamNum,
        double f0);

    // triangulation from all cameras and points
    bool triangulation_all(const Matrix34d Prj[],
                           int CamNum,
                           MatrixXd x[],
                           Vector3d Xr[],
                           int PtNum,
                           MatrixXi& idx,
                           double f0 = Default_f0);

    bool optimal_correction_all(
        Matrix34d Proj[],
        int CamNumAll,
        MatrixXd Xk0[],
        MatrixXd Xkc0[],
        MatrixXi& idx,
        double reperr[],
        int PtNum,
        int Max_Iter = Max_Iteration,
        double Conv_EPS = Convergence_EPS,
        double f0 = Default_f0);

    bool optimal_correction(
        const Matrix3d tfT[][3],
        int CNum,
        Vector3d xk[],
        Vector3d xc[],
        double *reperr,
        int Max_Iter,
        double Conv_EPS);

    void make_trifocal_tensor(
        const Matrix34d *P,
        Matrix3d *tfT);

    double calc_Txyz(
        const Matrix3d tfT[],
        const Vector3d &x, const Vector3d &y, const Vector3d &z,
        int p, int q);

    Vector9d make_Fmat(const Matrix34d &P0, const Matrix34d &P1);
    MatrixXd generalized_inverse_rank_N(
        const MatrixXd &M, int rank);

    static Matrix3d eulerAnglesToRotationMatrix(Vector3d &theta);
    static bool isRotationMatirx(Matrix3d R);
    static Vector3d rotationMatrixToEulerAngles(Matrix3d &R);
    void triangulation(void);
private slots:
    void positionSlot(int camIndex, double x, double y);
signals:
    void onXYZSignals(double x, double y, double z);
};

#endif
