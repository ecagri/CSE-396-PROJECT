#ifndef MYVIDEOCAPTURE_H
#define MYVIDEOCAPTURE_H

#include <QPixmap>
#include <QImage>
#include <QThread>
#include <QDebug>
#include <QtCore>
#include "mainwindow.h"
#include <opencv2/opencv.hpp>


#define ID_CAMERA 0

class MyVideoCapture : public QThread
{
    Q_OBJECT
public:
    MyVideoCapture(QObject *parent = nullptr);
    ~MyVideoCapture();

    QPixmap pixmap() const
    {
        return mPixmap;
    }
signals:
    void newPixmapCaptured(); // imagen nueva ha sido capturada
protected:
    void run() override;
private:
    QPixmap mPixmap; // imagen de Qt
    cv::Mat mFrame; // imagen de OpenCV
    cv::VideoCapture mVideoCap; // capturador de video

    QImage cvMatToQImage(const cv::Mat &inMat);
    QPixmap cvMatToQPixmap(const cv::Mat &inMat);
};

#endif // MYVIDEOCAPTURE_H
