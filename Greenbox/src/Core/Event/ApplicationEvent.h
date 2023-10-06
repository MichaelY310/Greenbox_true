#pragma once
#include "Event.h"

namespace Greenbox {

	class WindowCloseEvent : public Event
	{
	public:
		EventType GetEventType() const { return EventType::WindowClose; }
		static EventType GetClassEventType() { return EventType::WindowClose; }

		WindowCloseEvent()
		{
		
		}
	private:
	};

	class WindowResizeEvent : public Event
	{
	public:
		EventType GetEventType() const { return EventType::WindowResize; }
		static EventType GetClassEventType() { return EventType::WindowResize; }

		WindowResizeEvent(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height)
		{

		}

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
	private:
		uint32_t m_Width;
		uint32_t m_Height;
	};

}