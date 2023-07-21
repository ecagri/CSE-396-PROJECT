#include <QtCore>
#include <QImage>
#include <vector>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <clocale>
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "MyThread.h"
#include "mainwindow.h"

MyThread::MyThread(QObject *parent) : QThread(parent)
{

}

void MyThread::run()
{

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9024);
    inet_pton(AF_INET, "192.168.152.150", &(server_address.sin_addr));

    if (::connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
        std::cerr << "Failed to connect to the server." << std::endl;
        return;
    }

    std::vector<uint8_t> frame_buffer;
    try
    {
        while (true)
        {
            uint32_t frame_length;
            if (read(client_socket, &frame_length, sizeof(frame_length)) < 0)
            {
                std::cerr << "Failed to read frame length." << std::endl;
                break;
            }

            frame_buffer.resize(frame_length);

            uint8_t* frame_data = frame_buffer.data();
            size_t bytes_received = 0;
            while (bytes_received < frame_length)
            {
                ssize_t result = read(client_socket, frame_data + bytes_received, frame_length - bytes_received);
                if (result < 0)
                {
                    std::cerr << "Failed to read frame data." << std::endl;
                    break;
                }

                bytes_received += result;
            }

            cv::Mat frame = cv::imdecode(frame_buffer, cv::IMREAD_COLOR);

            QImage qImage(frame.data, frame.cols, frame.rows, static_cast<int>(frame.step), QImage::Format_RGB888);

            if (qImage.format() != QImage::Format_RGB888)
            {
                qImage = qImage.convertToFormat(QImage::Format_RGB888);
            }

            //MainWindow::label->setPixmap(QPixmap::fromImage(qImage));

            //cv::imshow("Video", frame);

            if (cv::waitKey(1) == 'q')
            {
                break;
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    ::close(client_socket);
    cv::destroyAllWindows();

    return;

    std::cout << "IN THREAD!";
}
