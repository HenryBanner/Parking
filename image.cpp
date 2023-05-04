#include "image.h"

Image::Image()
{

}

QByteArray Image::imageToBase64(QString fileName)
{
    QImage img(fileName);
    QByteArray ba;
    QBuffer buf(&ba);//用QByteArray构造QBuffer
    buf.open(QIODevice::WriteOnly);
    img.save(&buf,"JPG");//把img 写入QBuffer

    QByteArray base64 = ba.toBase64();//不包含编码头
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    //对图片urlencode
    QByteArray imaData = codec->fromUnicode(base64).toPercentEncoding();

    return imaData;
}

