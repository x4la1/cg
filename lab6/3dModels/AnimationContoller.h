#pragma once
#include "pch.h"

class AnimationController
{
public:
	AnimationController()
	{
		Reset();
	}

	void Reset()
	{
		m_lastTick = GetTickCount64();
		m_delta = 0;
	}

	void Tick()
	{
		auto const currentTick = GetTickCount64();
		m_delta = currentTick - m_lastTick;
		m_lastTick = currentTick;
	}

	ULONGLONG GetTimeDelta() const
	{
		return m_delta;
	}
private:
	ULONGLONG m_lastTick;
	ULONGLONG m_delta;
};