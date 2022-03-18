//
//  MetalContext.h
//  Arcane
//
//  Created by Thomas Heeley on 19/03/22.
//

#ifndef MetalContext_h
#define MetalContext_h


#include "Arcane/Renderer/GraphicsContext.h"
#include "MetalDevice.h"

namespace Arcane {
    class MetalContext : public GraphicsContext
    {
    public:
        MetalContext();
        
        void SwapBuffers() override;
    private:
        MetalDevice* m_Device;
    };
}

#endif /* MetalContext_h */
