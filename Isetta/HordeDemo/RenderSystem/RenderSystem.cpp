#define _CRT_SECURE_NO_WARNINGS

#include "RenderSystem.h"
#include <iostream>


RenderSystem::RenderSystem(const char * title, int renderer, float fov, float near_plane, float far_plane, int width, int height, bool fullscreen, bool show_cursor)
    : renderInterface{ renderer },
    resourcePath{ "..\\Content" },
    winTitle{ title },
    winHandle{ nullptr },
    initWinWidth{ width },
    initWinHeight{ height },
    fov{ fov },
    nearPlane{ near_plane },
    farPlane{far_plane},
    winFullScreen{ fullscreen },
    winShowCursor{ show_cursor } {

}

RenderSystem::~RenderSystem() {
}

bool RenderSystem::StartUp() {
    glfwInit();

    if (!initWindow()) {
        return false;
    }

    glfwSetInputMode(winHandle, GLFW_STICKY_KEYS, GL_TRUE);

    if (!initH3D()) {
        return false;
    }
    initConfig();

    // Init resources
    if (!initResources()) {
        std::cout << "Unable to initialize resources" << std::endl;

        h3dutDumpMessages();
        return false;
    }
    return true;
}

void RenderSystem::showCursor(bool visible) {
    glfwSetInputMode(winHandle, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

    winShowCursor = visible;
}

bool RenderSystem::initWindow() {
    // Create OpenGL window
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    if (renderInterface == H3DRenderDevice::OpenGL4) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    if (winFullScreen) {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        winHandle = glfwCreateWindow(mode->width, mode->height, winTitle.c_str(), glfwGetPrimaryMonitor(), NULL);
    }
    else {
        winHandle = glfwCreateWindow(initWinWidth, initWinHeight, winTitle.c_str(), NULL, NULL);
    }

    if (winHandle == NULL) {
        // Fake message box
        glfwDestroyWindow(winHandle);

        winHandle = glfwCreateWindow(800, 50, "Unable to initialize engine - Make sure you have an OpenGL 2.0 compatible graphics card", NULL, NULL);
        double startTime = glfwGetTime();
        while (glfwGetTime() - startTime < 5.0) { /* Sleep */ }

        std::cout << "Unable to initialize window" << std::endl;
        std::cout << "Make sure you have an OpenGL " << ((renderInterface == H3DRenderDevice::OpenGL2) ? "2.0" : "4.3") << " compatible graphics card" << std::endl;

        return false;
    }

    glfwSetWindowUserPointer(winHandle, this);
    glfwMakeContextCurrent(winHandle);

    // Init cursor
    showCursor(winShowCursor);
    return true;
}

void RenderSystem::initConfig() {
    // Set options
    h3dSetOption(H3DOptions::LoadTextures, 1);
    h3dSetOption(H3DOptions::TexCompression, 0);
    h3dSetOption(H3DOptions::MaxAnisotropy, 4);
    h3dSetOption(H3DOptions::ShadowMapSize, 2048);
    h3dSetOption(H3DOptions::FastAnimation, 1);
    h3dSetOption(H3DOptions::SampleCount, 0);
    h3dSetOption(H3DOptions::DumpFailedShaders, 1);
    // Disable vertical synchronization
    glfwSwapInterval(0);
}

bool RenderSystem::initH3D() {
    // Initialize horde3d
    if (!h3dInit((H3DRenderDevice::List) renderInterface)) {
        std::cout << "Unable to initialize engine" << std::endl;

        h3dutDumpMessages();
        return false;
    }
    return true;
}

bool RenderSystem::initResources() {
    // 1. Add resources

    // Pipelines
    pipelineRes[0] = h3dAddResource(H3DResTypes::Pipeline, "pipelines/forward.pipeline.xml", 0);
    pipelineRes[1] = h3dAddResource(H3DResTypes::Pipeline, "pipelines/deferred.pipeline.xml", 0);
    pipelineRes[2] = h3dAddResource(H3DResTypes::Pipeline, "pipelines/hdr.pipeline.xml", 0);

    // Overlays
    fontMatRes = h3dAddResource(H3DResTypes::Material, "overlays/font.material.xml", 0);
    panelMatRes = h3dAddResource(H3DResTypes::Material, "overlays/panel.material.xml", 0);
    logoMatRes = h3dAddResource(H3DResTypes::Material, "overlays/logo.material.xml", 0);

    if (!h3dutLoadResourcesFromDisk(resourcePath.c_str())) {
        std::cout << "Error in loading resources" << std::endl;

        h3dutDumpMessages();
        return false;
    }

    cam = h3dAddCameraNode(H3DRootNode, "Camera", pipelineRes[curPipeline]);
    h3dSetNodeParamI(cam, H3DCamera::OccCullingI, 1);
    h3dSetNodeTransform(cam, 0, 50, 600, 0, 0, 0, 1, 1, 1);

    // Setup camera and resize buffers
    resizeViewport();

    h3dutDumpMessages();

    return true;
}

void RenderSystem::resizeViewport() {
    int width, height;
    if (winHandle) {
        glfwGetWindowSize(winHandle, &width, &height);
    }
    else {
        width = -1;
        height = -1;
    }

    // Resize viewport
    h3dSetNodeParamI(cam, H3DCamera::ViewportXI, 0);
    h3dSetNodeParamI(cam, H3DCamera::ViewportYI, 0);
    h3dSetNodeParamI(cam, H3DCamera::ViewportWidthI, width);
    h3dSetNodeParamI(cam, H3DCamera::ViewportHeightI, height);

    // Set virtual camera parameters
    h3dSetupCameraView(cam, fov, (float)width / height, nearPlane, farPlane);
    h3dResizePipelineBuffers(pipelineRes[0], width, height);
    h3dResizePipelineBuffers(pipelineRes[1], width, height);
    h3dResizePipelineBuffers(pipelineRes[2], width, height);
}

void RenderSystem::Update() {
    if (!cam) return;

    glfwPollEvents();

    // Render scene
    h3dRender(cam);

    h3dFinalizeFrame();
    h3dutDumpMessages();
    glfwSwapBuffers(winHandle);
}

void RenderSystem::ShutDown() {
    if (winHandle) {
        // Release engine
        h3dRelease();

        // Destroy window
        glfwDestroyWindow(winHandle);
        winHandle = nullptr;
    }
}
