#include <jni.h>
#include <AndCam.h>
#include "AndNodeCam.h"
#include <android/Log.h>

     
JNIEXPORT void JNICALL Java_edu_gwu_rpg_androidnodecam_AndNodeCam_sendData
  (JNIEnv *env, jobject jobj, jbyteArray bytes, jint width, jint height)
{
    int len = env->GetArrayLength (bytes);
    char* buf;
    buf = new char[len];
    env->GetByteArrayRegion (bytes, 0, len, reinterpret_cast<jbyte*>(buf));
    sendData(buf, (int) width, (int) height, 5121, 6409);
    delete buf;
}

JNIEXPORT void JNICALL Java_edu_gwu_rpg_androidnodecam_AndNodeCam_Initialize
  (JNIEnv *env, jobject jobj, jstring ip_address, jint port)
{
    InitializeNode(env->GetStringUTFChars(ip_address, NULL), (int) port);
}



JNIEXPORT void JNICALL Java_edu_gwu_rpg_androidnodecam_AndNodeCam_InitializeLogger
(JNIEnv *env, jobject obj)
{
    InitializeLogger();
}
    
JNIEXPORT void JNICALL Java_edu_gwu_rpg_androidnodecam_AndNodeCam_logCamData
    (JNIEnv *env, jobject obj, jbyteArray bytes, jint width, jint height)
{
    int len = env->GetArrayLength (bytes);
    char* buf;
    buf = new char[len];
    env->GetByteArrayRegion (bytes, 0, len, reinterpret_cast<jbyte*>(buf));
    logCamData(buf, (int) width, (int) height, 5121, 6409);
    delete buf;
}
    
JNIEXPORT void JNICALL Java_edu_gwu_rpg_androidnodecam_AndNodeCam_logIMUData
    (JNIEnv *env, jobject obj, jfloat ax, jfloat ay, jfloat az, jfloat gr, jfloat gp, jfloat gy, jfloat mx, jfloat my, jfloat mz)
{
    logIMUData(ax, ay, az, gr, gp, gy, mx, my, mz);
}

JNIEXPORT void JNICALL Java_edu_gwu_rpg_androidnodecam_AndNodeCam_stopLogging
  (JNIEnv *env, jobject obj)
{
    logger_.StopLogging();
}
