#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myvideocapture.h"
//#include "mythread.h"

int a = 0;
int positionX;
int positionY;
int clickX;
int clickY;
int degree = 0;

int d = 0;
int result = 0;
int counter = 0;
int lastDegree = NULL;
int harita[1000][1000];
bool refresh = true; //should be false
int lastDirection;
vector<pair<int, int>> path;
vector<string> directions;

#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480

int status, valread, client_fd;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    mOpenCV_videoCapture = new MyVideoCapture(this);

    connect(mOpenCV_videoCapture, &MyVideoCapture::newPixmapCaptured, this, [&]()
            {
                ui->image->setPixmap(mOpenCV_videoCapture->pixmap().scaled(500, 500));
            });

    autonom = new Autonom;

    #ifdef WINDOWS_PORT
    // for windows
    WSADATA wsaData;

    // initialize Winsock for windows
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "Failed to initialize Winsock." << std::endl;
    }
    #endif

    connect(&m_timer, &QTimer::timeout, this, &MainWindow::updateScreen);
    m_timer.start(1000);

    ui->setupUi(this);

    mOpenCV_videoCapture->start(QThread::HighestPriority);

    std::string connectString = IP_ADDRESS;
    std::string portString = MOVEMENT_PORT;

    struct sockaddr_in serv_addr;
    char buffer[1024] = { 0 };

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return;
    }

    std::cout << "IP: " << connectString << std::endl;
    std::cout << "PORT: " << portString << std::endl;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(std::stoi(portString));

    // for windows
    #ifdef WINDOWS_PORT
    wchar_t* wString = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, connectString.c_str(), -1, wString, 4096);
    if (InetPtonW(AF_INET, wString, &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return;
    }
    #endif

    #ifdef POSIX_PORT
    if (inet_pton(AF_INET, connectString.c_str(), &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return;
    }
    #endif

    if ((status = ::connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0)
    {
        printf("\nConnection Failed \n");
        std::cout << "Or, in German, " << std::strerror(errno) << '\n';
        return;
    }
}

MainWindow::~MainWindow()
{
    ::close(client_fd);
    delete ui;
}
void MainWindow::updateScreen()
{
    QImage mapImage(750, 750, QImage::Format_RGB888);



    // cast void* autonom to Autonom*

    qDebug() << "degree changed!: "<<degree;



    ((Autonom*) autonom)->run();
    if(clickX == NULL && clickY == NULL)
        directions.clear();
    else
        ((Autonom*) autonom)->getDirections(directions);

    if (directions.size() > 0){
        int newDirection;

        for(int i = 0; i < 10 && i <  directions.size(); i++){
            qDebug() << directions[i] << "-->";
        }
        qDebug() << "BOŞLUK";

        if(directions[0] == "FORWARD"){
            newDirection = 1;
        }else if(directions[0] == "RIGHT"){
            newDirection = 2;
        }else if(directions[0] == "LEFT"){
            newDirection = -2;
        }else{
            newDirection = -1;
        }
        int newDegree = degree;
        while(newDegree < 0){
            newDegree += 360;
        }
        if(newDirection * - 1 == lastDirection){
            string currentDirection = directions[0];
            while(directions[0] == currentDirection){
                directions.erase(directions.begin());
                switch(newDegree%360){
                    case 0:
                        positionY += 1;
                        break;
                    case 90:
                        positionX -= 1;
                        break;
                    case 180:
                        positionY += 1;
                        break;
                    case 270:
                        positionX += 1;
                        break;
                    }
            }
        }
        qDebug() << directions[0] << "--";
        if (directions[0] == "FORWARD"){
            switch(newDegree%360){
                case 0:
                {
                    bool turn = true;
                    for(int i = 1; i < 8 && turn == true; i++){
                        if(directions[i] == "RIGHT"){
                            on_rotate_right_button_clicked();
                            //on_forward_button_clicked();
                            turn = false;

                        }
                        else if(directions[i] == "LEFT"){
                            on_rotate_left_button_clicked();
                            turn = false;

                        }
                    }
                    if(turn == true)
                        on_forward_button_clicked();
                    break;
                }
                case 270:
                    on_rotate_right_button_clicked();
                    break;
                case 90:
                    on_rotate_left_button_clicked();
                    break;
                case 180:
                    on_back_button_clicked();
                    break;
                }

        }
        else if (directions[0] == "LEFT"){
            switch(newDegree%360){
                case 0:
                    on_rotate_left_button_clicked();
                    counter=1;
            case 270:
                {
                bool turn = true;
                for(int i = 1; i < 8 && turn == true && counter == 0; i++){
                    if(directions[i] == "FORWARD"){
                        on_rotate_right_button_clicked();
                        turn = false;
                    }
                }
                if(turn == true)
                    on_forward_button_clicked();

                if(counter)
                    counter = 0;
                break;
            }

            case 180:
                on_rotate_right_button_clicked();
                break;
            case 90:
                on_forward_button_clicked();
                break;
            }

        }
        else if (directions[0] == "RIGHT"){
            switch(newDegree%360){
                case 0:
                    on_rotate_right_button_clicked();
                    counter = 1;
                case 270:
                    on_forward_button_clicked();
                    break;
                case 180:
                    on_rotate_left_button_clicked();
                    break;
                case 90:
                    {
                        bool turn = true;
                        for(int i = 1; i < 8 && turn == true && counter == 0; i++){
                            if(directions[i] == "FORWARD"){
                                on_rotate_left_button_clicked();
                                turn = false;
                            }
                        }
                        if(turn == true)
                            on_forward_button_clicked();
                        if(counter == 1)
                            counter = 0;
                        break;
                    }
            }
        }
        else if (directions[0] == "BACK"){
            switch(newDegree%360){
                case 0:
                    on_rotate_right_button_clicked();
                    on_rotate_right_button_clicked();
                    break;
                case 90:
                    on_rotate_right_button_clicked();
                    break;
                case 180:
                    on_forward_button_clicked();
                    break;
                case 270:
                    on_rotate_left_button_clicked();
                    break;
            }


        }
        lastDirection = newDirection;

    }



    for (int i = 0; i < 750; i++)
    {
        for (int j = 0; j < 750; j++)
        {
            mapImage.setPixel(j , i, qRgb(255, 255, 255));
        }
    }

    for (int i = 0; i < 1000; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            if(harita[i][j] == 0)
            {
                mapImage.setPixel(j * 0.75, i * 0.75, qRgb(255, 255, 255));
            }
            else if(harita[i][j] == 1)
            {
                mapImage.setPixel((j) * 0.75, (i) * 0.75, qRgb(255, 0, 0));
                mapImage.setPixel((j) * 0.75 + 1, (i) * 0.75, qRgb(255, 0, 0));
                mapImage.setPixel((j) * 0.75 + 1, (i) * 0.75 + 1, qRgb(255, 0, 0));
                mapImage.setPixel((j) * 0.75, (i) * 0.75 - 1, qRgb(255, 0, 0));
                mapImage.setPixel((j) * 0.75, (i) * 0.75 + 1, qRgb(255, 0, 0));
                mapImage.setPixel((j) * 0.75 + 1, (i) * 0.75 - 1, qRgb(255, 0, 0));
                mapImage.setPixel((j) * 0.75 - 1, (i) * 0.75, qRgb(255, 0, 0));
                mapImage.setPixel((j) * 0.75 - 1, (i) * 0.75 + 1, qRgb(255, 0, 0));
                mapImage.setPixel((j) * 0.75 - 1, (i) * 0.75 - 1, qRgb(255, 0, 0));
            }
            else if(harita[i][j] == 4)
            {
                mapImage.setPixel((j) * 0.75, (i) * 0.75, qRgb(0, 255, 0));
                mapImage.setPixel((j) * 0.75 + 1, (i) * 0.75, qRgb(0, 255, 0));
                mapImage.setPixel((j) * 0.75 + 1, (i) * 0.75 + 1, qRgb(0, 255, 0));
                mapImage.setPixel((j) * 0.75, (i) * 0.75 - 1, qRgb(0, 255, 0));
                mapImage.setPixel((j) * 0.75, (i) * 0.75 + 1, qRgb(0, 255, 0));
                mapImage.setPixel((j) * 0.75 + 1, (i) * 0.75 - 1, qRgb(0, 255, 0));
                mapImage.setPixel((j) * 0.75 - 1, (i) * 0.75, qRgb(0, 255, 0));
                mapImage.setPixel((j) * 0.75 - 1, (i) * 0.75 + 1, qRgb(0, 255, 0));
                mapImage.setPixel((j) * 0.75 - 1, (i) * 0.75 - 1, qRgb(0, 255, 0));
                harita[i][j] = 0;
            }
//             else if(harita[i][j] == 5)
//             {
//                 mapImage.setPixel((j) * 0.75, (i) * 0.75, qRgb(0, 0, 255));
//                 mapImage.setPixel((j) * 0.75 + 1, (i) * 0.75, qRgb(0, 0, 255));
//                 mapImage.setPixel((j) * 0.75 + 1, (i) * 0.75 + 1, qRgb(0, 0, 255));
//                 mapImage.setPixel((j) * 0.75, (i) * 0.75 - 1, qRgb(0, 0, 255));
//                 mapImage.setPixel((j) * 0.75, (i) * 0.75 + 1, qRgb(0, 0, 255));
//                 mapImage.setPixel((j) * 0.75 + 1, (i) * 0.75 - 1, qRgb(0, 0, 255));
//                 mapImage.setPixel((j) * 0.75 - 1, (i) * 0.75, qRgb(0, 0, 255));
//                 mapImage.setPixel((j) * 0.75 - 1, (i) * 0.75 + 1, qRgb(0, 0, 255));
//                 mapImage.setPixel((j) * 0.75 - 1, (i) * 0.75 - 1, qRgb(0, 0, 255));
//                 harita[i][j] = 0;
//             }

        }
    }

    for(int i = -3; i <= 3; i++)
    {
        for(int j = -3; j <= 3; j++)
        {
            mapImage.setPixel((positionX + i) * 0.75, (positionY + j) * 0.75, qRgb(0, 0, 255));
        }
    }


    QPixmap mapPixMap();
    ui->map->setPixmap(QPixmap::fromImage(mapImage).scaled(750, 750));
    ui->map->resize(750, 750);
    update();
    lastDegree = degree;
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        clickX = event->x();
        clickY = event->y();
        qDebug("Mouse clicked at (%d, %d)", int(clickY * 1.33) - 20, int(clickX * 1.33) - 570);
    }
}

