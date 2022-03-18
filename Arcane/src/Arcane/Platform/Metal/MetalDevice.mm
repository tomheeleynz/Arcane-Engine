//
//  MetalDevice.m
//  Arcane
//
//  Created by Thomas Heeley on 19/03/22.
//

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>

#include "MetalDevice.h"

namespace Arcane {
    struct MetalDevice::DeviceImpl
    {
        id<MTLDevice> device;
    };

    MetalDevice::MetalDevice()
    {
        m_DeviceImpl = new DeviceImpl();
        m_DeviceImpl->device = MTLCreateSystemDefaultDevice();
    }
}
