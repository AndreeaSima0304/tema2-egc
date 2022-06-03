#include "lab_m1/tema2/tema2.h"
#include "lab_m1/tema2/object2D.h"
#include "lab_m1/tema2/transform2D.h"
#include "lab_m1/tema2/transform3D.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;



Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    renderCameraTarget = true;
    fov = 60;
    offsetX = 0;
    offsetY = 0;
    randomMazeNumber = rand() % 3 + 1;
    bulletInterval = 0;
    bulletStart = 0;
    fireBullet = false;
    battleMode = false;
    squareSide = 0.5f;
    timeLeft = 100;
    modelMatrixBullet = glm::mat4(1);
    modelMatrix = glm::mat4(1);

    glm::vec3 corner = glm::vec3(0, 0, 0);

    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 0.65f, 0), glm::vec3(0, 1, 0));

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* healthbar = object2D::CreateSquare("healthbar", corner, squareSide, glm::vec3(1, 0, 0), true);
        AddMeshToList(healthbar);
    }

    {
        Mesh* healthbarframe = object2D::CreateSquare("healthbarframe", corner, squareSide, glm::vec3(0, 0, 0));
        AddMeshToList(healthbarframe);
    }

    {
        Mesh* time = object2D::CreateSquare("time", corner, squareSide, glm::vec3(0, 0.4f, 0), true);
        AddMeshToList(time);
    }

    projectionMatrix = glm::ortho(- offsetX, offsetX,  - offsetY, offsetY, Z_NEAR, Z_FAR);
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, Z_NEAR, Z_FAR);

}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.5f, 0.8f, 0.9f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::RenderMaze(glm::mat4 modelMatrix)
{
    // First maze
    int mazeMatrix1[20][20] = { {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1},
                                {1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1},
                                {1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1},
                                {1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1},
                                {1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1},
                                {1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1},
                                {1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1},
                                {1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1},
                                {1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
                                {1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
                                {1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
                                {1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1},
                                {1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} };
    // Second maze
    int mazeMatrix2[20][20] = { {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
                                {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
                                {1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
                                {1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                {1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1},
                                {1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1},
                                {1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1},
                                {1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
                                {1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
                                {1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} };
    // Third maze
    int mazeMatrix3[20][20] = { {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1},
                                {1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1},
                                {1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1},
                                {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1},
                                {1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1},
                                {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} };
    // Render a random maze
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (randomMazeNumber == 1 && mazeMatrix1[i][j] == 1) {
                modelMatrix = glm::mat4(1);
                modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(i - 9, 0.5f, -j + 10));
                RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
            }
            else if (randomMazeNumber == 2 && mazeMatrix2[i][j] == 1) {
                modelMatrix = glm::mat4(1);
                modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(i - 9, 0.5f, -j + 10));
                RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
            }
            else if (randomMazeNumber == 3 && mazeMatrix3[i][j] == 1) {
                modelMatrix = glm::mat4(1);
                modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
                modelMatrix = glm::translate(modelMatrix, glm::vec3(i - 9, 0.5f, -j + 10));
                RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
            }
        }
    }
}

void Tema2::RenderFloor(glm::mat4 modelMatrix)
{
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(1, -0.05f, 1));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(40, 0.1f, 40));
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);
}

void Tema2::RenderPlayer(glm::mat4 modelMatrix, float cursorAngle) 
{
    // Head
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::rotate(modelMatrix, -atan2(camera->right[2], camera->right[0]), glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
    RenderMesh(meshes["box"], shaders["Color"], modelMatrix);

    // Eyes
    modelMatrix = glm::scale(modelMatrix, glm::vec3(3.33f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.05f, -0.2f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 1));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
    RenderMesh(meshes["box"], shaders["VertexColor"], modelMatrix);

    // Body
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.5f, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.6f, 0.2f));
    RenderMesh(meshes["box"], shaders["VertexColor"], modelMatrix);

    // Hands
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.3f, -0.5f, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.5f, 0.1f));
    RenderMesh(meshes["box"], shaders["Color"], modelMatrix);
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.3f, -0.5f, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.5f, 0.1f));
    RenderMesh(meshes["box"], shaders["Color"], modelMatrix);

    // Legs
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.1f, -1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.19f, 0.5f, 0.1f));
    RenderMesh(meshes["box"], shaders["Color"], modelMatrix);
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.1f, -1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.19f, 0.5f, 0.1f));
    RenderMesh(meshes["box"], shaders["Color"], modelMatrix);
}