void MainWindow::on_rotate_left_button_clicked()
{

    send(client_fd, "LEFT,LEFT", 9, 0);

    // receive feedback
    char buffer[1024] = {0};
    ssize_t valread = recv(client_fd, buffer, 1024, 0);

    if (valread < 0)
    {
        // get the error
        qDebug() << "!!!Error-Left: " << strerror(errno);
        return;
    }

    degree -= 90;
    qDebug() << "-LEFT- valread: " << valread;
}

void MainWindow::on_rotate_right_button_clicked()
{

    send(client_fd, "RIGHT,RIGHT", 11, 0);

    // receive feedback
    char buffer[1024] = {0};
    ssize_t valread = recv(client_fd, buffer, 1024, 0);

    if (valread < 0)
    {
        // get the error
        qDebug() << "!!!Error-Right: " << strerror(errno);
        return;
    }
    degree += 90;
    qDebug() << "-RIGHT- valread: " << valread;

}


void MainWindow::on_forward_button_clicked()
{
    double radian= degree*(M_PI/180);
    qDebug() << "degree: " << degree;


    send(client_fd, "FORWARD,FORWARD", 15, 0);

    // receive feedback
    char buffer[1024] = {0};
    
    ssize_t valread = recv(client_fd, buffer, 1024, 0);

    if (valread < 0 && buffer == "NOTRECEIVED")
    {
        // get the error
        qDebug() << "!!!Error: " << strerror(errno);
        return;
    }

    qDebug() << "---valread: " << valread;
    qDebug() << "---buffer: " << buffer;

    positionY -= round(7 * cos(radian));
    positionX += round(7 * sin(radian));

}


