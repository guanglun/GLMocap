#include "multipleViewTriangulation.h"
#include <iostream>

// struct VISION_PARAM vision_param = {
//     .CamNum = 0,
// };

struct VISION_PARAM vision_param;

MultipleViewTriangulation::MultipleViewTriangulation()
{
    // MatrixXi idx(PT_NUM,CAM_NUM_ALL);

    // Prj[0] << 
    // 548.049981125843,	0,	379.671945319308,	0,
    // 0,	546.580521576416,	219.142050804001,	0,
    // 0,	0,	1,	0;

    // Prj[1] << 
    // 581.602049369935,	-6.69977397891934,	392.276652597385,	129586.910805590,
    // 4.23333484278993,	544.536754956410,	238.300220263312,	-2295.93069926829,
    // 0.0643427051349901,	-0.0415907957059505,	0.997060791531017,	-15.6183130081946;

    
    // Prj[2] <<
    // 527.114979412141,	0.169291136410878,	231.181347920051,	194710.194193955,
    // 65.0399923964585,	438.312938642950,	196.735669670058,	24743.6483437954,
    // 0.292663543763369,	0.0127532204559081,	0.956130433319570,	150.934134282299;

    // Prj[3] <<
    // 629.635920552627,	25.8147970025918,	319.590254142795,	368723.322069735,
    // 33.5506579257593,	608.136814742276,	139.809609470878,	12649.0376949619,
    // 0.108263701344935,	0.113534049998329,	0.987617836241363,	89.3190155186334;

    // std::cout << "Prj[0]:\n" << Prj[0] << std::endl;
    // std::cout << "Prj[1]:\n" << Prj[1] << std::endl;
    // std::cout << "Prj[2]:\n" << Prj[2] << std::endl;
    // std::cout << "Prj[3]:\n" << Prj[3] << std::endl;

    // xy[0].resize(2,CAM_NUM_ALL);

    // xy[0] << 
    // 265.2741,  328.4092,  364.6402,  605.1474,
    // 366.7181,  325.4520,  330.6526,  310.3652;

    // x[1].resize(2,4);

    // x[1] << 
    // 265.2714,  328.3260,  364.5706,  605.4942,
    // 359.7180,  318.8383,  324.1468,  302.2464;

    // std::cout << "x[0]:\n" << x[0] << std::endl;

    //std::cout << "x[0].col(0):\n" << x[0].col(0) << std::endl;
    
    // idx << 
    // 1,1,1,1,
    // 1,1,1,1;

    // idx << 
    // 1,
    // 1;

    // std::cout << "idx:\n" << idx << std::endl;

    // triangulation_all(Prj,CAM_NUM_ALL,xy,Xr,PT_NUM,idx);

    // std::cout << "Xr[0]\n" << Xr[0] << std::endl;
    // std::cout << "Xr[1]\n" << Xr[1] << std::endl;

    // std::cout << "Xr[1]\n" << Xr[1](1,0) << std::endl;
    
    // float sum = sqrt(pow(Xr[0](0,0)-Xr[1](0,0),2)+pow(Xr[0](1,0)-Xr[1](1,0),2)+pow(Xr[0](2,0)-Xr[1](2,0),2));

    // std::cout << "sum\n" << sum << std::endl;
}

Vector3d
MultipleViewTriangulation::triangulation(
    const Matrix34d Prj[],
    const Vector2d xy[],
    int CamNum,
    double f0)
{
    MatrixXd T(2*CamNum,3);
    VectorXd p(2*CamNum);
    Matrix3d M;
    Vector3d b;
    Vector3d r;

    for (int cm = 0; cm < CamNum; cm++)
    {
        T(2*cm  ,0) = f0 * Prj[cm](0,0) - xy[cm](0) * Prj[cm](2,0);
        T(2*cm+1,0) = f0 * Prj[cm](1,0) - xy[cm](1) * Prj[cm](2,0);

        T(2*cm  ,1) = f0 * Prj[cm](0,1) - xy[cm](0) * Prj[cm](2,1);
        T(2*cm+1,1) = f0 * Prj[cm](1,1) - xy[cm](1) * Prj[cm](2,1);

        T(2*cm  ,2) = f0 * Prj[cm](0,2) - xy[cm](0) * Prj[cm](2,2);
        T(2*cm+1,2) = f0 * Prj[cm](1,2) - xy[cm](1) * Prj[cm](2,2);

        p(2*cm)   = f0 * Prj[cm](0,3) - xy[cm](0) * Prj[cm](2,3);
        p(2*cm+1) = f0 * Prj[cm](1,3) - xy[cm](1) * Prj[cm](2,3);
    }

    M = T.transpose() * T;
    b = - T.transpose() * p;

    // solve using LU decomp.
    FullPivLU<Matrix3d> LU(M);
    r = LU.solve(b);

    return r;
}

