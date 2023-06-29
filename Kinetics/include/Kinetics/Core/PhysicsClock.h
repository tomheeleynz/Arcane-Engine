#ifndef _CLOCK_H_
#define _CLOCK_H_

// Library Includes
#include <chrono>
#include <vector>

namespace Kinetics
{
	// Prototypes
	class PhysicsClock
	{
		// Member Functions
	public:
		PhysicsClock();
		~PhysicsClock();
		bool Initialise();
		void Process();
		float GetDeltaTick();
		void Reset();

	protected:

	private:
		PhysicsClock(const PhysicsClock& _kr);
		PhysicsClock& operator= (const PhysicsClock& _kr);

		// Member Variables
	public:

	protected:
		double m_fTimeElapsed;
		double m_fDeltaTime;
		std::chrono::high_resolution_clock::time_point m_fLastTime;
		std::chrono::high_resolution_clock::time_point m_fCurrentTime;

		std::vector<double> m_vecTimeHistory;

		long long m_llNumCounts;

	private:

	};
}

#endif // 