void MainWindow::on_back_button_clicked()
{
    send(client_fd, "BACK,BACK", 9, 0);

    qDebug() << "--Back Clicked--";
    // receive feedback
    char buffer[1024] = {0};
    ssize_t valread = recv(client_fd, buffer, 1024, 0);
}

void MainWindow::on_move_left_button_clicked()
{
    double radian= degree*(M_PI/180);

    send(client_fd, "STEPLEFT,STEPLEFT", 17, 0);

    // receive feedback
    char buffer[1024] = {0};
    ssize_t valread = recv(client_fd, buffer, 1024, 0);

    if (valread < 0)
    {
        // get the error
        qDebug() << "!!!Error STEPLEFT: " << strerror(errno);
        return;
    }

    qDebug() << "-LEFT- valread: " << valread;
    positionY -= round(8 * sin(radian));
    positionX -= round(8 * cos(radian));   
}


void MainWindow::on_move_right_button_clicked()
{
    double radian= degree*(M_PI/180);

    send(client_fd, "STEPRIGHT,STEPRIGHT", 19, 0);

    // receive feedback
    char buffer[1024] = {0};
    ssize_t valread = recv(client_fd, buffer, 1024, 0);

    if (valread < 0)
    {
        // get the error
        qDebug() << "!!!Error STEPRIGHT: " << strerror(errno);
        return;
    }

    qDebug() << "-RIGHT- valread: " << valread;
    positionY += round(8 * sin(radian));
    positionX += round(8 * cos(radian));
}

void MainWindow::on_rotate_right_button_2_clicked()
{
    for(int i = 0; i < 1000; i++){
        for(int j = 0; j < 1000; j++){
            harita[i][j] = 0;
        }
    }
    degree = 0;
    positionX = 500;
    positionY = 500;
    clickY = NULL;
    clickX = NULL;
    directions.clear();
    allPoints.clear();
}

void MainWindow::on_rotate_right_button_3_clicked()
{
    clickY = NULL;
    clickX = NULL;
    directions.clear();

}


void MainWindow::on_rotate_right_button_4_clicked()
{
    send(client_fd, "TURNRIGHT,TURNRIGHT", 19, 0);
    char buffer[1024] = {0};
    ssize_t valread = recv(client_fd, buffer, 1024, 0);

    if (valread < 0)
    {
        // get the error
        qDebug() << "!!!Error TURNRIGHT: " << strerror(errno);
        return;
    }
}


void MainWindow::on_rotate_left_button_2_clicked()
{
    send(client_fd, "TURNLEFT,TURNLEFT", 19, 0);
    char buffer[1024] = {0};
    ssize_t valread = recv(client_fd, buffer, 1024, 0);

    if (valread < 0)
    {
        // get the error
        qDebug() << "!!!Error TURNLEFT: " << strerror(errno);
        return;
    }
}

