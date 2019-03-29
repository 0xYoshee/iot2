#include "motiondetector.h"

MotionDetector::MotionDetector(quint32 area, quint16 width, quint16 height, QObject *parent)
    : QObject(parent)
{
    //mog2.nmixtures=3;
    //mog2.bShadowDetection=false;
    mMinArea=400;
    mSizeImg = Size(width, height);
}

MotionDetector::~MotionDetector()
{

}

void MotionDetector::input2(QImage *image)
{
    if(mMatFramePre.empty()) {
        mMatFramePre = Mat(image->height(), image->width(), CV_8UC3, (uchar*)image->bits(), image->bytesPerLine()).clone();
        cvtColor(mMatFramePre, mMatFramePre, COLOR_BGR2GRAY);
        return;
    }

    mMatFrame = Mat(image->height(), image->width(), CV_8UC3, (uchar*)image->bits(), image->bytesPerLine()).clone();
    cvtColor(mMatFrame, mMatFrame, CV_BGR2RGB);

    //    cv::flip(mMatFrame,mMatFrame,1);
    mMatOriginal=mMatFrame.clone();

    cvtColor(mMatFrame,mMatFrame,CV_RGB2GRAY);
    blur(mMatFrame,mMatFrame,Size(5,5));

    cv::absdiff(mMatFramePre,mMatFrame,mMatFore);
    cv::threshold(mMatFore,mMatFore,40,255,0);

//    Mat element=getStructuringElement(MORPH_ELLIPSE,Size(10,10));
//    morphologyEx(matFore,matFore,MORPH_DILATE,element);
//    morphologyEx(matFore,matFore,MORPH_CLOSE,element);
//    matBack=matFore.clone();

    findContours(mMatFore,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
    vector<vector<Point> > ::iterator itc = contours.begin();

    bool status = false;

    while(itc!=contours.end()) {
        if(contourArea(*itc)>mMinArea && contourArea(*itc)<100000) {
            status = true;
            Rect tempRect = boundingRect((Mat(*itc)));
            rectangle(mMatOriginal,tempRect,Scalar(0,255,0),2);
        }
        ++itc;
    }

    if(status){
        QImage img = Mat2QImage(mMatOriginal);
        emit motion(img);
    }

    mMatFramePre=mMatFrame.clone();
}

//void MotionDetector::input(QImage *image)
//{
//    mMatFrame = Mat(image->height(), image->width(), CV_8UC3, (uchar*)image->bits(), image->bytesPerLine()).clone();
//    cvtColor(mMatFrame, mMatFrame, CV_BGR2RGB);

//    if(mMatFrame.empty()) {
//        emit error("Empty frame!!!");
//        return;
//    }

//    //MOG2 bkg subtractor algo
//    mog2.operator ()(mMatFrame,mMatFore);
////    mog2.getBackgroundImage(mMatBack);
//    cv::erode(mMatFore,mMatFore,cv::Mat());
//    cv::dilate(mMatFore,mMatFore,cv::Mat());

//    findContours(mMatFore,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
//    vector<vector<Point> > ::iterator itc = contours.begin();

//    //approxPolyDP(contours,contours,20,true);

////        cout<<"No of contours : "<<contours.size()<<endl;
//    //Vector<RotatedRect> rects;

//    bool status = false;

//    while(itc!=contours.end()) {
//        //RotatedRectRot tempRect=minAreaRect(Mat(*itc));
//        if(contourArea(*itc)>mMinArea) {
//            status = true;
//            Rect tempRect = boundingRect((Mat(*itc)));
//            mMatOriginal=mMatFrame.clone();
//            rectangle(mMatOriginal,tempRect,Scalar(0,255,0),2);
//        }
//        ++itc;
//    }

//    if(status){
//        QImage img = Mat2QImage(mMatOriginal);
//        emit motion(img);
//    }
//}

QImage MotionDetector::Mat2QImage(Mat img) {
    if(img.empty()) {
//        cout<<"Empty image to show!!!"<<endl;
        return QImage();
    }
    QImage Qimgbkg;
    if(img.type()==CV_8UC3) {
        cvtColor(img,img,CV_BGR2RGB);
        QImage Qimgbkg1((uchar*)img.data,img.cols,img.rows,img.step,QImage::Format_RGB888);
        Qimgbkg = Qimgbkg1;
    }
    else if(img.type()==CV_8UC4) {
        cvtColor(img,img,CV_BGRA2RGBA);
        QImage Qimgbkg1((uchar*)img.data,img.cols,img.rows,img.step,QImage::Format_RGBA8888);
        Qimgbkg = Qimgbkg1;
    }
    else {
        QImage Qimgbkg1((uchar*)img.data,img.cols,img.rows,img.step,QImage::Format_Indexed8);
        Qimgbkg=Qimgbkg1;
    }

    return Qimgbkg;
}

Mat MotionDetector::matOriginal() const
{
    return mMatOriginal;
}

void MotionDetector::setMinArea(const quint32 &minArea)
{
    mMinArea = minArea;
}

//static void MotionDetector::showImage(Mat img, QLabel *lbl) {
//    if(img.empty()) {
////        cout<<"Empty image to show!!!"<<endl;
//        return;
//    }
//    QImage Qimgbkg;
//    if(img.type()==CV_8UC3) {
//        cvtColor(img,img,CV_BGR2RGB);
//        QImage Qimgbkg1((uchar*)img.data,img.cols,img.rows,img.step,QImage::Format_RGB888);
//        Qimgbkg = Qimgbkg1;
//    }
//    else if(img.type()==CV_8UC4) {
//        cvtColor(img,img,CV_BGRA2RGBA);
//        QImage Qimgbkg1((uchar*)img.data,img.cols,img.rows,img.step,QImage::Format_RGBA8888);
//        Qimgbkg = Qimgbkg1;
//    }
//    else {
//        QImage Qimgbkg1((uchar*)img.data,img.cols,img.rows,img.step,QImage::Format_Indexed8);
//        Qimgbkg=Qimgbkg1;
//    }

//    QPixmap pixmap1 = QPixmap::fromImage(Qimgbkg);
////    ui->lCamera->setPixmap(pixmap1.scaled(ui->lCamera->size(),Qt::KeepAspectRatio));
////    lbl->setPixmap(QPixmap::fromImage(Qimgbkg));
//    lbl->setPixmap(pixmap1.scaled(lbl->size(),Qt::KeepAspectRatio));
//}
