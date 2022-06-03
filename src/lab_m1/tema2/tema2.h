#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/tema2_camera.h"

#define Z_NEAR 0.01f
#define Z_FAR 200.0f

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
        void RenderPlayer(glm::mat4 modelMatrix, float cursorAngle);
        void RenderFloor(glm::mat4 modelMatrix);
        void RenderMaze(glm::mat4 modelMatrix);
        void RenderBullet(glm::mat4 modelMatrix);
        void RenderHealthbar(glm::mat3 modelMatrix);
        void RenderTime(glm::mat3 modelMatrix, float timeLeft);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        float fov;
        float offsetX, offsetY;
        int randomMazeNumber;
        float bulletInterval, bulletStart, squareSide, cursorAngle, timeLeft;
        bool fireBullet, battleMode;
        glm::mat4 modelMatrixBullet;
        glm::mat4 modelMatrix;
    };
}   // namespace m1
