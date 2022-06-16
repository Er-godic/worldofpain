#pragma once
#include <chrono>

class Clock
{
	using clock = std::chrono::high_resolution_clock;
	std::chrono::time_point<clock> m_start;

public:
	using seconds = std::chrono::duration<float>;

	void start() { m_start = clock::now(); }

	seconds tick()
	{
		auto time = clock::now();
		seconds dt = time - m_start;
		m_start = time;
		return dt;
	}
};
