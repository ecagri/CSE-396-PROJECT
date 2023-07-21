#include "autonom.h"

using namespace std;

Direction directions_temp[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
Direction directions_temp0[] = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}}; // FORWARD // LEFT
Direction directions_temp1[] = {{0, -1}, {-1, 0}, {1, 0}, {0, 1}}; // LEFT // FORWARD
Direction directions_temp2[] = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}}; // RIGHT // BACK
Direction directions_temp3[] = {{1, 0}, {0, -1}, {0, 1}, {-1, 0}}; // BACK // RIGHT



Autonom::Autonom()
{
}

int Autonom::sendDirection(int sockfd,vector<string>& directions){

    string serializedData = directions[0];

    // if (send(sockfd, serializedData.c_str(), serializedData.length(), 0) == -1) {
    //     std::cerr << "Failed to send data." << std::endl;
    //     return 1;
    // }

    //qDebug() << "Sent: " << serializedData.c_str();

    /*char endSignal[1024];
    memset(endSignal,0,1024);
    cout << "recv" << endl;
    ssize_t endControl = recv(sockfd,endSignal,1023,0);
    string strEndControl = endSignal;

    cout << "recv end " <<  strEndControl << endl;*/

    return 0;

}

vector <string> Autonom::giveCommand(vector<pair<int, int> > &path){
    vector<pair<int, int> > secondpath;

    secondpath.push_back(path[0]);
    secondpath.push_back(path[1]);

    return ConvertPathToDirections(secondpath);
}

/*
void printMap(vector<vector<int> >& grid,int row,int col){


    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if(grid[i][j] == 0) printf("\e[47m \e[0m");
            if(grid[i][j] == 1) printf("\e[40m \e[0m");
            if(grid[i][j] == 2) printf("\e[42m \e[0m");
            if(grid[i][j] == 7) printf("\e[41m \e[0m");
            if(grid[i][j] == 4) printf("\e[47mX\e[0m");
        }
        std::cout << std::endl;
    }

}
*/

void Autonom::printDirections(vector<string>directions){
    for (int i = 0; i <directions.size() ; i++)
    {
        cout << directions[i] << endl;
    }
}

void Autonom::addPathToMap(int (&map)[1000][1000],vector<pair<int, int> > &path){
    for (const auto& point : path) {
        map[point.first][point.second] = 4;
    }
    return;
}

/*
vector <vector <int> > addTargetandStartToGrid(vector<vector<int> >& grid,pair<int, int> &start,pair<int, int> &target){
    grid[start.first][start.second] = 2;
    grid[target.first][target.second] = 7;
    return grid;
}

*/
pair<int, int> Autonom::updateTarget(pair<int, int> &target,string& command){

    if(command == "LEFT"){
        target= make_pair(target.first,target.second+1);
    }

    else if(command == "FORWARD"){
        target= make_pair(target.first+1,target.second);
    }

    else if(command == "RIGHT"){
        target= make_pair(target.first,target.second-1);
    }

    else if(command == "BACK"){
        target= make_pair(target.first-1,target.second);
    }

    return target;
}

/*
int connectMap(string ip,int port){

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);


    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
    connect(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    return sockfd;
}
*/

/*
int connectMove(string ip,int port){

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
    connect(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    return sockfd;
}
*/

vector <vector<int> > Autonom::createGridFromMap(int (&map)[1000][1000],int row, int col){

    std::vector<std::vector<int> > vec(row, std::vector<int>(col));

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            vec[i][j]=map[i][j] ;
        }
    }
    return vec;
}


std::vector<std::string> Autonom::ConvertPathToDirections(const std::vector<std::pair<int, int> >& path) {
    std::vector<std::string> directions;

    int ratio = 1;

    for (int i = 1; i < path.size(); ++i) {
        int currRow = path[i].first;
        int currCol = path[i].second;
        int prevRow = path[i - 1].first;
        int prevCol = path[i - 1].second;
        if (currRow == prevRow && currCol == prevCol - 1) {
            for (int i = 0; i < ratio; i++)
            {
                directions.push_back("LEFT");
            }
        }

        else if (currRow == prevRow && currCol == prevCol + 1) {

            for (int i = 0; i < ratio; i++)
            {
                directions.push_back("RIGHT");
            }
        }
        // Determine the direction based on the movement between nodes
        else if (currRow == prevRow - 1 && currCol == prevCol) {
            for (int i = 0; i < ratio; i++)
            {
                directions.push_back("FORWARD");

            }
        }
        else if (currRow == prevRow + 1 && currCol == prevCol) {
            directions.push_back("BACK");
        }
    }
    return directions;
}

