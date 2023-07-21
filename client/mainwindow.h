#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Forward declaration of MyVideoCapture class
class MyVideoCapture;


// for windows
#ifndef WINDOWS_PORT
#define WINDOWS_PORT
#endif

// for mac and ubuntu
// #ifndef POSIX_PORT
// #define POSIX_PORT
// #endif

#ifdef WINDOWS_PORT
#define _HAS_STD_BYTE 0
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#endif

#ifdef POSIX_PORT
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QPainter>
#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <unistd.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QDebug>
#include <QtCore>
#include <QImage>
#include <cerrno>
#include <clocale>
#include <cstring>
#include <algorithm>
#include <queue>
#include <QMouseEvent>
#include "autonom.h"
#include "util.h"


class MainWindow;

extern int a;
extern int degree;
extern int d;
extern int result;
extern int positionX;
extern int positionY;
extern int harita[1000][1000];
extern int clickX;
extern int clickY;
extern std::vector<std::pair<int, int>> allPoints;
extern std::vector<std::pair<int, int>> path;
extern MainWindow *w_ptr;
class QPushButton;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void mousePressEvent(QMouseEvent *event);
private slots:
    void updateScreen();

    void on_rotate_left_button_clicked();

    void on_rotate_right_button_clicked();

    void on_forward_button_clicked();

    void on_back_button_clicked();

    void on_move_left_button_clicked();

    void on_move_right_button_clicked();
    void on_rotate_right_button_2_clicked();
    void on_rotate_right_button_3_clicked();

    void on_rotate_right_button_4_clicked();

    void on_rotate_left_button_2_clicked();

private:
    Ui::MainWindow *ui;
    MyVideoCapture *mOpenCV_videoCapture;
    QTimer m_timer;
    void *autonom;


};
#endif // MAINWINDOW_H
