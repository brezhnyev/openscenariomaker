#pragma once

#include "Selectable.h"

#include <eigen3/Eigen/Eigen>

class Waypath;

class Actor : public Selectable
{
public:
    Actor(const std::string & name) : m_name(name) {}
    virtual ~Actor() = 0;
    void setTrf(Eigen::Vector3f pos, Eigen::Vector3f ori)
    {
        m_pos = pos;
        m_ori = ori;
    }
    void setPos(Eigen::Vector3f pos) { m_pos = pos; }
    void setOri(Eigen::Vector3f ori) { m_ori = ori; }
    void setBbox(Eigen::Vector3f bbox) { m_bbox = bbox; }
    void setTrf(float x, float y, float z, float roll, float pitch, float yaw)
    {
        m_pos = Eigen::Vector3f(x,y,z);
        m_ori = Eigen::Vector3f(roll, pitch, yaw);
    }
    Eigen::Vector3f getPos() { return m_pos; }
    Eigen::Vector3f getOri() { return m_ori; }
    Eigen::Vector3f getBbox() { return m_bbox; }
    std::string getName() { return m_name; }
    void setName(std::string name) { m_name = name; }
    void updatePose();

protected:
    std::string m_name;
    Eigen::Vector3f m_pos{0.0f, 0.0f, 0.0f};
    Eigen::Vector3f m_ori{0.0f, 0.0f, 0.0f};
    Eigen::Vector3f m_bbox{0.0f, 0.0f, 0.0f};
};