#include "MultipleViewTriangulation.h"
#include <iostream>

// correct all points for every camera.
bool
MultipleViewTriangulation::optimal_correction_all(
    Matrix34d Proj[],
    int CamNumAll,
    MatrixXd Xk0[],
    MatrixXd Xkc0[],
    MatrixXi& idx,
    double reperr[],
    int PtNum,
    int Max_Iter,
    double Conv_EPS,
    double f0
)
{
    int CamNum;
    int cc;
    bool flag;

    // size check
    if (Xk0[0].rows() != 2)
        return false;

    // correction loop for each point
    for (int pnum = 0; pnum < PtNum; pnum++)
    {
        // Count cameras
        CamNum = 0;
        for (int cnum = 0; cnum < CamNumAll; cnum++)
        {
            idx(pnum,cnum) = false;
            if (Xk0[pnum].col(cnum)(0) < 0.0) continue; // x < 0: a point cannot be observe
            idx(pnum,cnum) = true;
            CamNum++;
        }

        // three or more cameras
        if (CamNum >= 3)
        {
            Matrix34d Prj[CamNum];
            Matrix3d tfT[CamNum-2][3];
            Vector3d xk[CamNum], xkc[CamNum];

            cc = 0;
            for (int cnum = 0; cnum < CamNumAll; cnum++)
            {
                if (idx(pnum,cnum))
                {
                    xk[cc] << Xk0[pnum].col(cnum)/f0, 1.0;
                    Prj[cc] = Proj[cnum];
                    //std::stringstream stream;

                    //stream << "Prj[" << cc << "] :" << std::endl;
                    //stream << Prj[cc] << std::endl;
                    //stream << "Proj[" << cnum << "] :" << std::endl;
                   //stream << Proj[cnum] << std::endl;
                   //stream << "xk[" << cc << "] :" << std::endl;
                   // stream << xk[cc] << std::endl;
                   // stream << "Xk0[" << pnum << "] :" << std::endl;
                   // stream << Xk0[pnum].col(cnum) << std::endl;
                    //stream << "xkt[" << kp << "] :" << std::endl;
                    //stream << xkt[kp] << std::endl;
                   // std::fprintf( stderr, "%s\n", stream.str().c_str());
                    cc++;
                }
            }

            // compute trifocal tensors
            for (int cnum = 0; cnum < CamNum - 2; cnum++)
                make_trifocal_tensor(&(Prj[cnum]), &(tfT[cnum][0]));

            // correction
            flag = optimal_correction(tfT, CamNum, xk, xkc, reperr+pnum,
                                      Max_Iter, Conv_EPS);

            //mlog->show(">>>>>>> " + QString::number(CamNum) + " " + QString::number(flag) + " " + QString::number(reperr[pnum]));

            if (flag) // success
            {
                //std::fprintf( stderr, "\n\nIndividual call to optimal_correction was successful.\n\n" );
                cc = 0;
                for (int cnum = 0; cnum < CamNumAll; cnum++)
                {
                    if (idx(pnum,cnum))
                    {
                        Xkc0[pnum].col(cnum) << f0 * xkc[cc](0), f0 * xkc[cc](1);
                        cc++;
                    }
                    else
                        Xkc0[pnum].col(cnum) = ZeroVec2;
                }
            }
            else // failed
            {
                reperr[pnum] = -1;
				//std::fprintf( stderr, "\n\nIndividual call to optimal_correction FAILED.\n\n" );
                for (int cnum = 0; cnum < CamNum; cnum++)
                    Xkc0[pnum].col(cnum) = ZeroVec2;
            }
        }
    }

    return true;
}

