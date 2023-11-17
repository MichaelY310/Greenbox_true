#include "gbpch.h"

#include "EntityInspectorPanel.h"
#include "Core/Log.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>


namespace Greenbox {

    EntityInspectorPanel::EntityInspectorPanel(const std::shared_ptr<Scene>& scene)
    {
        SetScene(scene);
    }

    void EntityInspectorPanel::SetScene(const std::shared_ptr<Scene>& scene)
    {
        m_Scene = scene;
        m_SelectedEntity = {};
    }

    void EntityInspectorPanel::OnImGuiRender()
    {
        // Entity Panel
        ImGui::Begin("Entities");

        m_Scene->m_Registry.each([&](auto entityID)
            {
                Entity entity{ entityID , m_Scene.get() };
                DrawEntityNode(entity);
            });

        // Cancel selection
        // left click on the empty space of the panel
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectedEntity = {};

        // right click create entity
        if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
                m_Scene->CreateEntity("Empty Entity");
            ImGui::EndPopup();
        }

        ImGui::End();


        // Entity Components Panel
        ImGui::Begin("Entity Components");
        if (m_SelectedEntity)
        {
            DrawComponents(m_SelectedEntity);
        }
        ImGui::End();
    }

    void EntityInspectorPanel::DrawEntityNode(Entity entity)
    {
        std::string& name = entity.GetComponent<NameComponent>().Name;

        ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        // ImGuiTreeNodeFlags_Selected:       highlights the node. 
        // ImGuiTreeNodeFlags_OpenOnArrow:    makes it expandable
        // ImGuiTreeNodeFlags_SpanAvailWidth: extend it to the end
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());
        if (ImGui::IsItemClicked())
        {
            m_SelectedEntity = entity;
        }

        // right click delete entity
        bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Entity"))
                entityDeleted = true;
            ImGui::EndPopup();
        }

        if (opened)
        {
            ImGuiTreeNodeFlags testFlag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
            bool testOpened = ImGui::TreeNodeEx((void*)114514, testFlag, name.c_str());
            if (testOpened)
                ImGui::TreePop();
            ImGui::TreePop();
        }

        if (entityDeleted)
        {
            m_Scene->DestroyEntity(entity);
            if (m_SelectedEntity == entity)
                m_SelectedEntity = {};
        }
    }

    static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
    {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        ImGui::PushID(label.c_str());   // pushes an identifier onto the ID stack (for example "Translation")

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }

    template<typename T, typename UIFunction>
    static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
    {
        if (entity.HasComponent<T>())
        {
            ImGuiConfigFlags flag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();

            bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flag, name.c_str());
            bool removed = false;

            ImGui::PopStyleVar();

            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
            {
                ImGui::OpenPopup("Component Settings");
            }
            if (ImGui::BeginPopup("Component Settings"))
            {
                if (ImGui::MenuItem("Remove Component"))
                {
                    removed = true;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }

            if (opened)
            {
                uiFunction(entity.GetComponent<T>());
                ImGui::TreePop();
            }

            if (removed)
            {
                entity.RemoveComponent<T>();
            }
        }
    }

    void EntityInspectorPanel::DrawComponents(Entity entity)
    {
        std::string& name = entity.GetComponent<NameComponent>().Name;
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), name.c_str());
        if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
        {
            name = std::string(buffer);
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);

        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("Add Component");
        }
        
        if (ImGui::BeginPopup("Add Component"))
        {

            if (!m_SelectedEntity.HasComponent<SpriteRendererComponent>())
            {
                if (ImGui::MenuItem("Sprite Renderer"))
                {
                    entity.AddComponent<SpriteRendererComponent>();
                    ImGui::CloseCurrentPopup();
                }
            }
            if (!m_SelectedEntity.HasComponent<TriangleRendererComponent>())
            {
                if (ImGui::MenuItem("Triangle Renderer"))
                {
                    entity.AddComponent<TriangleRendererComponent>();
                    ImGui::CloseCurrentPopup();
                }
            }
            if (!m_SelectedEntity.HasComponent<QuadRendererComponent>())
            {
                if (ImGui::MenuItem("Quad Renderer"))
                {
                    entity.AddComponent<QuadRendererComponent>();
                    ImGui::CloseCurrentPopup();
                }
            }
            if (!m_SelectedEntity.HasComponent<Box2DRigidbodyComponent>())
            {
                if (ImGui::MenuItem("Rigid Body"))
                {
                    entity.AddComponent<Box2DRigidbodyComponent>();
                    ImGui::CloseCurrentPopup();
                }
            }
            if (!m_SelectedEntity.HasComponent<Box2DBoxColliderComponent>())
            {
                if (ImGui::MenuItem("Box Collider"))
                {
                    entity.AddComponent<Box2DBoxColliderComponent>();
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndPopup();
        }

        ImGui::PopItemWidth();


        DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
            DrawVec3Control("Translation", component.Translation);
            glm::vec3 rotation = glm::degrees(component.Rotation);
            DrawVec3Control("Rotation", rotation);
            component.Rotation = glm::radians(rotation);
            DrawVec3Control("Scale", component.Scale, 1.0f);
        });

        DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component) {
            ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

            ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_BROWSER_ITEM"))
                {
                    const wchar_t* path = (const wchar_t*)payload->Data;
                    std::filesystem::path texturePath = std::filesystem::path(path);
                    std::string texturePath_string = texturePath.string();
                    const char* texturePath_char = texturePath_string.c_str();
                    component.Texture = Texture2D::Create(texturePath_char);
                }
                ImGui::EndDragDropTarget();
            }
        });

        DrawComponent<TriangleRendererComponent>("Triangle Renderer", entity, [](auto& component) {
            ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

            ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_BROWSER_ITEM"))
                {
                    const wchar_t* path = (const wchar_t*)payload->Data;
                    std::filesystem::path texturePath = std::filesystem::path(path);
                    std::string texturePath_string = texturePath.string();
                    const char* texturePath_char = texturePath_string.c_str();
                    component.Texture = Texture2D::Create(texturePath_char);
                }
                ImGui::EndDragDropTarget();
            }
        });

        DrawComponent<QuadRendererComponent>("Quad Renderer", entity, [](auto& component) {
            ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

            ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_BROWSER_ITEM"))
                {
                    const wchar_t* path = (const wchar_t*)payload->Data;
                    std::filesystem::path texturePath = std::filesystem::path(path);
                    std::string texturePath_string = texturePath.string();
                    const char* texturePath_char = texturePath_string.c_str();
                    component.Texture = Texture2D::Create(texturePath_char);
                }
                ImGui::EndDragDropTarget();
            }
        });

        DrawComponent<CameraComponent>("Camera", entity, [](auto& component) {
            auto& camera = component.camera;
            bool isPerspective = camera.GetMode() == 1;
            ImGui::Checkbox("Perspective", &isPerspective);
            if (isPerspective)
            {
                camera.SetMode(1);
            }
            else 
            {
                camera.SetMode(0);
            }

            float FOV = camera.GetFOV();
            if (ImGui::DragFloat("FOV", &FOV))
                camera.SetFOV(FOV);

            float Near = camera.GetNearClip();
            if (ImGui::DragFloat("Near", &Near))
                camera.SetNearClip(Near);

            float Far = camera.GetFarClip();
            if (ImGui::DragFloat("Far", &Far))
                camera.SetFarClip(Far);
        });

        DrawComponent<Box2DRigidbodyComponent>("Rigid Body", entity, [](auto& component) {
            const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
            const char* currentBodyTypeString = bodyTypeStrings[0];
            if (component.BodyType == b2_staticBody) { currentBodyTypeString = bodyTypeStrings[0]; }
            else if (component.BodyType == b2_dynamicBody) { currentBodyTypeString = bodyTypeStrings[1]; }
            else if (component.BodyType == b2_kinematicBody) { currentBodyTypeString = bodyTypeStrings[2]; }

            if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
            {
                for (int i = 0; i < 3; i++)
                {
                    bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
                    if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
                    {
                        currentBodyTypeString = bodyTypeStrings[i];
                        if (i == 0) { component.BodyType = b2_staticBody;  }
                        if (i == 1) { component.BodyType = b2_dynamicBody; }
                        if (i == 2) { component.BodyType = b2_kinematicBody; }
                    }

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }

            ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
        });

        DrawComponent<Box2DBoxColliderComponent>("Box Collider", entity, [](auto& component) {
            ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
            ImGui::DragFloat2("Size", glm::value_ptr(component.Offset));
            ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
        });
    }

}