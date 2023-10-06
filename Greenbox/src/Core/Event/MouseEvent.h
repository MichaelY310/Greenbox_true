#pragma once

#include "Event.h"

namespace Greenbox {

	class MouseScrollEvent : public Event
	{
	public:
		EventType GetEventType() const { return EventType::MouseScrolled; }
		static EventType GetClassEventType() { return EventType::MouseScrolled; }

		MouseScrollEvent(const float x, const float y)
			: m_MouseX(x), m_MouseY(y) 
		{
		}

		float GetX() const { return m_MouseX; }
		float GetY() const { return m_MouseY; }
	private:
		float m_MouseX, m_MouseY;
	};
}