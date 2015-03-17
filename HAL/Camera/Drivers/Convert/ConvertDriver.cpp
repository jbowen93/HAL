#include "ConvertDriver.h"
#include "HAL/Devices/DeviceException.h"

#include <iostream>

#include <opencv2/opencv.hpp>

using namespace hal;

ConvertDriver::ConvertDriver(
    std::shared_ptr<CameraDriverInterface> input,
    int channel,
    const std::string& format,
    ImageDim size,
    float scale,
    float bias
  ) : input_(input),
      num_channels_(input->NumChannels()),
      channel_idx_(channel),
      format_(format),
      scale_(scale),
      bias_(bias)
{
  // Set the correct image size on the output interface, considering the request
  // to resize the images.
  for(size_t ii = 0; ii < input_->NumChannels(); ++ii) {
    img_width_.push_back(input_->Width(ii));
    img_height_.push_back(input_->Height(ii));
    orig_img_width_.push_back(input_->Width(ii));
    orig_img_height_.push_back(input_->Height(ii));

    if (channel_idx_ != -1) {
      if (channel_idx_ != ii) {
        continue;
      }
    }

    // Check if image resize was requested.
    resize_requested_ = (size.x != 0 || size.y != 0);
    if (resize_requested_) {
      img_width_[ii] = size.x;
      img_height_[ii] = size.y;
    }
  }

  // Guess output color coding.
  out_cv_format_ = -2;
  if (format_ == "DEFAULT") {
    out_cv_format_ = -1;
  } else if (format_ == "MONO8") {
    out_cv_format_ = CV_8UC1;
    out_pb_format_ = pb::Format::PB_LUMINANCE;
  } else if (format_ == "RGB8") {
    out_cv_format_ = CV_8UC3;
    out_pb_format_ = pb::Format::PB_RGB;
  } else if (format_ == "BGR8") {
    out_cv_format_ = CV_8UC3;
    out_pb_format_ = pb::Format::PB_BGR;
  }

  if (out_cv_format_ == -2) {
    throw DeviceException("HAL: Error! Unknown target format: " + format_);
  }
}

