#include "WaypathProps.h"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMessageBox>

WaypathProps::WaypathProps(Waypath & p) : m_waypath(p)
{
    QVBoxLayout * lh = new QVBoxLayout();

    lh->addWidget(new QLabel("Waypath " + QString::number(p.getID()), this));
    QPushButton * updateWaypath = new QPushButton("Update Waypath", this);
    lh->addWidget(updateWaypath);
    QPushButton * delThis = new QPushButton("Delete", this);
    lh->addStretch(1);
    lh->addWidget(delThis);

    setLayout(lh);
    connect(delThis, &QPushButton::clicked, [this]()
    {
        int id = m_waypath.getID();
        if (id == -1)
        {
            QMessageBox::warning(this, "Error deleting Element", "Failed to delete Waypoint: index not found!");
        }
        emit signal_delete(id);
        close();
    });
    connect(updateWaypath, &QPushButton::clicked, [this]()
    {
        m_waypath.updateSmoothPath();
        emit signal_update();
    });
    lh->addStretch(1);
}