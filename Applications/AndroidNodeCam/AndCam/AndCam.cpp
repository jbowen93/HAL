#include <stdio.h>
#include "AndCam.h"
#include <Node/Node.h>
#include <PbMsgs/Logger.h>
#include <PbMsgs/Image.h>
#include <PbMsgs/NodeCamMessage.pb.h>
#include <HAL/Camera/CameraDevice.h>
#include <HAL/IMU/IMUDevice.h>
#include <HAL/Utils/TicToc.h>
#include <android/Log.h>

using namespace std;

void RegsiterCamDevice(RegisterNodeCamReqMsg& req, RegisterNodeCamRepMsg& rep, void* UserData)
{
  rep.set_regsiter_flag(1);
  rep.set_time_step(gCounter);
  rep.set_width(width);
  rep.set_height(height);
  rep.set_channels(channels);
}

bool InitializeLogger( void )
{
  logger_.LogToFile("/sdcard/", "sensors");
}

bool InitializeNode( string IP, int port )
{
  gCounter = 0;
  width = 320;
  height = 240;
  channels = 1;

  n.set_verbosity(-2);
  n.init(string("LocalSim"));
  n.advertise(string("NodeCam"));
  n.provide_rpc(string("RegsiterCamDevice"), &RegsiterCamDevice, NULL);

  msg::GetTableResponse rep;
  n.ConnectNode( IP, port, &rep);


}

void sendData( char* data, int w, int h, int format = pb::PB_LUMINANCE, int type = pb::PB_UNSIGNED_BYTE )
{
  if (data != NULL) {
    pb::CameraMsg camMsg;
    pb::ImageMsg* pbImage = camMsg.add_image();
    pbImage->set_height(height);
    pbImage->set_width(width);
    pbImage->set_format(pb::PB_LUMINANCE);
    pbImage->set_type(pb::PB_UNSIGNED_BYTE);
    pbImage->set_timestamp(hal::Tic());
    pbImage->set_data(data, width*height);
    n.publish(topic, camMsg);
  }
}

void logCamData(char *data, int w, int h, int format, int type)
{
  if (data != NULL) {
    pb::CameraMsg camMsg;
    pb::ImageMsg* pbImage = camMsg.add_image();
    pbImage->set_height(height);
    pbImage->set_width(width);
    pbImage->set_format(pb::PB_LUMINANCE);
    pbImage->set_type(pb::PB_UNSIGNED_BYTE);
    pbImage->set_data(data, width*height);
    pbImage->set_timestamp(hal::Tic());

    pb::Msg pbMsg;
    pbMsg.mutable_camera()->Swap(&camMsg);
    logger_.LogMessage(pbMsg);
  }
}



void logIMUData(float ax, float ay, float az, float gr, float gp, float gy, float mx, float my, float mz)
{
  pb::ImuMsg imuMsg;
  pb::VectorMsg vMesg;
  vMesg.add_data(ax);
  vMesg.add_data(ay);
  vMesg.add_data(az);
  imuMsg.mutable_accel()->Swap(&vMesg);

  vMesg.clear_data();
  vMesg.add_data(gr);
  vMesg.add_data(gp);
  vMesg.add_data(gy);
  imuMsg.mutable_gyro()->Swap(&vMesg);

  vMesg.clear_data();
  vMesg.add_data(mx);
  vMesg.add_data(my);
  vMesg.add_data(mz);
  imuMsg.mutable_mag()->Swap(&vMesg);

  pb::Msg pbMsg;
  pbMsg.set_timestamp(hal::Tic());
  pbMsg.mutable_imu()->Swap(&imuMsg);
  logger_.LogMessage(pbMsg);
}