bool
MultipleViewTriangulation::optimal_correction(
    const Matrix3d tfT[][3],
    int CNum,
    Vector3d xk[],
    Vector3d xc[],
    double *reperr,
    int Max_Iter,
    double Conv_EPS
)
{
    Vector3d xkh[CNum], xkt[CNum];
    double EE, E0;
    Matrix39d P[CNum], Q[CNum], R[CNum];
    Vector3d Pk[3], pt, qt, rt;
    Matrix9d A[CNum], B[CNum], C[CNum], D[CNum], E[CNum];
    Vector9d F[CNum];
//   Vector9d lmd[CNum];

    // Matrix 9(M-2) * 9(M-2)
    int sz = 9*(CNum - 2);
    MatrixXd T;
    MatrixXd Ti(sz, sz);
    VectorXd ff(sz);
    VectorXd llmd(sz);

    int count;

    // Set Pk
    Pk[0] << 1.0, 0.0, 0.0;
    Pk[1] << 0.0, 1.0, 0.0;
    Pk[2] << 0.0, 0.0, 0.0;

    //std::fprintf( stderr,"Starting Point matrices before iterations.\n");
    for (int kp = 0; kp < CNum; kp++)
    {
        xkh[kp] = xk[kp];
        xkt[kp] = ZeroVec3;
        //std::cout << "xk[" << kp << "] :" << std::endl;
        //std::cout << xk[kp] << std::endl;
        //std::cout << "xkt[" << kp << "] :" << std::endl;
        //std::cout << xkt[kp] << std::endl;
        //std::cout << "xkh[" << kp << "] :" << std::endl;
        //std::cout << xkh[kp] << std::endl;
    }

	
    E0 = Large_Number;
    count = 0;

    do
    {
        int pq, rs;
        // set Pkpqs, Qkpqs, and Rkpqs
        // Pkpqs = P[kp](s,3*p+q)
        // Qkpqs = Q[kp](s,3*p+q)
        // Rkpqs = R[kp](s,3*p+q)
        for (int kp = 0; kp < CNum; kp++)
        {
            int kpm1, kpm2, kpp1, kpp2;
            kpm1 = kp - 1;
            kpm2 = kp - 2;
            kpp1 = kp + 1;
            kpp2 = kp + 2;

            for (int s = 0; s < 3; s++)
                for (int p = 0; p < 3; p++)
                    for (int q = 0; q < 3; q++)
                    {
                        pq = 3*p + q;
                        if (kp < CNum - 2)
                            P[kp](s,pq) = calc_Txyz(tfT[kp], Pk[s], xkh[kpp1], xkh[kpp2], p, q);
                        else
                            P[kp](s,pq) = 0.0;
                        if (kp >= 1 && kp < CNum -1)
                            Q[kp](s,pq) = calc_Txyz(tfT[kp-1], xkh[kpm1], Pk[s], xkh[kpp1], p, q);
                        else
                            Q[kp](s,pq) = 0.0;
                        if (kp >= 2)
                            R[kp](s,pq) = calc_Txyz(tfT[kp-2], xkh[kpm2], xkh[kpm1], Pk[s], p, q);
                        else
                            R[kp](s,pq) = 0.0;
                    }
        }

        // set Ak(pqrs), Bk(pqrs), Ck(pqrs), Dk(pqrs), Ek(pqrs), and Fk(pq)
        for (int kp = 0; kp < CNum - 2; kp++)
        {
            for (int p = 0; p < 3; p++)
            {
                for (int q = 0; q < 3; q++)
                {
                    pq = 3*p + q;
                    for (int r = 0; r < 3; r++)
                    {
                        for (int s = 0; s < 3; s++)
                        {
                            rs = 3*r + s;

                            if (kp >= 2)
                            {
                                A[kp](pq,rs) =
                                    calc_Txyz(tfT[kp], R[kp].col(rs), xkh[kp+1], xkh[kp+2], p, q);
                            }
                            if (kp >= 1)
                            {
                                B[kp](pq,rs) =
                                    calc_Txyz(tfT[kp], Q[kp].col(rs), xkh[kp+1], xkh[kp+2], p, q)
                                    + calc_Txyz(tfT[kp], xkh[kp], R[kp+1].col(rs), xkh[kp+2], p, q);
                            }
                            C[kp](pq,rs) =
                                calc_Txyz(tfT[kp], P[kp].col(rs), xkh[kp+1], xkh[kp+2], p, q)
                                + calc_Txyz(tfT[kp], xkh[kp], Q[kp+1].col(rs), xkh[kp+2], p, q)
                                + calc_Txyz(tfT[kp], xkh[kp], xkh[kp+1], R[kp+2].col(rs), p, q);
                            if (kp <= CNum - 4)
                            {
                                D[kp](pq,rs) =
                                    calc_Txyz(tfT[kp], xkh[kp], P[kp+1].col(rs), xkh[kp+2], p, q)
                                    + calc_Txyz(tfT[kp], xkh[kp], xkh[kp+1], Q[kp+2].col(rs), p, q);
                            }
                            if (kp <= CNum - 5)
                            {
                                E[kp](pq,rs) =
                                    calc_Txyz(tfT[kp], xkh[kp], xkh[kp+1], P[kp+2].col(rs), p, q);
                            }
                        }
                    }
                    F[kp](pq) =
                        calc_Txyz(tfT[kp], xkh[kp], xkh[kp+1], xkh[kp+2], p, q)
                        + calc_Txyz(tfT[kp], xkt[kp], xkh[kp+1], xkh[kp+2], p, q)
                        + calc_Txyz(tfT[kp], xkh[kp], xkt[kp+1], xkh[kp+2], p, q)
                        + calc_Txyz(tfT[kp], xkh[kp], xkh[kp+1], xkt[kp+2], p, q);
                }
            }
        }

        T = MatrixXd::Zero(sz, sz);
        for (int kp = 0; kp < CNum - 2; kp++)
        {
            T.block(9*kp,9*kp,9,9) = C[kp];
            if (kp <= CNum - 4)
            {
                T.block(9*kp, 9*(kp+1), 9, 9) = D[kp];
            }
            if (kp <= CNum - 5)
            {
                T.block(9*kp, 9*(kp+2), 9, 9) = E[kp];
            }
            if (kp >= 1)
            {
                T.block(9*kp, 9*(kp-1), 9, 9) = B[kp];
            }
            if (kp >= 2)
            {
                T.block(9*kp, 9*(kp-2), 9, 9) = A[kp];
            }
            ff.segment(9*kp,9) = F[kp];
        }

        // Generalized Inverse
        Ti = generalized_inverse_rank_N(T, 2*CNum - 3);

        // solve linear equations
        llmd = Ti * ff;

        // update xt and xt
        //std::cout << "Updating matrices ..." << std::endl;
        for (int kp = 0; kp < CNum; kp++)
        {
            xkt[kp] = ZeroVec3;
            for (int i = 0; i < 3; i++)
            {
				for (int p = 0; p < 3; p++)
				{
                    for (int q = 0; q < 3; q++)
                    {
                        double ttt1, ttt2, ttt3;

                        pq = 3*p + q;

                        if (9*kp + pq >= sz)
                            ttt1 = 0.0;
                        else
                            ttt1 = P[kp](i,pq) * llmd(9*kp+pq);

                        if (9*(kp-1) + pq >= sz || kp < 1)
                            ttt2 = 0.0;
                        else
                            ttt2 = Q[kp](i,pq) * llmd(9*(kp-1)+pq);

                        if (9*(kp-2) + pq >= sz || kp < 2)
                            ttt3 = 0.0;
                        else
                            ttt3 = R[kp](i,pq) * llmd(9*(kp-2)+pq);

                        xkt[kp](i) += ttt1 + ttt2 + ttt3;
                    }
				}
			}
            xkh[kp] = xk[kp] - xkt[kp];
            //std::cout << "xk[" << kp << "] :" << std::endl;
            //std::cout << xk[kp] << std::endl;
            //std::cout << "xkt[" << kp << "] :" << std::endl;
            //std::cout << xkt[kp] << std::endl;
            //std::cout << "xkh[" << kp << "] :" << std::endl;
            //std::cout << xkh[kp] << std::endl;

            //std::stringstream stream;
            //stream << "Error for cam " << kp << ", iteration " << count << " is " << (xkt[kp].norm() * Default_f0)  << "." << std::endl;
            //stream << "Grady::Error for cam " << kp << ", iteration " << count << " is " << (sqrt(pow(xkt[kp][0],2) + pow(xkt[kp][1],2) +  pow(xkt[kp][2],2) ) * Default_f0 ) << "." << std::endl;


            //stream << "xk[" << kp << "] :" << std::endl;
            //stream << xk[kp] << std::endl;
            //stream << "xkh[" << kp << "] :" << std::endl;
            //stream << xkh[kp] << std::endl;
            //stream << "xkt[" << kp << "] :" << std::endl;
            //stream << xkt[kp] << std::endl;
            //std::fprintf( stderr, "%s\n", stream.str().c_str());

        }

        // Compute reprojection error
        EE = 0.0;
        for (int kp = 0; kp < CNum; kp++){
            EE += (xkt[kp].norm() * Default_f0);
		}
        std::stringstream stream;
        //stream << "Total error for iteration " << count << " is " << EE << "." << std::endl;
        //std::fprintf( stderr, "%s\n", stream.str().c_str());
        if (fabs (EE - E0) < Conv_EPS)
        {
            *reperr = EE;
            for (int kp = 0; kp < CNum; kp++){
                xc[kp] = xkh[kp];
            }
            //std::stringstream stream;
            //stream << "\n\nThe OC required " << count << " steps to converge to <" << Conv_EPS << std::endl;
            //stream << "Total error for that sequence was " << EE << "." << std::endl;
            //std::fprintf(stderr, "%s\n", stream.str().c_str());
            return true;
        }
        E0 = EE;
    }
    while (++count < Max_Iter);
	
    return false;
}

