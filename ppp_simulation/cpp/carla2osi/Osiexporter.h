#pragma once

#include <eigen3/Eigen/Eigen>
#include <osi3/osi_sensorview.pb.h>
#include "odrparser/odr_1_5.h"

#ifdef USE_CARLA
#include <carla/client/Client.h>
#include <carla/client/ActorList.h>
#include <carla/client/TrafficLight.h>
#include <carla/Memory.h>
#endif

#include <vector>
#include <string>
#include <fstream>

class Osiexporter
{
public:
    Osiexporter(std::string output = "output.osi");
    ~Osiexporter();

    void addStaticObject(const std::vector<Eigen::Vector3f> & v3d, const std::vector<Eigen::Vector2f> & base_polygon, uint64_t & id, std::string type, float scale = 1.0f);
    void addRoads(const odr_1_5::OpenDRIVE &, uint64_t & id, std::vector<std::vector<Eigen::Vector3f>> & vizCenterlines, std::vector<std::vector<Eigen::Vector3f>> & vizBoundaries);
    void extendStaticNames(const std::map<std::string, int> & customNames);

#ifdef USE_CARLA
    void updateMovingObjects(carla::SharedPtr<carla::client::ActorList> actors, std::vector<Eigen::Matrix4f> & vizActors);
#endif
    void setFrameTime(uint32_t seconds, uint32_t nanos);
    std::string toValidType(std::string);
    bool writeFrame(bool);

    osi3::GroundTruth * gt_;
    osi3::SensorView sv_;

    std::ofstream ofs_;
    std::string output_;
};