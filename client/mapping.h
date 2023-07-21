#ifndef MAPPING_H
#define MAPPING_H
#include <QThread>
#include "autonom.h"
#include "mainwindow.h"

class Mapping : public QThread
{
    Q_OBJECT
public:
    Autonom * autonom = new Autonom;
    Mapping(QObject *parent = nullptr);
protected:
    void run() override;
    std::vector<std::pair<int, int> > stringToVector(const std::string& data);
    void beautifyMap(int map[1000][1000], std::vector<std::pair<int, int>> oldPoints, std::vector<std::pair<int, int>> newPoints, int distance);
    void connect(int map[1000][1000], int row, int col, int newRow, int newCol);
};


#endif // MAPPING_H
