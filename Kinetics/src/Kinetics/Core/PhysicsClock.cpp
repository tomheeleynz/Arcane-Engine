#include "Kinetics/Core/Clock.h"

namespace Kinetics
{
	// Library Includes
#include <chrono>

// Local Includes

// This Includes
#include "Kinetics/Core/Clock.h"


// Implementation
//****************************************************
// PhysicsClock: PhysicsClock Class Constructor
// @author: 
// @parameter: No parameters
//
// @return: none
//*****************************************************
	PhysicsClock::PhysicsClock()
		: m_fTimeElapsed(0.0f)
		, m_fDeltaTime(0.0f)
	{

	}

	//****************************************************
	// ~PhysicsClock: PhysicsClock Class Destructor
	// @author: 
	// @parameter: No parameters
	//
	// @return: none
	//*****************************************************
	PhysicsClock::~PhysicsClock()
	{

	}

	//****************************************************
	// Initialise: PhysicsClock Class Initialiser - sets the first time values
	// @author: 
	// @parameter: No parameters
	//
	// @return: true if initialisation is successful, false if not
	//*****************************************************
	bool PhysicsClock::Initialise()
	{
		m_fCurrentTime = std::chrono::high_resolution_clock::now();
		return (true);
	}

	//****************************************************
	// Process: processes the change in time since it was last called
	// @author:
	// @parameter: No parameters
	//
	// @return: void
	//*****************************************************
	void
		PhysicsClock::Process()
	{
		m_fLastTime = m_fCurrentTime;

		m_fCurrentTime = std::chrono::high_resolution_clock::now();


		m_fDeltaTime = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(m_fCurrentTime - m_fLastTime).count());

		m_fTimeElapsed += m_fDeltaTime;
	}

	//****************************************************
	// GetDeltaTick: gets the current delta tick value
	// @author: 
	// @parameter: No parameters
	//
	// @return: the current delta tick value
	//*****************************************************
	float PhysicsClock::GetDeltaTick()
	{
		return (static_cast<float>(m_fDeltaTime));
	}

	void PhysicsClock::Reset()
	{
		m_fCurrentTime = std::chrono::high_resolution_clock::now();
	}
}