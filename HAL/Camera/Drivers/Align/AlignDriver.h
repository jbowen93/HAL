#pragma once

#include <memory>

#include <HAL/Camera/CameraDriverInterface.h>
#include <HAL/Utils/Uri.h>
#include <calibu/Calibu.h>
#include "depth_registration.h"

namespace hal
{

class AlignDriver : public CameraDriverInterface
{
public:
    AlignDriver(const calibu::CameraRig& rig, int idx,
                std::shared_ptr<CameraDriverInterface> Input);

    bool Capture( pb::CameraMsg& vImages );
    std::shared_ptr<CameraDriverInterface> GetInputDevice() { return m_Input; }

    std::string GetDeviceProperty(const std::string& sProperty);

    size_t NumChannels() const;
    size_t Width( size_t idx = 0 ) const;
    size_t Height( size_t idx = 0 ) const;

private:
    unsigned int sizeofType(pb::Type type) const;
    unsigned int sizeofFormat(pb::Format format) const;

protected:
    const calibu::CameraRig&                         m_Rig;
    int                                              m_RefIdx;
    std::shared_ptr<CameraDriverInterface>           m_Input;
    std::vector<std::shared_ptr<DepthRegistration>>  m_DepthReg;

    std::vector<unsigned int>                        m_nImgWidth;
    std::vector<unsigned int>                        m_nImgHeight;
};

}