bool Autonom::isWithinRange(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}
/*
vector<pair<int, int>> Autonom::findPath(vector<vector<int>>& grid, pair<int, int> start, pair<int, int> target, int size) {
    int rows = grid.size();
    int cols = grid[0].size();

    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<pair<int, int>>> parents(rows, vector<pair<int, int>>(cols));

    queue<pair<int, int>> q;
    q.push(start);
    visited[start.first][start.second] = true;

    bool pathFound = false;

    while (!q.empty()) {
        pair<int, int> current = q.front();
        q.pop();

        if (current == target) {
            pathFound = true;
            break;
        }

        for (const auto& direction : directions_temp) {
            int nextX = current.first + direction.dx;
            int nextY = current.second + direction.dy;

            if (isWithinRange(nextX, nextY, rows, cols) && grid[nextX][nextY] == 0 && !visited[nextX][nextY]) {
                bool isValid = true;

                // Check if any pixel within the robot's dimensions reaches the target
                for (int i = nextX; i < nextX + size && isValid; ++i) {
                    for (int j = nextY; j < nextY + size; ++j) {
                        if (isWithinRange(i, j, rows, cols) && grid[i][j] != 0) {
                            isValid = false;
                            break;
                        }
                    }
                }

                if (isValid) {
                    q.push(make_pair(nextX, nextY));
                    visited[nextX][nextY] = true;
                    parents[nextX][nextY] = current;
                }
            }
        }
    }

    if (!pathFound) {
        return vector<pair<int, int>>();
    }

    vector<pair<int, int>> path;
    pair<int, int> current = target;

    // Reconstruct the path using the middle pixel of the robot
    while (current != start) {
        path.push_back(make_pair(current.first + size / 2, current.second + size / 2));
        current = parents[current.first][current.second];
    }

    path.push_back(make_pair(start.first + size / 2, start.second + size / 2));

    reverse(path.begin(), path.end());

    return path;
}
*/

// Yol bulma fonksiyonu
vector<pair<int, int> > Autonom::findPath(vector<vector<int> >& grid, pair<int, int> start, pair<int, int> target) {
    int rows = grid.size();
    int cols = grid[0].size();

    vector<vector<bool> > visited(rows, vector<bool>(cols, false)); // Ziyaret edilen hücreleri takip etmek için
    vector<vector<pair<int, int> > > parents(rows, vector<pair<int, int> >(cols)); // Her hücrenin ebeveynini takip etmek için

    queue<pair<int, int> > q; // Genişlik öncelikli arama için kuyruk
    q.push(start);
    visited[start.first][start.second] = true;

    bool pathFound = false;

    while (!q.empty()) {
        pair<int, int> current = q.front();
        q.pop();

        if (current == target) {
            pathFound = true;
            break;
        }
        int newDegree = degree;
        while(newDegree < 0){
            newDegree += 360;
        }
        switch(newDegree % 360){
            case 0:
                for(int i = 0; i < 4; i++){
                    directions_temp[i].dx = directions_temp0[i].dx;
                    directions_temp[i].dy = directions_temp0[i].dy;
                }
                break;
            case 270:
                for(int i = 0; i < 4; i++){
                    directions_temp[i].dx = directions_temp1[i].dx;
                    directions_temp[i].dy = directions_temp1[i].dy;
                }
                break;
            case 90:
                for(int i = 0; i < 4; i++){
                    directions_temp[i].dx = directions_temp2[i].dx;
                    directions_temp[i].dy = directions_temp2[i].dy;
                }
                break;
            case 180:
                for(int i = 0; i < 4; i++){
                    directions_temp[i].dx = directions_temp3[i].dx;
                    directions_temp[i].dy = directions_temp3[i].dy;
                }
                break;
        }

        for (const auto& direction : directions_temp) {
            int nextX = current.first + direction.dx;
            int nextY = current.second + direction.dy;

            if (nextX >= 0 && nextX < rows && nextY >= 0 && nextY < cols && grid[nextX][nextY] == 0 && !visited[nextX][nextY]){
                q.push(std::make_pair(nextX, nextY));
                visited[nextX][nextY] = true;
                parents[nextX][nextY] = current;
           }
        }

    }

    if (!pathFound) {
        return vector<pair<int, int> >();; // Hedefe ulaşılamazsa boş yol döndür
    }

    vector<pair<int, int> > path;
    pair<int, int> current = target;

    while (current != start) {
        path.push_back(current);
        current = parents[current.first][current.second];
    }

    path.push_back(start);

    reverse(path.begin(), path.end());

    return path;
}

