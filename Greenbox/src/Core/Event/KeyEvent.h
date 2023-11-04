#pragma once

#include "Event.h"

namespace Greenbox {

	class KeyPressedEvent : public Event
	{
	public:
		EventType GetEventType() const { return EventType::KeyPressed; }
		static EventType GetClassEventType() { return EventType::KeyPressed; }

		KeyPressedEvent(int keycode, int repeatCount)
			: m_KeyCode(keycode), m_RepeatCount(repeatCount) {}

		int GetKeyCode() { return m_KeyCode; }
		int GetRepeatCount() { return m_RepeatCount; }

	private:
		int m_KeyCode;
		int m_RepeatCount;
	};

	class KeyReleasedEvent : public Event
	{
	public:
		EventType GetEventType() const { return EventType::KeyReleased; }
		static EventType GetClassEventType() { return EventType::KeyReleased; }

		KeyReleasedEvent(int keycode)
			: m_KeyCode(keycode) {}

		int GetKeyCode() { return m_KeyCode; }

	private:
		int m_KeyCode;
	};

}