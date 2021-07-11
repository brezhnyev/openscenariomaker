#pragma once

#include <QGLViewer/qglviewer.h>
#include <eigen3/Eigen/Eigen>


class Viewer : public QGLViewer
{
    Q_OBJECT

public:

    void addDataStatic(std::vector<Eigen::Vector2f> && v);
    void updateDataRoads(std::vector<std::vector<Eigen::Vector2f>> && v);

signals:
    void ClosingWindow();

protected :
    virtual void draw();
    virtual void init();
    
private:
    std::vector<std::vector<Eigen::Vector2f>> dataStatic;
    std::vector<std::vector<Eigen::Vector2f>> dataRoads;
};