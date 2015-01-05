#pragma once

#include <wrl.h>

// ���ڻ�����ʱ�İ������ࡣ
ref class BasicTimer sealed
{
public:
	// ��ʼ���ڲ���ʱ��ֵ��
	BasicTimer()
	{
		if (!QueryPerformanceFrequency(&m_frequency))
		{
			throw ref new Platform::FailureException();
		}
		Reset();
	}
	
	// ����ʱ������Ϊ��ʼֵ��
	void Reset()
	{
		Update();
		m_startTime = m_currentTime;
		m_total = 0.0f;
		m_delta = 1.0f / 60.0f;
	}
	
	// ���¼�ʱ�����ڲ�ֵ��
	void Update()
	{
		if (!QueryPerformanceCounter(&m_currentTime))
		{
			throw ref new Platform::FailureException();
		}
		
		m_total = static_cast<float>(
			static_cast<double>(m_currentTime.QuadPart - m_startTime.QuadPart) /
			static_cast<double>(m_frequency.QuadPart)
			);
		
		if (m_lastTime.QuadPart == m_startTime.QuadPart)
		{
			// ����������˼�ʱ�����򽫱����Ч�� 60Hz ֡ʱ���ʱ��������
			m_delta = 1.0f / 60.0f;
		}
		else
		{
			m_delta = static_cast<float>(
				static_cast<double>(m_currentTime.QuadPart - m_lastTime.QuadPart) /
				static_cast<double>(m_frequency.QuadPart)
				);
		}
		
		m_lastTime = m_currentTime;
	}
	
	// �����һ�ε��� Reset()�����һ�ε��� Update()֮��ĳ���ʱ��(��)��
	property float Total
	{
		float get() { return m_total; }
	}
	
	// �ڶ� Update()��ǰ���ε���֮��ĳ���ʱ��(��)��
	property float Delta
	{
		float get() { return m_delta; }
	}

private:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_currentTime;
	LARGE_INTEGER m_startTime;
	LARGE_INTEGER m_lastTime;
	float m_total;
	float m_delta;
};
