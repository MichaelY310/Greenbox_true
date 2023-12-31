#pragma once

#include "../Scene.h"
#include "../Entity.h"
#include "../Components.h"
#include <memory>
#include <filesystem>

namespace Greenbox {

    class EntityInspectorPanel
    {
    public:
        EntityInspectorPanel() = default;
        EntityInspectorPanel(const std::shared_ptr<Scene>& scene);

        void SetScene(const std::shared_ptr<Scene>& scene);
        void SetSelectedEntity(Entity entity) { m_SelectedEntity = entity; };

        void OnImGuiRender();

        Entity GetSelectedEntity() const { return m_SelectedEntity; }
    private:
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);
    private:
        std::shared_ptr<Scene> m_Scene;
        Entity m_SelectedEntity;
    };

}