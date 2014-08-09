#pragma once

#include <eigen3/Eigen/Core>
#include <Utils/MathTypes.h>
#include <sophus/sophus.hpp>

class BicycleModel
{
public:
    BicycleModel();
    bool process_model(Eigen::Vector3d InitPose, Eigen::Matrix3d InitCov, Eigen::Vector2d DeltaDisp,
                                  Eigen::Vector3d &rNewPose, Eigen::Matrix3d &rNewCov);

private:
    Eigen::Vector2i read_sensor_data(/*PbMsg parameters*/);
    Eigen::Vector2i velocityTodistance(Eigen::Vector2i OdomVel);
    //Eigen::Vector2i OdomPose;           // {iLeftOdom,iRightOdom}


}
