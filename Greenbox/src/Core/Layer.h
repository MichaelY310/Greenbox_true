#pragma once
#include <string>
#include "Log.h"
#include <vector>
#include "Event/Event.h"

namespace Greenbox {

	class Layer
	{
	public:
		Layer(const std::string name) : m_Name(name) { }
		virtual ~Layer() = default;
		virtual void OnAttach() {}; 
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		virtual void OnImGuiRender() {};
		virtual void OnEvent(Event& e) {};
		const std::string GetName() { return m_Name; }
	private:
		std::string m_Name;
	};


	// Two kinds of layers: Layer and OverLay
	// Layer: placed at the bottom in order
	// OverLay: placed at the top and get propogated first
	class LayerStack
	{
	public:
		LayerStack(std::string name = "LayerStack") : m_Name(name), m_LayerInsertIndex(0) {};

		~LayerStack()
		{
			for (Layer* layer : m_Layers) {
				delete layer;
			}
		}

		void PushLayer(Layer* layer)
		{
			m_Layers.emplace(begin() + m_LayerInsertIndex, layer);
			m_LayerInsertIndex++;
		}

		void PushOverLay(Layer* layer)
		{
			m_Layers.emplace_back(layer);
		}

		void PopLayer(Layer* layer)
		{
			auto target = std::find(begin(), begin() + m_LayerInsertIndex, layer);
			if (target != begin() + m_LayerInsertIndex)
			{
				delete layer;
				m_LayerInsertIndex--;
			}
			else {
				GB_INFO("LayerStack::PopLayer   ", "Layer: ", layer->GetName(), " not found in ", m_Name);
			}
		}

		void PopOverLay(Layer* layer)
		{
			auto target = std::find(begin() + m_LayerInsertIndex, end(), layer);
			if (target != end())
			{
				delete layer;
			}
			else {
				GB_INFO("LayerStack::PopOverLay   ", "OverLay: ", layer->GetName(), " not found in ", m_Name);
			}
		}

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::string m_Name;
		std::vector<Layer*> m_Layers;
		int m_LayerInsertIndex;
	};

}