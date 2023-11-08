#pragma once

#include "Core/Layer.h"
#include "Core/Event/ApplicationEvent.h"
#include "Core/Event/MouseEvent.h"

namespace Greenbox {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();
		
		void SetDarkThemeColors();
		void SetBlock(bool block) { m_Block = block; }
	private:
		bool m_Block = true;
	};

}