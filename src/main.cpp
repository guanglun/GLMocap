#include <QApplication>

#include "formcamwindow.h"
#include "formcvwindow.h"

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#define camNum  4
#define pintNum 3
// int row = camNum;
// int rowCount = 0;
// int colCount = 0;
// MatrixXi per;
// //2,3
// void forloop(int row, int col)
// {
//     for (int icol = 0; icol < col; icol++)
//     {
//         for (int i = 0; i < pow(col, col); i++)
//         {
//             per.row(rowCount)(i) = 1;
//         }
//     }
// }
int count11 = 0;
int colCount = -1;
int ii[camNum];
void getMap(int row,int col)
{
    colCount++;
    for(ii[colCount]=0;ii[colCount]<col;ii[colCount]++)
    {
        if(colCount < col)
            getMap(row,col);
        else
        {
            count11++;
std::cout << count11 << "  : "  << ii[0] << " " << ii[1] << " " << ii[2] << " " << ii[3] << " " << endl;

        }
            
    }
    colCount--;
}

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);

    // FormCamWindow fCamWindow;
    // fCamWindow.show();

    // per.row(2)(1) = 1;
    int tmp = pow(pintNum, camNum);
    //MatrixXi per(81,4 );
    Matrix<int, 81, 4> per;
    //forloop(camNum, pintNum);

    getMap(camNum, pintNum);

    // int count = 0;

    // for (int a = 0; a < 3; a++)
    // {
    //     // per.row(0)(a * 4 + 0) = a;
    //     // per.row(0)(a * 4 + 1) = a;
    //     // per.row(0)(a * 4 + 2) = a;
    //     // per.row(0)(a * 4 + 3) = a;

    //     for (int b = 0; b < 3; b++)
    //     {
    //         // per.row(1)(b * 4 + 0) = b;
    //         // per.row(1)(b * 4 + 1) = b;
    //         // per.row(1)(b * 4 + 2) = b;
    //         // per.row(1)(b * 4 + 3) = b;
    //         for (int c = 0; c < 3; c++)
    //         {
    //             // per.row(0)(c * 4 + 0) = c;
    //             // per.row(2)(c * 4 + 1) = c;
    //             // per.row(2)(c * 4 + 2) = c;
    //             // per.row(2)(c * 4 + 3) = c;
    //             for (int d = 0; d < 3; d++)
    //             {
    //                 // per.row(3)(d * 4 + 0) = d;
    //                 // per.row(3)(d * 4 + 1) = d;
    //                 // per.row(3)(d * 4 + 2) = d;
    //                 // per.row(3)(d * 4 + 3) = d;
    //                 per.row(count) << a , b , c , d;
    //                 count++;
    //                 std::cout << count << "  : " << a << " " << b << " " << c << " " << d << " " << endl;
    //             }
    //         }
    //     }
    // }

    // std::cout << per << endl;

    return 0; //a.exec();
}
