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


