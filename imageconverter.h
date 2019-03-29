#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H

#include <QImage>

//#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
//#include "opencv2/photo.hpp"
//#include "opencv2/video.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/features2d.hpp"
//#include "opencv2/calib3d.hpp"
//#include "opencv2/objdetect.hpp"

QImage Ipl2QImage(const IplImage*, bool upsideDown);
// TODO IplImage QImage2Ipl(const QImage*, bool upsideDown);

//IplImage *QImage2IplImage(const QImage *image);

IplImage* QImage2IplImage(QImage *qimg);
QImage *IplImage2QImage(IplImage *iplImg);
#endif

