#include "image.h"

Image::Image()
{

}

QByteArray Image::imageToBase64(QString fileName)
{
    QImage img(fileName);
    QByteArray ba;
    QBuffer buf(&ba);//��QByteArray����QBuffer
    buf.open(QIODevice::WriteOnly);
    img.save(&buf,"JPG");//��img д��QBuffer

    QByteArray base64 = ba.toBase64();//����������ͷ
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    //��ͼƬurlencode
    QByteArray imaData = codec->fromUnicode(base64).toPercentEncoding();

    return imaData;
}

