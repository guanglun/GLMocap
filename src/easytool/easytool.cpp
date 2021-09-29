#include "easytool.h"


/*
 * @breif 将16进制字符串转换为对应的字节序列
 */
QByteArray EasyTool::HexStringToByteArray(QString HexString)
{
    bool ok;
    QByteArray ret;
    HexString = HexString.trimmed();
    HexString = HexString.simplified();
    QStringList sl = HexString.split(" ");

    foreach (QString s, sl) {
        if(!s.isEmpty())
        {
            char c = s.toInt(&ok,16)&0xFF;
            if(ok){
                ret.append(c);
            }else{
                //qDebug()<<"非法的16进制字符："<<s;
                //QMessageBox::warning(0,tr("错误："),QString("非法的16进制字符: \"%1\"").arg(s));
            }
        }
    }
    //qDebug()<<ret;
    return ret;
}

QString EasyTool::ByteArrayToHexString(QByteArray data)
{
    QString ret(data.toHex().toUpper());
    int len = ret.length()/2;
    for(int i=1;i<len;i++)
    {
        ret.insert(2*i+i-1," ");
    }

    return ret;
}

bool EasyTool::isDirExist(QString fullPath)
{
    QDir dir(fullPath);
    if(dir.exists())
    {
      return true;
    }
    return false;
}


bool EasyTool::isFileExist(QString fullPath)
{
    QFile file(fullPath);
    if(file.exists())
    {
      return true;
    }
    return false;
}

Eigen::Matrix<double, 4, 4> EasyTool::getRT44d(Eigen::Matrix<double, 3, 3> &R,Eigen::Matrix<double, 3, 1> &T)
{

    Eigen::Matrix<double, 4, 4> RT;
    RT << 
    R(0, 0),R(0, 1),R(0, 2),T(0,0),
    R(1, 0),R(1, 1),R(1, 2),T(1,0),
    R(2, 0),R(2, 1),R(2, 2),T(2,0),
    0,0,0,1;

    return RT;
}

Eigen::Matrix<double, 3, 4> EasyTool::getRT34d(Eigen::Matrix<double, 3, 3> &R,Eigen::Matrix<double, 3, 1> &T)
{

    Eigen::Matrix<double, 3, 4> RT;
    RT << 
    R(0, 0),R(0, 1),R(0, 2),T(0,0),
    R(1, 0),R(1, 1),R(1, 2),T(1,0),
    R(2, 0),R(2, 1),R(2, 2),T(2,0);

    return RT;
}

void EasyTool::RT44d(Eigen::Matrix<double, 4, 4> &RT,Eigen::Matrix<double, 3, 3> &R,Eigen::Matrix<double, 3, 1> &T)
{

    R << 
    RT(0, 0),RT(0, 1),RT(0, 2),
    RT(1, 0),RT(1, 1),RT(1, 2),
    RT(2, 0),RT(2, 1),RT(2, 2);
    T << RT(0,3),RT(1,3),RT(2,3);
}

QString EasyTool::MatToString(const Eigen::MatrixXd& mat){
    std::stringstream ss;
    ss << mat;
    return QString::fromStdString(ss.str());
}