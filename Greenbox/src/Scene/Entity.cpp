#include "gbpch.h"

#include "Entity.h"

namespace Greenbox {

	Entity::Entity(entt::entity entityHandle, Scene* scene) : 
		m_EntityHandle(entityHandle), m_Scene(scene) 
	{
	
	}

}