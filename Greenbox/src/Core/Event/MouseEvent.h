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

	class MouseButtonPressedEvent : public Event
	{
	public:
		EventType GetEventType() const { return EventType::MouseButtonPressed; }
		static EventType GetClassEventType() { return EventType::MouseButtonPressed; }

		MouseButtonPressedEvent(int button)
			: m_Button(button)
		{
		}

		int GetButton() const { return m_Button; }
	private:
		int m_Button;
	};

	class MouseButtonReleasedEvent : public Event
	{
	public:
		EventType GetEventType() const { return EventType::MouseButtonReleased; }
		static EventType GetClassEventType() { return EventType::MouseButtonReleased; }

		MouseButtonReleasedEvent(int button)
			: m_Button(button)
		{
		}

		int GetButton() const { return m_Button; }
	private:
		int m_Button;
	};
}