void Tema2::RenderBullet(glm::mat4 modelMatrixBullet)
{
    
    modelMatrixBullet = glm::translate(modelMatrixBullet, glm::vec3(bulletStart, -0.5f, 0));
    modelMatrixBullet = glm::scale(modelMatrixBullet, glm::vec3(0.1f));
    RenderMesh(meshes["sphere"], shaders["VertexColor"], modelMatrixBullet);
}

void Tema2::RenderHealthbar(glm::mat3 modelMatrixHealthbar)
{
    modelMatrixHealthbar *= transform2D::Translate(-0.5f, 1.8f);
    modelMatrixHealthbar *= transform2D::Scale(2, 0.3f);
    RenderMesh2D(meshes["healthbar"], shaders["VertexColor"], modelMatrixHealthbar);
    RenderMesh2D(meshes["healthbarframe"], shaders["VertexColor"], modelMatrixHealthbar);
}

void Tema2::RenderTime(glm::mat3 modelMatrixTime, float timeLeft)
{
    modelMatrixTime *= transform2D::Translate(-0.5f, 2);
    modelMatrixTime *= transform2D::Scale(2*timeLeft/100, 0.3f);
    RenderMesh2D(meshes["time"], shaders["VertexColor"], modelMatrixTime);
    modelMatrixTime = glm::mat3(1);
    modelMatrixTime *= transform2D::Translate(-0.5f, 2);
    modelMatrixTime *= transform2D::Scale(2, 0.3f);
    RenderMesh2D(meshes["healthbarframe"], shaders["VertexColor"], modelMatrixTime);
}


void Tema2::Update(float deltaTimeSeconds)
{
    timeLeft -= deltaTimeSeconds;

    glm::mat3 modelMatrixHealthbar = glm::mat3(1);
    RenderHealthbar(modelMatrixHealthbar);

    glm::mat3 modelMatrixTime = glm::mat3(1);
    RenderTime(modelMatrixTime, timeLeft);

    glm::mat4 modelMatrix = glm::mat4(1);
    RenderFloor(modelMatrix);

    if (renderCameraTarget)
    {
        modelMatrix = glm::mat4(1);
        RenderPlayer(modelMatrix, cursorAngle);
    }

    RenderMaze(modelMatrix);

    bulletInterval += deltaTimeSeconds;
    bulletStart += deltaTimeSeconds * 10;
    if (fireBullet == true) {
        RenderBullet(modelMatrixBullet);
    }

    if (timeLeft <= 0) 
    {
        printf("---TIME'S UP---\n");
        printf("---GAME OVER---\n");
        window->Close();
    }
}


void Tema2::FrameEnd()
{
   DrawCoordinateSystem({{0, 1000, 1000, 1000}, {1000, 0, 1000, 1000}, {1000, 1000, 0, 1000}, {1000, 1000, 1000, 0}}, projectionMatrix);
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}



void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    {
        float cameraSpeed = 4.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            camera->MoveForward(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            camera->TranslateRight(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            camera->MoveForward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            camera->TranslateRight(cameraSpeed * deltaTime);
        }
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_B) {
        // Enter battle mode
        if (battleMode == false)
        {
            camera->TranslateForward(camera->distanceToTarget);
            renderCameraTarget = false;
            battleMode = true;
        }
        // Exit battle mode
        else
        {
            camera->TranslateForward(-camera->distanceToTarget);
            renderCameraTarget = true;
            battleMode = false;
        }
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

    float sensivityOX = 0.01f;
    float sensivityOY = 0.01f;

    if (!renderCameraTarget) {
        camera->RotateFirstPerson_OX(-deltaY * sensivityOY);
        camera->RotateFirstPerson_OY(-deltaX * sensivityOX);
    }

    if (renderCameraTarget) {
        camera->RotateThirdPerson_OX(-deltaY * sensivityOY);
        camera->RotateThirdPerson_OY(-deltaX * sensivityOX);
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Bullet movement
    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && bulletInterval > 0.5f && battleMode == true) {
        modelMatrixBullet = glm::mat4(1);
        // Reset time interval for a shot
        bulletInterval = 0; 
        bulletStart = 0;
        // Spawn the bullet
        fireBullet = true;
        // Rotate bullet after camera
        modelMatrixBullet = glm::translate(modelMatrixBullet, camera->GetTargetPosition());
        modelMatrixBullet = glm::rotate(modelMatrixBullet, -atan2(camera->right[2], camera->right[0])+3.14f/2, glm::vec3(0, 1, 0));
    }
    else {
        fireBullet = false;
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
