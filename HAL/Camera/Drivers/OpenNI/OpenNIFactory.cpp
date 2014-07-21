#include <HAL/Devices/DeviceFactory.h>
#include "OpenNIDriver.h"

namespace hal
{

class OpenNIFactory : public DeviceFactory<CameraDriverInterface>
{
public:
    OpenNIFactory(const std::string& name)
        : DeviceFactory<CameraDriverInterface>(name)
    {
        Params() = {
            {"size", "640x480", "Capture resolution."},
            {"fps", "30", "Capture framerate."},
            {"rgb", "false", "Capture RGB image."},
            {"depth", "false", "Capture depth image."},
            {"ir", "true", "Capture infrared image."},
            {"align", "false", "Align depth with RGB."},
        };
    }

    std::shared_ptr<CameraDriverInterface> GetDevice(const Uri& uri)
    {
        ImageDim Dims       = uri.properties.Get("size", ImageDim(640,480));
        unsigned int nFPS   = uri.properties.Get("fps", 30);
        bool bRGB           = uri.properties.Get("rgb", false);
        bool bDepth         = uri.properties.Get("depth", false);
        bool bIR            = uri.properties.Get("ir", true);
        bool bAlign         = uri.properties.Get("align", false);

        OpenNIDriver* pDriver = new OpenNIDriver(
                    Dims.x, Dims.y, nFPS, bRGB, bDepth, bIR, bAlign
                    );
        return std::shared_ptr<CameraDriverInterface>( pDriver );
    }
};

// Register this factory by creating static instance of factory
static OpenNIFactory g_OpenNIFactory("openni");

}
