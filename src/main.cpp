#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <cstring>
#include <stdio.h>

#include "../include/RealEngine.h"

using namespace std;

const int width = 800, height = 800;

FrameBuffer *framebuffer;
Texture *luigiTexture, *gordacoTexture, *sceneTexture;
Mesh *luigiMesh, *gordacoMesh;

void Init()
{
    glClearColor(0.0, 0.0, 0.3, 1.0);
    glEnable(GL_DEPTH_TEST);

    //LUIGI THINGS
    Shader *luigifshader = new Shader(); luigifshader->Create("luigifshader", GL_FRAGMENT_SHADER);
    Shader *luigivshader = new Shader(); luigivshader->Create("luigivshader", GL_VERTEX_SHADER);
    ShaderProgram *luigiProgram = new ShaderProgram();
    luigiProgram->AttachShader(*luigifshader);
    luigiProgram->AttachShader(*luigivshader);
    luigiProgram->Link();

    Image *img = new Image(); img->LoadFromFile("luigiD.jpg");
    luigiTexture = new Texture();
    luigiTexture->SetData(img->GetData(), img->GetWidth(), img->GetHeight(), img->GetFormat(), img->GetFormat(), GL_UNSIGNED_BYTE);

    luigiProgram->AttachTexture("tex", *luigiTexture);

    luigiMesh = new Mesh();
    luigiMesh->LoadFromFile("luigi.obj");
    luigiMesh->SetShaderProgram(*luigiProgram);
    //

    //GORDACO THINGS
    Shader *gordacofshader = new Shader(); gordacofshader->Create("gordacofshader", GL_FRAGMENT_SHADER);
    Shader *gordacovshader = new Shader(); gordacovshader->Create("gordacovshader", GL_VERTEX_SHADER);
    ShaderProgram *gordacoProgram = new ShaderProgram();
    gordacoProgram->AttachShader(*gordacofshader);
    gordacoProgram->AttachShader(*gordacovshader);
    gordacoProgram->Link();

    img->LoadFromFile("luigiD.jpg");
    gordacoTexture = new Texture();
    gordacoTexture->SetData(img->GetData(), img->GetWidth(), img->GetHeight(), img->GetFormat(), img->GetFormat(), GL_UNSIGNED_BYTE);

    gordacoProgram->AttachTexture("tex", *gordacoTexture);

    gordacoMesh = new Mesh();
    gordacoMesh->LoadFromFile("gordaco.obj");
    gordacoMesh->SetShaderProgram(*gordacoProgram);
    //

    //SCENE THINGS
    sceneTexture = new Texture();
    sceneTexture->SetData(0, width, height, GL_RGB, GL_RGB, GL_FLOAT);
    sceneTexture->SetWrapMode(GL_REPEAT);
    sceneTexture->SetScaleMode(GL_LINEAR);
    //

    //FRAMEBUFFER THINGS
    framebuffer = new FrameBuffer(width, height);
    GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
    framebuffer->SetDrawingBuffers(2, buffers);
    //
}

float rot = 0.0f, luigiRot = 0.0f, appTime = 0.0f;

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 model(1.0f);
    appTime += 0.1f;
    luigiRot += 0.03f;
    vec3 axis(.0, 1.0, 0.0), translate(-0.3f, -0.3f, -1.5f), scale(0.004);
    mat4 T = glm::translate(model, translate);
    mat4 R = glm::rotate_slow(model, luigiRot, axis);
    mat4 S = glm::scale(model, scale);
    model = T * R * S;

    mat4 projection = perspective(45.0f * 3.1415f/180.0f, 1.0f, 0.1f, 100.0f);
    luigiMesh->GetShaderProgram()->SetUniform("projection", projection);
    luigiMesh->GetShaderProgram()->SetUniform("time", appTime);
    luigiMesh->GetShaderProgram()->SetUniform("model", model);

    framebuffer->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    luigiMesh->Draw(); //Render a sceneTexture
    framebuffer->UnBind();

    luigiMesh->Draw(); //Render a pantalla

    gordacoMesh->GetShaderProgram()->SetUniform("projection", projection);
    gordacoMesh->GetShaderProgram()->SetUniform("time", appTime);
    model = mat4(1.0f);
    translate = vec3(0.2f, -0.15f, -1.5f);
    T = glm::translate(model, translate);
    R = glm::rotate_slow(model, rot, axis);
    scale = vec3(0.003);
    S = glm::scale(model, scale);
    model = T * R * S;
    gordacoMesh->GetShaderProgram()->SetUniform("model", model);
    gordacoMesh->GetShaderProgram()->AttachTexture("tex", *framebuffer->GetTexture(GL_COLOR_ATTACHMENT0)); //Seteamos textura
    gordacoMesh->Draw(); //Dibujamos gordaco
}

bool IsPressed(int keyCode)
{
    int n;
    const Uint8 *state = SDL_GetKeyboardState(&n);
    int i = SDL_GetScancodeFromKey(keyCode);
    if(i < n && n >= 1) return state[i] == 1;
    return false;
}

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* win;
    win = SDL_CreateWindow("Real Engine", 100, 100, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    SDL_GLContext context;
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    context = SDL_GL_CreateContext(win);

    Init();
    bool running = true;
    while(running)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_RIGHT)) rot += 0.03;
            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_LEFT)) rot -= 0.03;
        }

        RenderScene();
        SDL_GL_SwapWindow(win);
        SDL_Delay(25);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
