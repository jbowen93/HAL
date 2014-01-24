#include <HAL/Devices/DeviceFactory.h>
#include "ProtoReaderDriver.h"

namespace hal
{

class ProtoReaderFactory : public DeviceFactory<CameraDriverInterface>
{
public:
    ProtoReaderFactory(const std::string& name)
        : DeviceFactory<CameraDriverInterface>(name)
    {
        Params() = {
            {"id","0","Camera id."},
            {"name", "ProtoCam", "Camera name."},
            {"startframe", "0", "First frame to capture."}
        };
    }

    std::shared_ptr<CameraDriverInterface> GetDevice(const Uri& uri)
    {
        unsigned int nCamId     = uri.properties.Get<unsigned int>("id", 0);
        std::string sName       = uri.properties.Get<std::string>("name", "ProtoCam");

        const std::string file = ExpandTildePath(uri.url);
        size_t startframe  = uri.properties.Get("startframe", 0);

        ProtoReaderDriver* driver = new ProtoReaderDriver(file,startframe);
        return std::shared_ptr<CameraDriverInterface>( driver );
    }
};

// Register this factory by creating static instance of factory
static ProtoReaderFactory g_ProtoReaderFactory1("proto");
static ProtoReaderFactory g_ProtoReaderFactory2("log");

}
