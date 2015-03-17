#pragma once

#include <memory>

#include <HAL/Camera/CameraDriverInterface.h>
#include <HAL/Utils/Uri.h>

namespace hal
{

class ConvertDriver : public CameraDriverInterface
{
public:
    ConvertDriver(std::shared_ptr<CameraDriverInterface> input,
                  int channel,
                  const std::string& format,
                  ImageDim size,
                  float scale,
                  float bias);

    bool Capture(pb::CameraMsg& images);
    std::shared_ptr<CameraDriverInterface> GetInputDevice() { return input_; }

    std::string GetDeviceProperty(const std::string& property);

    size_t NumChannels() const;
    size_t Width(size_t idx = 0) const;
    size_t Height(size_t idx = 0) const;

protected:
    std::shared_ptr<CameraDriverInterface>  input_;
    pb::CameraMsg                           message_;
    unsigned int                            num_channels_;
    int                                     channel_idx_;
    std::string                             format_;
    std::vector<int>                        cv_type_;
    std::vector<pb::Format>                 pb_type_;
    int                                     out_cv_format_;
    pb::Format                              out_pb_format_;
    bool                                    resize_requested_;
    std::vector<unsigned int>               img_width_;
    std::vector<unsigned int>               orig_img_width_;
    std::vector<unsigned int>               img_height_;
    std::vector<unsigned int>               orig_img_height_;
    float                                   scale_;
    float                                   bias_;
};

}
