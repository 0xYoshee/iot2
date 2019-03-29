#ifndef MOTIONDETECTOR_H
#define MOTIONDETECTOR_H

#include <QObject>
#include <QLabel>

#include <QDebug>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class MotionDetector : public QObject
{
    Q_OBJECT
public:
    explicit MotionDetector(quint32 area=400, quint16 width=640, quint16 height=480, QObject *parent = nullptr);
    ~MotionDetector();

    void input(QImage *image);
    void input2(QImage *image);

//    static void howImage(Mat img, QLabel *lbl);
    QImage Mat2QImage(Mat img);

    Mat matOriginal() const;

    void setMinArea(const quint32 &minArea);

signals:
    void error(QString err);
    void motion(QImage img);

public slots:

private:
    quint32 mMinArea;
    Size mSizeImg;

    Mat mMatFrame;
    Mat mMatBack;
    Mat mMatFore;
    Mat mMatFramePre;
    Mat mMatOriginal;
    Mat mMatMask;

//    BackgroundSubtractorMOG2 mog2;
    vector<vector<Point> > contours;
};

#endif // MOTIONDETECTOR_H
