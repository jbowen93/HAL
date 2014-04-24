#pragma once

#include <stdio.h>
#include <string>
#include <Node/Node.h>
#include <PbMsgs/Logger.h>
#include <PbMsgs/Image.h>
#include <PbMsgs/Imu.pb.h>
#include <PbMsgs/NodeCamMessage.pb.h>
#include <HAL/Camera/CameraDevice.h>


std::string sNodeName("LocalSim");
int gCounter = 0;
int width, height, channels;
hal::node n;
std::string topic("NodeCam");
pb::Logger& logger_(pb::Logger::GetInstance());

void RegsiterCamDevice(RegisterNodeCamReqMsg& req, RegisterNodeCamRepMsg& rep, void* UserData);
void sendData(char *data, int w, int h, int format, int type);
void logCamData(char *data, int w, int h, int format, int type);
void logIMUData(float ax, float ay, float az, float gr, float gp, float gy, float mx, float my, float mz);
bool InitializeNode( std::string IP, int port );
bool InitializeLogger( void );
