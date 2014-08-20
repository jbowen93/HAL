
#include "FileReaderDriver.h"

#include <HAL/Devices/DeviceTime.h>
#include <HAL/Devices/DeviceException.h>
#include <HAL/Utils/StringUtils.h>

#include <opencv2/opencv.hpp>

#include "ReadImage.h"

using namespace std;

namespace hal {

FileReaderDriver::FileReaderDriver(const std::vector<std::string>& regexes,
                                   size_t start_frame, bool loop,
                                   size_t buffer_size, int cv_flags,
                                   double frequency,
                                   const std::string& name,
                                   const std::string& id_string)
    : should_run_(false),
      num_channels_(regexes.size()),
      current_image_index_(num_channels_, start_frame),
      loop_(loop),
      buffer_size_(buffer_size),
      cv_image_read_flags_(cv_flags),
      name_(name),
      id_(id_string),
      frames_processed_(0),
      frequency_(frequency) {
  // clear variables if previously initialized
  file_list_.clear();

  if(num_channels_ < 1) {
    throw DeviceException("No channels specified.");
  }

  base_dir_ = DirUp(regexes[0]);
  file_list_.resize(num_channels_);

  for(unsigned int ii = 0; ii < num_channels_; ii++) {
    // Now generate the list of files for each channel
    std::vector< std::string >& vFiles = file_list_[ii];

    if(!hal::WildcardFileList(regexes[ii], vFiles)) {
      throw DeviceException("No files found from regexesp", regexes[ii]);
    }
  }

  // fill buffer
  for (int ii=0; ii < buffer_size_; ii++) {
    _Read();
  }

  // push timestamp of first image into the Virtual Device Queue
  DeviceTime::PushTime(_GetNextTime());

  // run thread to keep buffer full
  should_run_ = true;
  capture_thread_.reset(new std::thread(&_ThreadCaptureFunc, this));
}

FileReaderDriver::~FileReaderDriver() {
  should_run_ = false;
  if(capture_thread_) {
    while(!image_buffer_.empty()) {
      image_buffer_.pop();
    }
    buffer_full_.notify_one();
    capture_thread_->join();
  }
}

// Consumer
bool FileReaderDriver::Capture(pb::CameraMsg& vImages) {
  for (size_t i = 0; i < num_channels_; ++i) {
    if (current_image_index_[i] >= file_list_[i].size() &&
        image_buffer_.empty() && !loop_) {
      return false;
    }
  }

  std::unique_lock<std::mutex> lock(mutex_);

  // Wait until the buffer has data to read
  while (image_buffer_.empty()) {
    buffer_empty_.wait(lock);
  }

  DeviceTime::WaitForTime(_GetNextTime());

  //***************************************************
  // consume from buffer
  //***************************************************


  // now fetch the next set of images from buffer
  vImages = image_buffer_.front();

  // remove image from buffer queue
  image_buffer_.pop();

  // send notification that the buffer has space
  buffer_full_.notify_one();

  // push next timestamp to queue now that we popped from the buffer
  DeviceTime::PopAndPushTime(_GetNextTime());

  return true;
}

std::string FileReaderDriver::GetDeviceProperty(const std::string& sProperty) {
  if(sProperty == hal::DeviceDirectory) {
    return base_dir_;
  }
  else if(sProperty == "name") {
    return name_;
  }
  else if(sProperty == "id") {
    return id_;
  }

  return std::string();
}

size_t FileReaderDriver::NumChannels() const {
  const pb::CameraMsg& NextFrame = image_buffer_.front();
  return NextFrame.image_size();
}

size_t FileReaderDriver::Width(size_t idx) const {
  const pb::CameraMsg& NextFrame = image_buffer_.front();
  if((int)idx < NextFrame.image_size()) {
    const pb::ImageMsg& NextImg = NextFrame.image(idx);
    return NextImg.width();
  }
  return 0;
}

size_t FileReaderDriver::Height(size_t idx) const {
  const pb::CameraMsg& NextFrame = image_buffer_.front();
  if((int)idx < NextFrame.image_size()) {
    const pb::ImageMsg& NextImg = NextFrame.image(idx);
    return NextImg.height();
  }
  return 0;
}

// Producer
void FileReaderDriver::_ThreadCaptureFunc(FileReaderDriver* pFR) {
  while(pFR->should_run_) {
    if(!pFR->_Read()) {
      break;
    }
  }
}

bool FileReaderDriver::_Read() {
  std::unique_lock<std::mutex> lock(mutex_);

  // Wait until there is space in the buffer
  while(! (image_buffer_.size() < static_cast<size_t>(buffer_size_))){
    buffer_full_.wait(lock);
  }

  //*************************************************************************
  // produce to buffer
  //*************************************************************************

  // loop over if we finished our files!
  for (size_t i = 0; i < num_channels_; ++i) {
    if (current_image_index_[i] >= file_list_[i].size()) {
      if (loop_) {
        current_image_index_.assign(num_channels_, 0);
        break;
      } else {
        return false;
      }
    }
  }

  // now fetch the next set of images
  std::string filename;

  static const double kNoTime = std::numeric_limits<double>::max();
  double current_timestamp = kNoTime;

  bool synced_channels = false;
  while (!synced_channels) {
    synced_channels = true;
    for (unsigned int ii = 0; ii < num_channels_; ++ii) {
      filename = file_list_[ii][current_image_index_[ii]];

      double timestamp = _GetTimestamp(filename);
      if (current_timestamp == kNoTime) {
        current_timestamp = timestamp;
      }

      while (timestamp < current_timestamp) {
        filename = file_list_[ii][++current_image_index_[ii]];
        timestamp = _GetTimestamp(filename);
      }

      if (timestamp > current_timestamp) {
        current_timestamp = timestamp;
        synced_channels = false;
        break;
      }
    }
  }

  pb::CameraMsg vImages;
  double device_timestamp = -1;
  for (unsigned int ii = 0; ii < num_channels_; ++ii) {
    pb::ImageMsg* pbImg = vImages.add_image();
    filename = file_list_[ii][current_image_index_[ii]];
    cv::Mat cvImg = _ReadFile(filename, cv_image_read_flags_);

    double timestamp = _GetTimestamp(filename);
    if (timestamp < 0) timestamp = frames_processed_ / frequency_;
    if (device_timestamp < 0) device_timestamp = timestamp;
    pbImg->set_timestamp(timestamp);

    //        pb::ReadCvMat(cvImg, pbImg);
    pbImg->set_height(cvImg.rows);
    pbImg->set_width(cvImg.cols);

    // TODO this is BAD since 4 bytes can be int, or float, etc, etc
    if(cvImg.elemSize1() == 1) {
      pbImg->set_type(pb::PB_UNSIGNED_BYTE);
    }
    if(cvImg.elemSize1() == 2) {
      pbImg->set_type(pb::PB_UNSIGNED_SHORT);
    }
    if(cvImg.elemSize1() == 4) {
      pbImg->set_type(pb::PB_FLOAT);
    }

    if(cvImg.channels() == 1) {
      pbImg->set_format(pb::PB_LUMINANCE);
    }
    if(cvImg.channels() == 3) {
      pbImg->set_format(pb::PB_RGB);
    }
    pbImg->set_data(
        (const char*)cvImg.data,
        cvImg.rows * cvImg.cols * cvImg.elemSize1() * cvImg.channels());
  }
  vImages.set_device_time(device_timestamp);

  ++frames_processed_;

  // add images at the back of the queue
  image_buffer_.push(vImages);

  //*************************************************************************

  // send notification that the buffer is not empty
  buffer_empty_.notify_one();

  for (unsigned int ii = 0; ii < num_channels_; ++ii) {
    ++current_image_index_[ii];
  }
  return true;
}

double FileReaderDriver::_GetNextTime() {
  if(image_buffer_.empty()) {
    return -1;
  }
  return (image_buffer_.front().has_device_time() ?
          image_buffer_.front().device_time() : 0);
}

double FileReaderDriver::_GetTimestamp(const std::string& filename) const {
  // Returns the timestamp encoded in a filename, or -1.
  //
  // A timestamp is any valid number (starting with a digit) that appears in
  // any position of the string. If there are several numbers, the largest one
  // is returned.
  // Examples:
  // Camera_Left_12345.6789.jpg     returns 12345.6789
  // 12345.6789.jpg                 returns 12345.6789
  // m0001234.pgm                   returns 1234
  // Camera_1_12345.6789.jpg        returns 12345.6789
  // file.png                       returns -1

  // skip the path
  std::string::size_type pos = filename.find_last_of("/\\");
  if (pos == std::string::npos) pos = 0;

  double t = -1;
  const char* begin = filename.c_str() + pos;
  const char* end = filename.c_str() + filename.size();

  for(const char* cur = begin; cur != end;) {
    if (*cur < '0' || *cur > '9')
      ++cur;
    else {
      char* next_pos;
      double value = strtod(cur, &next_pos);
      if (next_pos == cur) break; // could not parse
      cur = next_pos;

      // in the insidious case of several numbers, choose the largest one
      if (value != HUGE_VAL && value > t) t = value;
    }
  }
  return t;
}

}
