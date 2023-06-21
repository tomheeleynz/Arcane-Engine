#pragma once

#include "CameraController.h"

class OrthographicController : public CameraController
{
public:
	OrthographicController();

	virtual void OnUpdate();
private:

};