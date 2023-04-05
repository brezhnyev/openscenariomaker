#include "CameraProps.h"
#include "LDoubleSpinBox.h"

#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>

#include <eigen3/Eigen/Eigen>

#include <sstream>
#include <iostream>

#define MINMAXPOS 1000000

CameraProps::CameraProps(Camera & camera) : m_camera(camera)
{
    QVBoxLayout * mainLayout = new QVBoxLayout();

    x = new LDoubleSpinBox(this, camera.getPos().x(), -MINMAXPOS, MINMAXPOS, 0.1, "X");
    y = new LDoubleSpinBox(this, camera.getPos().y(), -MINMAXPOS, MINMAXPOS, 0.1, "Y");
    z = new LDoubleSpinBox(this, camera.getPos().z(), -MINMAXPOS, MINMAXPOS, 0.1, "Z");

    QVBoxLayout * l1 = new QVBoxLayout();
    l1->addWidget(x);
    l1->addWidget(y);
    l1->addWidget(z);

    QGroupBox * posInfo = new QGroupBox(this);
    posInfo->setTitle("Position");
    posInfo->setLayout(l1);
    mainLayout->addWidget(posInfo);

    connect(x, &LDoubleSpinBox::valueChanged, [this](double val){ m_camera.setPos(Eigen::Vector3f(val, y->value(), z->value())); emit signal_update(); });
    connect(y, &LDoubleSpinBox::valueChanged, [this](double val){ m_camera.setPos(Eigen::Vector3f(x->value(), val, z->value())); emit signal_update(); });
    connect(z, &LDoubleSpinBox::valueChanged, [this](double val){ m_camera.setPos(Eigen::Vector3f(x->value(), y->value(), val)); emit signal_update(); });

    QVBoxLayout * l2 = new QVBoxLayout();
    LDoubleSpinBox * roll = new LDoubleSpinBox(this, camera.getOri().x(), -360, 360, 0.1, "roll");
    LDoubleSpinBox * pitch = new LDoubleSpinBox(this, camera.getOri().y(), -360, 360, 0.1, "pitch");
    LDoubleSpinBox * yaw = new LDoubleSpinBox(this, camera.getOri().z(), -360, 360, 0.1, "yaw");

    l2->addWidget(roll);
    l2->addWidget(pitch);
    l2->addWidget(yaw);

    QGroupBox * oriInfo = new QGroupBox(this);
    oriInfo->setTitle("Orientation");
    oriInfo->setLayout(l2);
    mainLayout->addWidget(oriInfo);

    connect(roll,  &LDoubleSpinBox::valueChanged, [pitch,yaw,this ](double val){ m_camera.setOri(Eigen::Vector3f(val, pitch->value(), yaw->value()));  emit signal_update(); });
    connect(pitch, &LDoubleSpinBox::valueChanged, [roll,yaw, this ](double val){ m_camera.setOri(Eigen::Vector3f(roll->value(), val,  yaw->value()));  emit signal_update(); });
    connect(yaw,   &LDoubleSpinBox::valueChanged, [roll,pitch,this](double val){ m_camera.setOri(Eigen::Vector3f(roll->value(), pitch->value(), val)); emit signal_update(); });

    QVBoxLayout * l3 = new QVBoxLayout();
    LDoubleSpinBox * FOV = new LDoubleSpinBox(this, m_camera.get_FOV(), 0, 180, 1, "FOV");
    l3->addWidget(FOV);
    LDoubleSpinBox * width = new LDoubleSpinBox(this, m_camera.get_width(), 0, 100000, 1, "Width");
    l3->addWidget(width);
    LDoubleSpinBox * height = new LDoubleSpinBox(this, m_camera.get_height(), 0, 100000, 1, "Height");
    l3->addWidget(height);
    QGroupBox * intrinsics = new QGroupBox(this);
    intrinsics->setTitle("Cam props");
    intrinsics->setLayout(l3);
    mainLayout->addWidget(intrinsics);

    connect(FOV, &LDoubleSpinBox::valueChanged, [this](double val){ m_camera.set_FOV(val); emit signal_update(); });
    connect(width, &LDoubleSpinBox::valueChanged, [this](double val){ m_camera.set_width(val); emit signal_update(); });
    connect(height, &LDoubleSpinBox::valueChanged, [this](double val){ m_camera.set_height(val); emit signal_update(); });

    QPushButton * delButton = new QPushButton("Delete", this);
    mainLayout->addStretch(1);
    mainLayout->addWidget(delButton);
    connect(delButton, &QPushButton::clicked, [this]()
    { 
        int id = m_camera.getID();
        if (id == -1)
        {
            QMessageBox::warning(this, "Error deleting Element", "Failed to delete Camera: index not found!");
            return;
        }
        emit signal_delete(id);
        close();
    });

    setLayout(mainLayout);
}

void CameraProps::update(float valx, float valy, float valz)
{
    x->setValue(valx);
    y->setValue(valy);
    z->setValue(valz);
}