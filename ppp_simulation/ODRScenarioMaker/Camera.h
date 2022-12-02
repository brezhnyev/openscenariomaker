#pragma once

#include "Actor.h"
#include <QtWidgets/QLabel> // need to think about Qt here

class QLabel;

class Camera : public Actor
{
public:
    Camera(Selectable * parent) : Actor("", parent) { m_cameraWidget = new QLabel(); m_bbox = Eigen::Vector3f(1,1,1); }
    Camera(const std::string & name, Selectable * parent) : Actor(name, parent) {}
    void draw() const override;
    void drawWithNames() const override;
    void drawGeometry() const override;
    std::string getType() const override { return "Camera"; }
    QLabel * getCamWidget() { return m_cameraWidget; }

    float getFOV() { return m_FOV; }
    void setFOV(double val) { m_FOV = (float)val; }

private:
    QLabel * m_cameraWidget{nullptr};
    float m_FOV{60.0f};
};
