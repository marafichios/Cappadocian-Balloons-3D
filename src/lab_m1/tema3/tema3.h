#pragma once

#include "components/simple_scene.h"
#include <Core/GPU/Mesh.h>


namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;

        




    private:
        void FrameStart() override;
        void RenderTerrain();
        void Update(float deltaTimeSeconds) override;

        void RenderSun(glm::vec3 sunPosition);

        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        Mesh* GenerateTerrainMesh(const std::string& name, int width, int height);

        void RenderSimpleMeshText(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2);

        void RenderBalloon(float x, float z, float y, const std::string& textureName);

        std::unordered_map<std::string, Texture2D*> mapTextures;



        const float GRAVITY = -9.81f;
        const float WIND_CHANGE_INTERVAL = 2.0f;
        const float MAX_WIND_STRENGTH = 0.5f;
        const float BALLOON_LIFT = 15.0f;
        bool isLiftActive = false;
        const float centerX = 0.0f;
        const float centerZ = 0.0f;

        float animationTime;

       // Lab9 lab9Instance;





    };
}   // namespace m1
