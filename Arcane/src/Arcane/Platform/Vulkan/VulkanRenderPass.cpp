#include "VulkanRenderPass.h"
#include "Arcane/Core/Application.h"
#include "VulkanContext.h"

namespace Arcane {

    VulkanRenderPass::VulkanRenderPass(RenderPassSpecs specs)
    {
        m_Specifications = specs;
    }

    RenderPassSpecs& VulkanRenderPass::GetRenderPassSpecs()
    {
        return m_Specifications;
    }
}