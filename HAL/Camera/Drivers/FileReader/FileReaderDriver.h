#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <HAL/Camera/CameraDriverInterface.h>

namespace hal {

class FileReaderDriver : public CameraDriverInterface {
 public:
  FileReaderDriver(const std::vector<std::string>& ChannelRegex,
                   size_t StartFrame = 0,
                   bool Loop = false,
                   size_t BufferSize = 35,
                   int cvFlags = 0 /*cv::IMREAD_UNCHANGED*/,
                   double frequency = 30.0,
                   const std::string& sName = std::string(),
                   const std::string& idString = std::string());
  ~FileReaderDriver();

  bool Capture( pb::CameraMsg& vImages );
  std::shared_ptr<CameraDriverInterface> GetInputDevice() {
    return std::shared_ptr<CameraDriverInterface>();
  }

  std::string GetDeviceProperty(const std::string& sProperty);

  size_t NumChannels() const;
  size_t Width( size_t idx = 0 ) const;
  size_t Height( size_t idx = 0 ) const;

 private:
  static void _ThreadCaptureFunc( FileReaderDriver* pFR );
  bool _Read();
  double _GetNextTime();
  double _GetTimestamp(const std::string& sFileName) const;

 private:
  volatile bool should_run_;
  std::shared_ptr<std::thread> capture_thread_;

  // vector of lists of files
  std::mutex mutex_;
  std::condition_variable buffer_empty_, buffer_full_;

  std::queue<pb::CameraMsg> image_buffer_;
  std::vector<std::vector<std::string> > file_list_;
  std::string base_dir_;
  unsigned int num_channels_;

  // Current image index per channel
  std::vector<unsigned int> current_image_index_;
  bool loop_;
  int buffer_size_;
  int cv_image_read_flags_;
  std::string time_keeper_, name_, id_;
  int frames_processed_;
  double frequency_;
};
}  // end namespace hal
