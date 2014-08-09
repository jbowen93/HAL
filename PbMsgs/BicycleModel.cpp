#include <BicycleModel.h>

BicycleModel::BicycleModel()
{


}

// CurrPose = {Initial_X, Initial_Y, Initial_teta}
// DeltaDisp = {distance_traveled, heading_angle_change}
bool BicycleModel::process_model(Eigen::Vector3d InitPose, Eigen::Matrix3d InitCov,Eigen::Vector2d DeltaDisp,
                                            Eigen::Vector3d& rNewPose, Eigen::Matrix3d& rNewCov)
{
    Eigen::Vector3d Pose(InitPose[0]+(DeltaDisp[0])*cos(CurrPose[2]*DeltaDisp[1]),
                         InitPose[1]+(DeltaDisp[0])*sin(CurrPose[2]*DeltaDisp[1]),
                         InitPose[2]+DeltaDisp[1]);
    rNewPose = Pose;

    return true;
}


