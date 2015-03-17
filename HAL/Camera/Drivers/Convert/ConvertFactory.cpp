#include <HAL/Devices/DeviceFactory.h>
#include "ConvertDriver.h"

#include <cstdlib>
#include <string>

namespace hal
{

class ConvertFactory : public DeviceFactory<CameraDriverInterface>
{
public:
  ConvertFactory(const std::string& name)
    : DeviceFactory<CameraDriverInterface>(name)
  {
    Params() = {
      {"channel", "-1", "Particular channel to convert (-1 for all)."},
      {"fmt", "DEFAULT", "Output video format: MONO8, RGB8, BGR8"},
      {"size", "0x0", "Capture resolution (0x0 for unused)."},
      {"scale", "1.0", "Scale factor to apply to image."},
      {"bias", "0.0", "Bias factor to apply to image."}
  };
  }

  std::shared_ptr<CameraDriverInterface> GetDevice(const Uri& uri)
  {
    int         channel = uri.properties.Get<int>("channel", -1);
    std::string format  = uri.properties.Get<std::string>("fmt", "DEFAULT");
    ImageDim    size    = uri.properties.Get<ImageDim>("size", ImageDim(0, 0));
    float       scale   = uri.properties.Get<float>("scale", 1.0f);
    float       bias    = uri.properties.Get<float>("bias", 0.0f);

    const Uri input_uri = Uri(uri.url);

    // Create input camera.
    std::shared_ptr<CameraDriverInterface> input =
        DeviceRegistry<hal::CameraDriverInterface>::I().Create(input_uri);

    ConvertDriver* driver_ptr = new ConvertDriver(input, channel, format, size,
                                                  scale, bias);
    return std::shared_ptr<CameraDriverInterface>(driver_ptr);
  }
};

// Register this factory by creating static instance of factory.
static ConvertFactory g_ConvertFactory("convert");

}
