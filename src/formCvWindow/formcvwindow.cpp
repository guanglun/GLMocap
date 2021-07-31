#include "formcvwindow.h"
#include "ui_formcvwindow.h"

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

FormCvWindow::FormCvWindow(QWidget *parent) : QWidget(parent),
                                              ui(new Ui::FormCvWindow)
{
    ui->setupUi(this);

    Mat image = imread("F:\\windows\\Desktop\\camera3\\camera3_0.png", IMREAD_GRAYSCALE);

    Mat temp;


    //cv::imshow("image",temp);

    //cv::waitKey();
}

FormCvWindow::~FormCvWindow()
{

    delete ui;
}

void FormCvWindow::imageSlot(QImage qImage)
{
    Mat image = cv::Mat(qImage.height(),qImage.width(),CV_8UC1,qImage.bits());
    threshold(image, image, 230, 255.0, THRESH_BINARY);


	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(image,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE,Point());
	Mat imageContours=Mat::zeros(image.size(),CV_8UC1);
	Mat Contours=Mat::zeros(image.size(),CV_8UC1);  //绘制

    //DBG("found number %d",contours.size());

    vector<Point2f>centers(contours.size());//圆心
    vector<float>radius(contours.size());//半径

	for(int i=0;i<contours.size();i++)
	{
		//contours[i]代表的是第i个轮廓，contours[i].size()代表的是第i个轮廓上所有的像素点数
		for(int j=0;j<contours[i].size();j++) 
		{
			//绘制出contours向量内所有的像素点
			Point P=Point(contours[i][j].x,contours[i][j].y);
			Contours.at<uchar>(P)=255;
		}
 
		//绘制轮廓
		//drawContours(imageContours,contours,i,Scalar(255),1,1,hierarchy);

        //寻找并绘制最小圆
        minEnclosingCircle(contours[i], centers[i], radius[i]);
        circle(imageContours, centers[i], radius[i], Scalar(255), 1);

	}


    
    cv::cvtColor(imageContours, image, cv::COLOR_BGR2RGB);
    QImage qImg = QImage((const unsigned char *)(image.data), image.cols, image.rows, image.step, QImage::Format_RGB888);
    ui->lb_img->setPixmap(QPixmap::fromImage(qImg));
    ui->lb_img->setScaledContents(true);

    if(contours.size() > 0)
    {
        if(this->index != -1)
        {
            emit positionSignals(this->index, centers[0].x,centers[0].y);    
        }
        
    }
    
}