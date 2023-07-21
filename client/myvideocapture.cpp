#include "myvideocapture.h"
#include "mainwindow.h"

using namespace cv;
using namespace std;

int client_socket;

void detectShapes(cv::Mat frame);

MyVideoCapture::MyVideoCapture(QObject *parent)
    : QThread { parent }
{
}

MyVideoCapture::~MyVideoCapture()
{
    ::close(client_socket);
}

void MyVideoCapture::run()
{

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(CAMERA_PORT);
    inet_pton(AF_INET, IP_ADDRESS, &(server_address.sin_addr));

    std::cout << "Camera";

    if (::connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
        std::cerr << "Failed to connect to the server." << std::endl;
        return;
    }

//    CascadeClassifier faceCascade;
//    faceCascade.load(":/cascades/cascades/beyazkugu.xml");

//    if (faceCascade.empty())
//    {
//        cout << "XML file not loaded" << endl;
//    }
//    CascadeClassifier faceCascade2;
//    faceCascade2.load("C:/Users/Hasan/Downloads/untitled2 (1)/untitled2/untitled2/cascades/siyahkugu.xml");

//    if (faceCascade2.empty())
//    {
//        cout << "XML file not loaded" << endl;
//    }

    std::cout << "IN THREAD";
    std::vector<uint8_t> frame_buffer;
    try
    {
        while (true)
        {

            std::cout << "IN WHILE" << std::endl;

            uint32_t frame_length;
            if (recv(client_socket, reinterpret_cast<char*>(&frame_length), sizeof(frame_length), 0) <= 0)
            {
                std::cerr << "Failed to read frame length." << std::endl;
                break;
            }

            frame_buffer.resize(frame_length);

            uint8_t* frame_data = frame_buffer.data();
            size_t bytes_received = 0;
            while (bytes_received < frame_length)
            {
                ssize_t result = recv(client_socket, reinterpret_cast<char*>(frame_data + bytes_received), frame_length - bytes_received, 0);
                if (result <= 0)
                {
                    std::cout << "EXIT" << std::endl;
                    std::cerr << "Failed to read frame data." << std::endl;
                    break;
                }

                bytes_received += result;
            }

            mFrame = cv::imdecode(frame_buffer, cv::IMREAD_COLOR);
            Mat gray;
            cv::cvtColor(mFrame, gray, COLOR_BGR2GRAY);


//            vector<Rect> white_swan, black_swan;
//            faceCascade.detectMultiScale(gray, white_swan, 1.1, 3, 0, cv::Size(30,30));
//            faceCascade2.detectMultiScale(gray, black_swan, 1.1, 3, 0, cv::Size(30,30));

//            for (const cv::Rect& box : white_swan)
//            {
//                cv::rectangle(mFrame, box, cv::Scalar(0, 0, 255), 2);
//            }

//            for (const cv::Rect& box : black_swan)
//            {
//                cv::rectangle(mFrame, box, cv::Scalar(0, 255, 0), 2);
//            }

            detectShapes(mFrame);


            if (!mFrame.empty())
            {
                mPixmap = cvMatToQPixmap(mFrame);
                emit newPixmapCaptured();
            }

            else
            {
                std::cout << "FRAME EMPTY" << std::endl;
            }

        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    ::close(client_socket);
    //cv::destroyAllWindows();

    return;

}

QImage MyVideoCapture::cvMatToQImage(const cv::Mat &inMat)
{
    switch (inMat.type())
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step), QImage::Format_ARGB32);
        return image;
    }
    // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step), QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    // 8-bit, 1 channel
    case CV_8UC1:
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
        QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step), QImage::Format_Grayscale8);
#else
        static QVector<QRgb>  sColorTable;

        // only create our color table the first time
        if (sColorTable.isEmpty())
        {
            sColorTable.resize(256 );

            for (int i = 0; i < 256; ++i )
            {
                sColorTable[i] = qRgb(i, i, i );
            }
        }

        QImage image(inMat.data,
                     inMat.cols, inMat.rows,
                     static_cast<int>(inMat.step),
                     QImage::Format_Indexed8 );

        image.setColorTable(sColorTable );
#endif
        return image;
    }
    default:
    {
        qWarning()<< "ASM::cvMatToQImage()- cv::Mat image type not handled in switch:" << inMat.type();
        break;
    }
    }
    return QImage();
}

QPixmap MyVideoCapture::cvMatToQPixmap(const cv::Mat &inMat)
{
    return QPixmap::fromImage(cvMatToQImage(inMat));
}

void detectShapes(cv::Mat frame)
{
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);

    cv::Canny(gray, gray, 50, 150);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(gray, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);


    for (size_t i = 0; i < contours.size(); i++)
    {
        double area = cv::contourArea(contours[i]);
        if (area < 100)
            continue;

        std::vector<cv::Point> approx;
        cv::approxPolyDP(contours[i], approx, cv::arcLength(contours[i], true) * 0.02, true);

        if (approx.size() == 4)
        {
            cv::Rect rect = cv::boundingRect(approx);
            double aspectRatio = static_cast<double>(rect.width) / rect.height;
            if (aspectRatio >= 0.9 && aspectRatio <= 1.1)
            {
                cv::drawContours(frame, contours, static_cast<int>(i), cv::Scalar(255, 0, 0), 2);
                cv::putText(frame, "Square", approx[0], cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2);
            }
            else
            {
                cv::drawContours(frame, contours, static_cast<int>(i), cv::Scalar(0, 0, 255), 2);
                cv::putText(frame, "Rectangle", approx[0], cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 2);
            }
        }
        else if (approx.size() == 3)
        {
            cv::drawContours(frame, contours, static_cast<int>(i), cv::Scalar(0, 255, 0), 2);
            cv::putText(frame, "Triangle", approx[0], cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 255), 2);
        }


    }
}