bool ConvertDriver::Capture(pb::CameraMsg& images)
{
  message_.Clear();
  input_->Capture(message_);

  // Guess source color coding.
  if (cv_type_.empty()) {
    for(int ii = 0; ii < message_.image_size(); ++ii) {

      pb::Format pbtype = message_.image(ii).format();

      int channels = 0;
      if (pbtype == pb::PB_LUMINANCE) {
        channels = 1;
      } else if (pbtype == pb::PB_RGB || pbtype == pb::PB_BGR) {
        channels = 3;
      }

      int cvtype = -1;
      if (channels != 0) {
        if (message_.image(ii).type() == pb::PB_BYTE ||
            message_.image(ii).type() == pb::PB_UNSIGNED_BYTE)
          cvtype = (channels == 1 ? CV_8UC1 : CV_8UC3);
        else if (message_.image(ii).type() == pb::PB_UNSIGNED_SHORT ||
                 message_.image(ii).type() == pb::PB_SHORT)
          cvtype = (channels == 1 ? CV_16UC1 : CV_16UC3);
        else if (message_.image(ii).type() == pb::PB_FLOAT)
          cvtype = (channels == 1 ? CV_32FC1 : CV_32FC3);
      }

      cv_type_.push_back(cvtype);
      pb_type_.push_back(pbtype);

      if (cvtype == -1) {
        std::cerr << "HAL: Error! Could not guess source color coding of "
                     "channel " << ii << ". Is it RAW?" << std::endl;
      }
    }
  }

  // Prepare return images.
  images.set_device_time(message_.device_time());
  images.set_system_time(message_.system_time());

  for(size_t ii = 0; ii < num_channels_; ++ii) {
    pb::ImageMsg* pbImg = images.add_image();

    // If the user has specified to convert a single channel only,
    // gate it here
    if (channel_idx_ != -1) {
      if (channel_idx_ != ii) {
//        pbImg->Swap(message_.mutable_image(ii));
        *pbImg = message_.image(ii);
        continue;
      }
    }

    if (cv_type_[ii] == -1) { // This image cannot be converted.
//      pbImg->Swap(message_.mutable_image(ii));
      *pbImg = message_.image(ii);
      continue;
    }

    // OpenCV source image wrapper.
    cv::Mat in_img(orig_img_height_[ii], orig_img_width_[ii], cv_type_[ii],
                   (void*)message_.mutable_image(ii)->data().data());


    ///-------------------- RESIZE
    if (resize_requested_) {
      cv::Mat resized_img;
      cv::resize(in_img, resized_img, cv::Size(img_width_[ii], img_height_[ii]));
      in_img = resized_img;
    }

    pbImg->set_timestamp(message_.image(ii).timestamp());
    pbImg->set_serial_number(message_.image(ii).serial_number());
    pbImg->set_width(img_width_[ii]);
    pbImg->set_height(img_height_[ii]);

    // If output format is default, leave image untouched.
    if (out_cv_format_ == -1) {
      pbImg->set_type(message_.image(ii).type());
      pbImg->set_format(message_.image(ii).format());
      pbImg->mutable_data()->resize(img_width_[ii] * img_height_[ii]
                                    * CV_MAT_CN(cv_type_[ii])
                                    * CV_MAT_DEPTH(cv_type_[ii]));
      std::cout << "Chans: " << CV_MAT_CN(cv_type_[ii]) << std::endl;
      std::cout << "Depth: " << CV_MAT_DEPTH(cv_type_[ii]) << std::endl;
    } else {
      // Otherwise the only conversion is to unsigned byte.
      pbImg->set_type(pb::PB_UNSIGNED_BYTE);
      pbImg->set_format(out_pb_format_);
      pbImg->mutable_data()->resize(img_width_[ii] * img_height_[ii] *
                                   (out_cv_format_ == CV_8UC1 ? 1 : 3) );
    }

    ///-------------------- CONVERT + BIAS + SCALE
    // If format was left default, do not convert only apply bias + scale
    // if applicable.
    cv::Mat out_img(img_height_[ii], img_width_[ii], out_cv_format_,
                   (void*)pbImg->mutable_data()->data());
    if (out_cv_format_ == -1) {
      if (scale_ != 1.0 && bias_ != 0.0) {
        out_img = in_img / scale_;
        out_img = in_img + bias_;
      }
    } else {
#if 0
      // note: cv::cvtColor cannot convert between depth types and
      // cv::Mat::convertTo cannot change the number of channels
      switch (cv_type_[ii]) {
        case CV_8UC1:
          if (out_cv_format_ == CV_8UC1)
            std::copy(s_img.begin<unsigned char>(), s_img.end<unsigned char>(),
                      out_img.begin<unsigned char>());
          else
            cv::cvtColor(s_img, out_img,
              (out_pb_format_ == pb::Format::PB_RGB ? CV_GRAY2RGB : CV_GRAY2BGR));
          break;

        case CV_8UC3:
          if( out_cv_format_ == CV_8UC1 )
            cv::cvtColor(s_img, out_img,
              (pb_type_[ii] == pb::Format::PB_RGB ? CV_RGB2GRAY : CV_BGR2GRAY));
          else {
            if( pb_type_[ii] == out_pb_format_ )
              std::copy(s_img.begin<unsigned char>(), s_img.end<unsigned char>(),
                        out_img.begin<unsigned char>());
            else
              cv::cvtColor(s_img, out_img,
                (pb_type_[ii] == pb::Format::PB_RGB ? CV_RGB2BGR : CV_BGR2RGB));
          }
          break;

        case CV_16UC1:
          s_img.convertTo(aux, CV_64FC1);
          if( out_cv_format_ == CV_8UC1 )
            aux.convertTo(out_img, CV_8UC1, 255. / m_dRange);
          else {
            aux.convertTo(aux, CV_8UC1, 255. / m_dRange);
            cv::cvtColor(aux, out_img,
              (out_pb_format_ == pb::Format::PB_RGB ? CV_GRAY2RGB : CV_GRAY2BGR));
          }
          break;

        case CV_16UC3:
          s_img.convertTo(aux, CV_64FC3);
          if( out_cv_format_ == CV_8UC1 ) {
            aux.convertTo(aux, CV_8UC3, 255. / m_dRange);
            cv::cvtColor(aux, out_img,
              (pb_type_[ii] == pb::Format::PB_RGB ? CV_RGB2GRAY : CV_BGR2GRAY));
          } else {
            if( pb_type_[ii] == out_pb_format_ )
              aux.convertTo(out_img, CV_8UC3, 255. / m_dRange);
            else {
              aux.convertTo(aux, CV_8UC3, 255. / m_dRange);
              cv::cvtColor(aux, out_img,
                (pb_type_[ii] == pb::Format::PB_RGB ? CV_RGB2BGR : CV_BGR2RGB));
            }
          }
          break;

        case CV_32FC1:
          if( out_cv_format_ == CV_8UC1 ) {
            s_img.convertTo(out_img, CV_8UC1, 255. / m_dRange);
          } else {
            s_img.convertTo(aux, CV_8UC1, 255. / m_dRange);
            cv::cvtColor(aux, out_img,
              (out_pb_format_ == pb::Format::PB_RGB ? CV_GRAY2RGB : CV_GRAY2BGR));
          }
          break;

        case CV_32FC3:
          if( out_cv_format_ == CV_8UC1 ) {
            s_img.convertTo(aux, CV_8UC3, 255. / m_dRange);
            cv::cvtColor(aux, out_img,
              (pb_type_[ii] == pb::Format::PB_RGB ? CV_RGB2GRAY : CV_BGR2GRAY));
          } else {
            if( pb_type_[ii] == out_pb_format_ )
              s_img.convertTo(out_img, CV_8UC3, 255. / m_dRange);
            else {
              s_img.convertTo(aux, CV_8UC3, 255. / m_dRange);
              cv::cvtColor(aux, out_img,
                (pb_type_[ii] == pb::Format::PB_RGB ? CV_RGB2BGR : CV_BGR2RGB));
            }
          }
          break;
      }
#endif
    }
  }

  return true;
}

std::string ConvertDriver::GetDeviceProperty(const std::string& property)
{
  return input_->GetDeviceProperty(property);
}

size_t ConvertDriver::NumChannels() const
{
  return num_channels_;
}

size_t ConvertDriver::Width(size_t idx) const
{
  return img_width_[idx];
}

size_t ConvertDriver::Height(size_t idx) const
{
  return img_height_[idx];
}
