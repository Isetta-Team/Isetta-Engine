#pragma once

#include <Horde3D.h>
#include <Horde3DUtils.h>
#include <GLFW/glfw3.h>
#include <string>

class RenderSystem {
public:
    RenderSystem(
        const char* title = "Hord3D Sample",
        int renderer = H3DRenderDevice::OpenGL4,
        float fov = 45.0f, float near_plane = 0.1f, float far_plane = 1000.0f,
        int width = 1024, int height = 576,
        bool fullscreen = false,
        bool show_cursor = false
    );
    ~RenderSystem();

    bool StartUp();
    void ShutDown();

    void Update();

    std::string getResourcePath() const { return resourcePath; }

    GLFWwindow* winHandle;

private:
    int renderInterface;
    std::string resourcePath;
    std::string winTitle;
    int initWinWidth;
    int initWinHeight;
    float fov;
    float nearPlane;
    float farPlane;
    bool winFullScreen;
    bool winShowCursor;

    void showCursor(bool visible);
    bool initWindow();
    void initConfig();
    bool initH3D();
    bool initResources();
    void resizeViewport();

    // Engine objects
    int          curPipeline;
    H3DRes       pipelineRes[3];
    H3DRes       fontMatRes, panelMatRes, logoMatRes;
    H3DNode      cam;
};

