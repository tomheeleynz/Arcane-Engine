#include "UUID.h"

#include <random>

namespace Arcane
{
	namespace Core
	{
		static std::random_device s_RandomDevice;
		static std::mt19937_64 s_Engine(s_RandomDevice());
		static std::uniform_int_distribution<uint64_t> m_UniformDistribution;

		UUID::UUID() : m_UUID(m_UniformDistribution(s_Engine))
		{
		}

		UUID::UUID(uint64_t uuid) : m_UUID(uuid)
		{
		}
	}

}
