// Copyright (c) George Washington University, all rights reserved. See the
// accompanying LICENSE file for more information.

#include "KitKatDriver.h"
#include <PbMsgs/Camera.pb.h>
#include "./Camera.h"
#include "./Image.h"

#include <android/log.h>
namespace hal {

KitKatDriver::KitKatDriver() {
  camera_ = camera_alloc(0);
  if (camera_) {
    camera_play(camera_);
  } else {
    abort();
  }
}

KitKatDriver::~KitKatDriver() {
  camera_stop(camera_);
  camera_free(camera_);
}

bool KitKatDriver::Capture( pb::CameraMsg& vImages )
{
  pb::ImageMsg* pb_img = vImages.add_image();

  image_t image;
  camera_lock_buffer(camera_, &image);

  if (!image.buffer) return false;

  pb_img->set_width(image.width);
  pb_img->set_height(image.height);
  pb_img->set_type(pb::PB_UNSIGNED_BYTE);
  pb_img->set_format(pb::PB_LUMINANCE);
  pb_img->set_timestamp(image.timestamp);
  pb_img->set_data(image.buffer, image.width * image.height);

  camera_unlock_buffer(camera_, &image);
  return true;
}

size_t KitKatDriver::NumChannels() const
{
  return 1;
}

size_t KitKatDriver::Width( size_t ) const
{
  return 1280;
}

size_t KitKatDriver::Height( size_t ) const
{
  return 720;
}

}
