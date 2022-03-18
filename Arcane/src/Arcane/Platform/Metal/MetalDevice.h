//
//  MetalDevice.h
//  Arcane
//
//  Created by Thomas Heeley on 19/03/22.
//

#ifndef MetalDevice_h
#define MetalDevice_h


namespace Arcane {
    class MetalDevice
    {
    public:
        MetalDevice();
        
    private:
        struct DeviceImpl;
        DeviceImpl* m_DeviceImpl;
    };
}


#endif /* MetalDevice_h */