/*
vector<vector<int> > gridScale(vector<vector<int> > grid){

    int row = grid.size();
    int col = grid[0].size();

    std::vector<std::vector<int> > grid(int(row/10), std::vector<int>(int(col/10)));

    for (int i = 0; i < int(row/10); i++)
    {
        for (int j = 0; j < int(col/10); j++)
        {
            grid[i][j] = 0;
        }
    }

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (grid[i][j] == 1)
            {
                grid[int(i/10)][int(j/10)] = 1;
            }
        }
    }
}
*/

void Autonom::run()
{
    pair<int, int> start = make_pair(positionY, positionX);

    pair<int, int> target = make_pair(int((clickY - 20) * 1.33) , int((clickX -570) * 1.33));

    std::vector<std::vector<int> > grid;
    grid = createGridFromMap(harita,1000,1000);
    path = findPath(grid, start, target);
    if (path.empty()) {
        std::cout << "Hedefe ulasmak mumkun degil." << endl;
    }

    directions = ConvertPathToDirections(path);

    //printDirections(directions);
    addPathToMap(harita,path);

    // check size
   
    //vector <string> directions_2 = giveCommand(path);

}

void Autonom::getDirections (vector<string>& directions){
    directions = this->directions;
}


/*
int main() {

    //int mapSocket = connectMap("192.168.43.150",9025);
    //int moveSocket = connectMap("192.168.43.150",8034);

    pair<int, int> start = make_pair(14, 8);
    pair<int, int> target = make_pair(3, 14);

    while (true)
    {

        std::vector<std::vector<int> > grid;

        int row1[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        int row2[] = {1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0};
        int row3[] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        int row4[] = {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        int row5[] = {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        int row6[] = {0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        int row7[] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        int row8[] = {0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        int row9[] = {0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0};
        int row10[] = {0, 1, 0, 1, 0, 0 , 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
        int row11[] = {0, 1, 0, 1, 0, 0 , 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
        int row12[] = {0, 1, 0, 1, 0, 1 , 1, 1, 1, 0, 0, 0, 0, 0, 0, 0};
        int row13[] = {0, 1, 0, 1, 0, 1 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        int row14[] = {0, 1, 0, 1, 0, 1 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        int row15[] = {0, 0, 0, 0, 0, 1 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        grid.push_back(std::vector<int>(row1, row1 + 16));
        grid.push_back(std::vector<int>(row2, row2 + 16));
        grid.push_back(std::vector<int>(row3, row3 + 16));
        grid.push_back(std::vector<int>(row4, row4 + 16));
        grid.push_back(std::vector<int>(row5, row5 + 16));
        grid.push_back(std::vector<int>(row6, row1 + 16));
        grid.push_back(std::vector<int>(row7, row2 + 16));
        grid.push_back(std::vector<int>(row8, row3 + 16));
        grid.push_back(std::vector<int>(row9, row4 + 16));
        grid.push_back(std::vector<int>(row10, row5 + 16));
        grid.push_back(std::vector<int>(row11, row1 + 16));
        grid.push_back(std::vector<int>(row12, row2 + 16));
        grid.push_back(std::vector<int>(row13, row3 + 16));
        grid.push_back(std::vector<int>(row14, row4 + 16));
        grid.push_back(std::vector<int>(row15, row5 + 16));

    // Accessing and printing elements

        printMap(grid,15,16);

        cout << endl << endl << endl ;
        //createGridFromMap(mapSocket,15,30,10);

        //printMap(grid,5,5);

        vector<pair<int, int> > path = findPath(grid, start, target);


        if (path.empty()) {
            std::cout << "Hedefe ulasmak mumkun degil." << endl;
        }
        else {
        vector <string> directions = ConvertPathToDirections(path);
        addPathToGrid(grid,path);
        addTargetandStartToGrid(grid,start,target);
        printMap(grid,15,16);

        return 0;
        vector <string> directions_2 = giveCommand(path);
        //sendDirection(moveSocket,directions_2);
        //updateTarget(target,directions_2[0]);

        }

    }


    //Find Path




    return 0;
}

*/
