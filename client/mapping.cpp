#include "mapping.h"
std::vector<std::pair<int, int> > allPoints;
Mapping::Mapping(QObject *parent)
    : QThread { parent }
{
    autonom = new Autonom();
}
std::vector<std::pair<int, int> > Mapping::stringToVector(const std::string& data) {
    std::vector<std::pair<int, int> > result;
    std::string temp = data;

    // Remove leading and trailing square brackets
    temp.erase(std::remove(temp.begin(), temp.end(), '['), temp.end());
    temp.erase(std::remove(temp.begin(), temp.end(), ']'), temp.end());

    //std::cout << temp<<std::endl;
    std::istringstream iss(temp);
    std::string token;

    std::getline(iss, token, '(');

    while (std::getline(iss, token, '(')) {
        std::istringstream subiss(token);
        std::string pairToken;

        // Remove leading and trailing parentheses
        pairToken = token.substr(token.find('(') + 1, token.find(')') - token.find('(') - 1);

        std::istringstream subsubiss(pairToken);
        std::string first, second;

        //std::cout << pairToken;

        // Split the pairToken using comma delimiter
        std::getline(subsubiss, first, ',');
        std::getline(subsubiss, second, ',');

        //std::cout << "First: " << first << std::endl;
        //std::cout << "Second: " << second << std::endl;

        if(first == "-" || second == "-" || first == "" || second == "")
        {
            continue;
        }

        int firstValue;
        int secondValue;

        try
        {
            firstValue = std::stoi(first);
            secondValue = std::stoi(second);
            result.push_back(std::make_pair(firstValue, secondValue));
        }
        catch(const std::exception& e)
        {
            std::cerr << "Stoi Exception: " << e.what() << std::endl;
        }
    }
    return result;
}

void Mapping::run()
{
    // Initialize the map outside the loop
    degree = 0;
    positionX = 500;
    positionY = 500;

    int sizeX = 300;
    int sizeY = 150;

    for(int i=0; i<1000; i++)
    {
        for(int j=0;j<1000;j++)
        {
            harita[i][j]=0;
        }
    }

    // for windows
    #ifdef WINDOWS_PORT
    WSADATA wsaData;
    // Initialize Winsock for windows
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "Failed to initialize Winsock." << std::endl;
    }
    #endif    

    std::vector<int> data_points;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //std::cout<<sockfd<<endl;
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(MAPPING_PORT);
    if (inet_pton(AF_INET, IP_ADDRESS, &serverAddress.sin_addr)
        <= 0) {
        std::cout<<"error"<<std::endl;
         }
    ::connect(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));


//         for (int i = 450; i < 650; i++) {
//            for (int j = 450; j < 650; j++) {
//                map_array[i][j] = 0;
//            }
//         }

