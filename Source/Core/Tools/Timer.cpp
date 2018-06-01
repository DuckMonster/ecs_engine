#include "CorePCH.h"
#include "Timer.h"

/**	Constructor
*******************************************************************************/
Timer::Timer( float duration, float value ) : m_Duration(duration), m_Value(value)
{
}

/**	Update
*******************************************************************************/
void Timer::Update( float delta )
{
	m_Value += delta / m_Duration;
	m_Value = glm::clamp( m_Value, 0.f, 1.f );
}

/**	Reset
*******************************************************************************/
void Timer::Reset( float newDuration )
{
	m_Value = 0.f;
	m_Duration = newDuration;
}
