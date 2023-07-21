#ifndef AUTONOM_H
#define AUTONOM_H

#include <QThread>
#include <vector>
#include "mainwindow.h"
#include <iostream>
#include <vector>
#include <queue>
#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <unistd.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utility>

using namespace std;

struct Direction
{
    int dx;
    int dy;
};

class Autonom
{
public:
    Autonom();
    void run();
    int sendDirection(int sockfd,vector<string>& directions);
    vector <string> giveCommand(vector<pair<int, int> > &path);
    void printDirections(vector<string>directions);
    pair<int, int> updateTarget(pair<int, int> &target,string& command);
    std::vector<std::string> ConvertPathToDirections(const std::vector<std::pair<int, int> >& path);
    vector <vector<int> > createGridFromMap(int (&map)[1000][1000],int row, int col);
    //vector<vector<int> > gridScale(vector<vector<int> > grid);
    void addPathToMap(int (&map)[1000][1000],vector<pair<int, int> > &path);
    void getDirections (vector<string>& directions);
    bool isWithinRange(int x, int y, int rows, int cols);
    vector<pair<int, int>> findPath(vector<vector<int>>& grid, pair<int, int> start, pair<int, int> target);

protected:
    vector <string> directions;
};

#endif // AUTONOM_H
