#pragma once

#include <iostream>

struct FramebufferSpecifications
{
	uint32_t width = 0;
	uint32_t height = 0;
};

class Framebuffer
{
public:

	static Framebuffer* Create(FramebufferSpecifications& specs);
private:

};