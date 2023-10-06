#include <Greenbox.h>
#include <Core/Entrypoint.h>
#include "RendererLayer.h"

namespace Greenbox {

	class SandboxApp : public Application
	{
	public:
		SandboxApp()
			: Application("Sandbox App")
		{
			PushLayer(new RendererLayer());
		}

		~SandboxApp()
		{
			
		}
	};

	Application* CreateApplication() { return new SandboxApp(); }

}