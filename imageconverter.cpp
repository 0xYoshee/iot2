#include "imageconverter.h"

QImage Ipl2QImage(const IplImage* newImage, bool upsideDown)
{
    if ((0 != newImage) && (cvGetSize(newImage).width > 0))
    {
        const char* src = newImage->imageData;
        QImage qtemp(newImage->width,newImage->height,QImage::Format_RGB32);

        if (upsideDown)
        {
            for (int y = 0; y < newImage->height; ++y, src += newImage->widthStep)
            {
                uint* dst = reinterpret_cast<uint*>(qtemp.scanLine (newImage->height - y - 1));
                for (int x = 0; x < newImage->width; ++x, ++dst)
                {
                    *dst = qRgb(src[x * newImage->nChannels + 2]
                            ,src[x * newImage->nChannels + 1]
                            ,src[x * newImage->nChannels]);
                }
            }
        }
        else
        {
            for (int y = 0; y < newImage->height; ++y, src += newImage->widthStep)
            {
                uint* dst = reinterpret_cast<uint*>(qtemp.scanLine(y));
                for (int x = 0; x < newImage->width; ++x, ++dst)
                {
                    *dst = qRgb(src[x * newImage->nChannels + 2]
                            ,src[x * newImage->nChannels + 1]
                            ,src[x * newImage->nChannels]);
                }
            }
        }
        return qtemp;
    }
    else
        return QImage();
}
/* TODO
IplImage QImage2Ipl(const QImage & img, bool upsideDown)
{
    IplImage i;
    return i;
}
*/


//IplImage *QImage2IplImage(const QImage *image)
//{
//    IplImage *cvImage = cvCreateImageHeader(cvSize(image->width(), image->height()), IPL_DEPTH_8U, 4);

//    cvImage->imageData = (char*)image->bits();
//    IplImage* colorImage = cvCreateImage(cvGetSize(cvImage), 8, 3);
//    cvConvertImage(cvImage, colorImage, 0);
//    return colorImage;
//}

IplImage* QImage2IplImage(QImage *qimg)
{

    IplImage *imgHeader = cvCreateImageHeader( cvSize(qimg->width(), qimg->height()), IPL_DEPTH_8U, 4);
    imgHeader->imageData = (char*) qimg->bits();
    uchar* newdata = (uchar*) malloc(sizeof(uchar) * qimg->byteCount());
    memcpy(newdata, qimg->bits(), qimg->byteCount());
    imgHeader->imageData = (char*) newdata;
    //cvClo
    return imgHeader;
}

QImage *IplImage2QImage(IplImage *iplImg)
{
    int h = iplImg->height;
    int w = iplImg->width;
    int channels = iplImg->nChannels;
    QImage *qimg = new QImage(w, h, QImage::Format_ARGB32);
    char *data = iplImg->imageData;

    for (int y = 0; y < h; y++, data += iplImg->widthStep)
    {
        for (int x = 0; x < w; x++)
        {
            char r, g, b, a = 0;
            if (channels == 1)
            {
                r = data[x * channels];
                g = data[x * channels];
                b = data[x * channels];
            }
            else if (channels == 3 || channels == 4)
            {
                r = data[x * channels + 2];
                g = data[x * channels + 1];
                b = data[x * channels];
            }

            if (channels == 4)
            {
                a = data[x * channels + 3];
                qimg->setPixel(x, y, qRgba(r, g, b, a));
            }
            else
            {
                qimg->setPixel(x, y, qRgb(r, g, b));
            }
        }
    }
    return qimg;
}
