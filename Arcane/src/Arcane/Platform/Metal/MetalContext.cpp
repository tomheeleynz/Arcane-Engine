//
//  MetalContext.cpp
//  Arcane
//
//  Created by Thomas Heeley on 19/03/22.
//

#include <stdio.h>
#include "MetalContext.h"

namespace Arcane {
    MetalContext::MetalContext()
    {
        m_Device = new MetalDevice();
    }

    void MetalContext::SwapBuffers()
    {
        
    }
}
