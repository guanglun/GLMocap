#include "MultipleViewTriangulation.h"
#include <iostream>

MultipleViewTriangulation::MultipleViewTriangulation()
{
    int PtNum = 2;
    int CamNumAll = 4;

    Matrix34d Prj[CamNumAll];

    Prj[0] << 
    443.169467966925,	0,	287.619684182048,	0,
    0,	440.173341386739,	230.626794522958,	0,
    0,	0,	1,	0;

    Prj[1] << 
    479.600401347489,	23.5705924151087,	273.184469599511,	86560.3059274555,
    20.7645709392858,	460.165369349341,	196.629121042498,	5104.77578558034,
    0.107714891453310,	0.0708712177551011,	0.991652546335213,	43.7904190041849;

    Prj[2] <<
    527.114979412141,	0.169291136410878,	231.181347920051,	194710.194193955,
    65.0399923964585,	438.312938642950,	196.735669670058,	24743.6483437954,
    0.292663543763369,	0.0127532204559081,	0.956130433319570,	150.934134282299;

    Prj[3] <<
    629.635920552627,	25.8147970025918,	319.590254142795,	368723.322069735,
    33.5506579257593,	608.136814742276,	139.809609470878,	12649.0376949619,
    0.108263701344935,	0.113534049998329,	0.987617836241363,	89.3190155186334;

    // std::cout << "Prj[0]:\n" << Prj[0] << std::endl;
    // std::cout << "Prj[1]:\n" << Prj[1] << std::endl;
    // std::cout << "Prj[2]:\n" << Prj[2] << std::endl;
    // std::cout << "Prj[3]:\n" << Prj[3] << std::endl;

    MatrixXd x[PtNum];

    x[0].resize(2,4);

    x[0] << 
    265.2741,  328.4092,  364.6402,  605.1474,
    366.7181,  325.4520,  330.6526,  310.3652;

    x[1].resize(2,4);

    x[1] << 
    265.2714,  328.3260,  364.5706,  605.4942,
    359.7180,  318.8383,  324.1468,  302.2464;

    std::cout << "x[0]:\n" << x[0] << std::endl;

    //std::cout << "x[0].col(0):\n" << x[0].col(0) << std::endl;

    //MatrixXi idx(PtNum,CamNumAll);
    MatrixXi idx(PtNum,CamNumAll);
    idx << 
    1,1,1,1,
    1,1,1,1;
    std::cout << "idx:\n" << idx << std::endl;


    Vector3d Xr[PtNum];
    triangulation_all(Prj,CamNumAll,x,Xr,PtNum,idx);

    std::cout << "Xr[0]\n" << Xr[0] << std::endl;
    std::cout << "Xr[1]\n" << Xr[1] << std::endl;

    //std::cout << "Xr[1]\n" << Xr[1](1,0) << std::endl;
    
    float sum = sqrt(pow(Xr[0](0,0)-Xr[1](0,0),2)+pow(Xr[0](1,0)-Xr[1](1,0),2)+pow(Xr[0](2,0)-Xr[1](2,0),2));

    std::cout << "sum\n" << sum << std::endl;
}

// triangulation from multiple view
bool
MultipleViewTriangulation::triangulation_all(
    const Matrix34d Prj[],
    int CamNumAll,
    MatrixXd x[],
    Vector3d Xr[],
    int PtNum,
    const MatrixXi& idx,
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