// triangulation from multiple view
bool
MultipleViewTriangulation::triangulation_all(
    const Matrix34d Prj[],
    int CamNumAll,
    MatrixXd x[],
    Vector3d Xr[],
    int PtNum,
    Matrix<double,PT_NUM_MAX,CAM_NUM_MAX>& idx,
    double f0)
{
    int CamNum;
    FullPivLU<Matrix3d> LU;
    Matrix3d M;
    Vector3d b;

    for (int pt = 0; pt < PtNum; pt++)
    {
        CamNum = 0;
        for (int cm = 0; cm < CamNumAll; cm++)
            if (idx(pt,cm))  CamNum++;

        MatrixXd T(2*CamNum,3);
        VectorXd p(2*CamNum);
        int cc;

        cc = 0;
        for (int cm = 0; cm < CamNumAll; cm++)
        {
            if (idx(pt,cm))
            {
                T(cc,0) = f0 * Prj[cm](0,0) - x[pt].col(cm)(0) * Prj[cm](2,0);
                T(cc,1) = f0 * Prj[cm](0,1) - x[pt].col(cm)(0) * Prj[cm](2,1);
                T(cc,2) = f0 * Prj[cm](0,2) - x[pt].col(cm)(0) * Prj[cm](2,2);
                p(cc++) = f0 * Prj[cm](0,3) - x[pt].col(cm)(0) * Prj[cm](2,3);

                T(cc,0) = f0 * Prj[cm](1,0) - x[pt].col(cm)(1) * Prj[cm](2,0);
                T(cc,1) = f0 * Prj[cm](1,1) - x[pt].col(cm)(1) * Prj[cm](2,1);
                T(cc,2) = f0 * Prj[cm](1,2) - x[pt].col(cm)(1) * Prj[cm](2,2);
                p(cc++) = f0 * Prj[cm](1,3) - x[pt].col(cm)(1) * Prj[cm](2,3);
            }
        }

        M = T.transpose() * T;
        b = - T.transpose() * p;

        // solve using LU decomp.
        LU.compute(M);
        Xr[pt] = LU.solve(b);
    }

    return true;
}

void MultipleViewTriangulation::positionSlot(int camIndex, double x,double y)
{
    //DBG("position %d %f %f",camIndex,x,y);

    // MatrixXi idx(PT_NUM,CAM_NUM_ALL);

    // positionFlag[camIndex] = 1;
    // vision_param.xy[0].col(camIndex)(0) = x;
    // vision_param.xy[0].col(camIndex)(1) = y;

    // if(positionFlag[0] && positionFlag[1])
    // {
    //     idx << 
    //     1,
    //     1;

    //     positionFlag[0] = 0;
    //     positionFlag[1] = 0;
    //     triangulation_all(vision_param.P,vision_param.CamNum,vision_param.xy,Xr,PT_NUM,idx);
        
    //     //std::cout << Xr[0](0,0)<< "\t" << Xr[0](1,0)<< "\t\t"<< Xr[0](2,0)<< std::endl;

    //     emit onXYZSignals(Xr[0](0,0),Xr[0](1,0),Xr[0](2,0));
    // }
    
}

void MultipleViewTriangulation::triangulation(void)
{
    triangulation_all(vision_param.P,vision_param.CamNum,vision_param.xy,Xr,vision_param.ptNum,vision_param.idx);
    emit onXYZSignals(Xr[0](0,0),Xr[0](1,0),Xr[0](2,0));
}

Matrix3d MultipleViewTriangulation::eulerAnglesToRotationMatrix(Vector3d &theta)
{
    Matrix3d R_x;    // 计算旋转矩阵的X分量
    R_x <<
            1,              0,               0,
            0,  cos(theta[0]),  -sin(theta[0]),
            0,  sin(theta[0]),   cos(theta[0]);

    Matrix3d R_y;    // 计算旋转矩阵的Y分量
    R_y <<
            cos(theta[1]),   0, sin(theta[1]),
            0,   1,             0,
            -sin(theta[1]),  0, cos(theta[1]);

    Matrix3d R_z;    // 计算旋转矩阵的Z分量
    R_z <<
            cos(theta[2]), -sin(theta[2]), 0,
            sin(theta[2]),  cos(theta[2]), 0,
            0,              0,             1;
    Matrix3d R = R_z * R_y * R_x;
    return R;
}

bool MultipleViewTriangulation::isRotationMatirx(Matrix3d R)
{
    double err=1e-6;
    Matrix3d shouldIdenity;
    shouldIdenity=R*R.transpose();
    Matrix3d I=Matrix3d::Identity();
    return (shouldIdenity - I).norm() < err;
}

Vector3d MultipleViewTriangulation::rotationMatrixToEulerAngles(Matrix3d &R)
{
    assert(isRotationMatirx(R));
    double sy = sqrt(R(0,0) * R(0,0) + R(1,0) * R(1,0));
    bool singular = sy < 1e-6;
    double x, y, z;
    if (!singular)
    {
        x = atan2( R(2,1), R(2,2));
        y = atan2(-R(2,0), sy);
        z = atan2( R(1,0), R(0,0));
    }
    else
    {
        x = atan2(-R(1,2), R(1,1));
        y = atan2(-R(2,0), sy);
        z = 0;
    }
    return {x, y, z};
}