//         for (int i = 525; i < 575; i++) {
//            for (int j = 525; j < 575; j++) {
//                map_array[i][j] = 1;
//            }
//         }
//        for (int i = 440; i < 450; i++) {
//            for (int j = 490; j < 510; j++) {
//                map_array[i][j] = 1;
//            }
//            map_array[500][i]=1;
//        }
/*
         // Forward path
         for (int i = start_y - path_length - 50; i < start_y; i++) {
        for (int j = start_x; j < start_x + path_width; j++) {
            map_array[i][j] = 0;
        }
         }

         // Right turn
         for (int i = start_y - path_length - path_width; i < start_y - path_length; i++) {
        for (int j = start_x + path_width; j < start_x + path_width * 4; j++) {
            map_array[i][j] = 0;
        }
         }*/

    while (true) {
        // Receive data from socket
        // TODO: Implement socket communication in C++
        // For example, you can use the following code to receive data from a socket:


//        std::cout << "eklendi";
        char data[1024];
        memset(data,0,1024);
        ssize_t bytesRead = recv(sockfd,data,1023,0);
        if(bytesRead == -1){
            //std::cout << "error" << endl;
        }

        //std::cout<<data;f
        //std::cout << endl;

        std::string data_string = data;
        //cout << data_string << endl << endl << endl  ;

        std::vector<std::pair<int, int> > points = stringToVector(data_string);
        std::vector<std::pair<int, int> > newPoints;

        // Elde edilen vektörü kullanarak istediğiniz işlemi gerçekleştirin

        int row = 150;
        int column = 300;
        int ratio = 1;

        int map[150][300];

        // Diziyi ekrana yazdırma
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < column; j++) {
                map[i][j]=0 ;
            }
        }

       // std::cout << "eklendi";

        for (const auto& point : points) {
            //qDebug() << "("<<point.first << "," <<int((point.second)) << ")";

            // if points 0,0 don't put 1
            if(point.first == 0 && point.second == 0)
            {
                continue;
            }

            /*else if(point.first < 5 && point.second < 5)
            {
                continue;
            }*/

            // if it is less than 0, don't put 1
            if (point.second/ratio - 1 > row)
                continue;

            map[row-int(point.second/ratio)-1][int((point.first+150)/ratio)] = 1;

        }
        map[0][150] = 0;
        map[1][151] = 0;

        double radian= degree*(M_PI/180);


        switch(degree%360){
            case 0:
                break;
            case -90:
                positionY += 300;
                break;
            case -180:
                positionX += 300;
                positionY += 300;
                break;
            case -270:
                positionX += 300;
                break;
            case 90:
                positionX += 300;
                break;
            case 180:
                positionX += 300;
                positionY += 300;
                break;
            case 270:
                positionY += 300;
                //positionX -= 300;
                break;
        }

        for(int i = sizeY -1 ; i >= 0; i--){
            for(int j = 0; j < sizeX; j++){
//                if(i < positionX+20 && i+500 > positionX-20 && ( j > positionY-20 && j < positionY+20 ) ){
//                    int realX = round(j*cos(radian) - i*sin(radian));
//                    int realY = round(j*sin(radian) + i*cos(radian));

//                    if(positionX + realX < 1000 && positionY + realY < 1000){
//                        if(positionX + realX >= 0 && positionY + realY >= 0){
//                            harita[positionY + realY][positionX + realX] = 0;
//                        }

//                    }
//                }
                 if(map[i][j] == 1){

                    int realX = round(j*cos(radian) - i*sin(radian));
                    int realY = round(j*sin(radian) + i*cos(radian));


                    if(positionX + realX < 1000 && positionY + realY < 1000){
                        if(positionX + realX >= 150 && positionY + realY >= 150) {
                            harita[positionY + realY - 150][positionX + realX - 150] = 1;
                            std::pair<int,int> dot = std::make_pair(positionY + realY - 150,positionX + realX - 150);
                            newPoints.push_back(dot);
                            allPoints.push_back(dot);
                        }
                    }
                }
                 else if(j > 145 && j < 155 && i > 8){
                    int realX = round(j*cos(radian) - i*sin(radian));
                    int realY = round(j*sin(radian) + i*cos(radian));

                    if(positionX + realX < 1000 && positionY + realY < 1000){
                        if(positionX + realX >= 150 && positionY + realY >= 150 && harita[positionY + realY - 150][positionX + realX - 150]!=4){

                            harita[positionY + realY - 150][positionX + realX - 150] = 0;
                            std::pair<int,int> elementValueToRemove = std::make_pair(positionY + realY - 150,positionX + realX - 150);
                            allPoints.erase(std::remove(allPoints.begin(), allPoints.end(), elementValueToRemove), allPoints.end());
                        }
                    }
                }
                else{
                    int realX = round(j*cos(radian) - i*sin(radian));
                    int realY = round(j*sin(radian) + i*cos(radian));

                    if(positionX + realX < 1000 && positionY + realY < 1000){
                        if(positionX + realX >= 150 && positionY + realY >= 150){
                            //qDebug() << realX << endl;
                            if(j >= i && j <= 300 - i) {
                                //if(harita[positionY + realY - 150][positionX + realX - 150] != 4)
                                //    harita[positionY + realY - 150][positionX + realX - 150] = 0;
                            }
                        }

                    }
                }
            }
        }

        switch(degree%360){
            case 0:
                break;
            case -90:
                positionY -= 300;
                break;
            case -180:
                positionX -= 300;
                positionY -= 300;
                break;
            case -270:
                positionX -= 300;
                break;
            case 90:
                positionX -= 300;
                break;
            case 180:
                positionX -= 300;
                positionY -= 300;
                break;
            case 270:
                positionY -= 300;
                //positionX += 300;
                break;
        }

        beautifyMap(harita, allPoints, newPoints, 10);
    }
}

void Mapping::connect(int map[1000][1000], int row, int col, int newRow, int newCol) {
    while (row != newRow || col != newCol) {
        if (newRow < row) {
            newRow++;
        }
        else if (newRow > row) {
            newRow--;
        }

        if (newCol < col) {
            newCol++;
        }
        else if (newCol > col) {
            newCol--;
        }

        if (map[newRow][newCol] != 1) {
            map[newRow][newCol] = 1;
        }

    }

}


void Mapping::beautifyMap(int map[1000][1000], std::vector<std::pair<int, int>> oldPoints, std::vector<std::pair<int, int>> newPoints, int distance) {
    int x, y;

    for (int i = 0; i < oldPoints.size(); i++) {
        for (int k = 0; k < newPoints.size(); k++) {
            x = oldPoints[i].first - newPoints[k].first;
            y = oldPoints[i].second - newPoints[k].second;
            if (x < 0)
                x *= -1;
            if (y < 0)
                y *= -1;

            if (x <= distance && y <= distance)
                connect(map, oldPoints[i].first, oldPoints[i].second, newPoints[k].first, newPoints[k].second);
        }
    }

    return;
}
