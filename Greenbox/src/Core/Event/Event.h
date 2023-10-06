#pragma once
#include <functional>

namespace Greenbox {

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	class Event
	{
	public:
		bool m_Handled = false;
		virtual EventType GetEventType() const = 0;
	private:
		//EventType m_EventType;
	};

	class EventDispatcher
	{
	public:
		template<typename T>	// T for example, WindowCloseEvent
		static bool Dispatch(Event& e, std::function<bool(T&)> handlerFn)	// handlerFn for example, Application::OnWindowClose
		{
			if (e.GetEventType() == T::GetClassEventType())
			{
				e.m_Handled = handlerFn(*(T*)&e);
				return true;
			}
			return false;
		}
	};

	static EventDispatcher dispatcher;

}