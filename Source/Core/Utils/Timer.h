#pragma once

class CTimer
{
public:
	CTimer( float duration, float value = 0.f );
	void Update( float delta );

	const float& Value() const { return m_Value; }

	float GetElapsed() const { return m_Value * m_Duration; }
	void SetElapsed( float elapsed ) { m_Value = glm::clamp( elapsed / m_Duration, 0.f, 1.f ); }

	bool IsDone() const { return m_Value >= 1.f; }

	void SetDuration( const float& duration ) { m_Duration = duration; }
	const float& Duration() const { return m_Duration; }

	void Reset() { Reset( m_Duration ); };
	void Reset( float newDuraction );

private:
	float m_Value = 0.f;
	float m_Duration = 0.f;
};