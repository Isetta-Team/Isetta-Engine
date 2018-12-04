/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/GUIModule.h"

#include "Core/Config/Config.h"
#include "Core/Debug/Logger.h"
#include "Core/Memory/MemoryManager.h"
#include "Graphics/Font.h"
#include "Graphics/GUI.h"
#include "Input/GLFWInput.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"

#include "SID/sid.h"
#include "brofiler/ProfilerCore/Brofiler.h"
#include "glad/glad.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void* MemAlloc(size_t size, void* user_data) {
  if (user_data) {
    // LOG_INFO(Isetta::Debug::Channel::GUI, {(char*)user_data, "alloc"});
  }
  return malloc(size);
  // return Isetta::MemoryManager::AllocOnStack(size);
}
void FreeAlloc(void* ptr, void* user_data) {
  if (user_data) {
    // LOG_INFO(Isetta::Debug::Channel::GUI, {(char*)user_data, "free"});
    return;
  }
  free(ptr);
}

namespace Isetta {
void GUIModule::StartUp(const GLFWwindow* win) {
  GUI::guiModule = this;

  winHandle = win;
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  // TODO(Jacob)
  // ImGui::SetAllocatorFunctions(
  //    [](size_t size, void* user_data) {
  //      return Isetta::MemoryManager::AllocOnFreeList(size);
  //    },
  //    [](void* ptr, void* user_data) {
  //      Isetta::MemoryManager::FreeOnFreeList(ptr);
  //    });
  ImGui::SetAllocatorFunctions(MemAlloc, FreeAlloc);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();

  // Setup Dear ImGui binding
  ImGui_ImplGlfw_InitForOpenGL(const_cast<GLFWwindow*>(winHandle), false);
  ImGui_ImplOpenGL3_Init();
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable
  // Gamepad Controls

  // Setup style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // io.IniFilename = NULL;
  // Load Fonts
  // io.Fonts->AddFontDefault();
  // io.Fonts->AddFontFromFileTTF("../External/imgui/misc/fonts/Roboto-Medium.ttf",
  // 16.0f); io.Fonts->AddFontFromFileTTF(
  //    "../External/imgui/misc/fonts/Cousine-Regular.ttf", 15.0f);
  // io.Fonts->AddFontFromFileTTF(
  //    "../External/imgui/misc/fonts/DroidSans.ttf", 72.0f);
  // io.Fonts->AddFontFromFileTTF("../External/imgui/misc/fonts/ProggyTiny.ttf", 10.0f);
  // ImFont* font =
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
  // NULL, io.Fonts->GetGlyphRangesJapanese());
  // IM_ASSERT(font != NULL);
  const std::string fontName = "Lato-Regular";
  const std::string filepath =
      CONFIG_VAL(resourcePath) + "\\fonts\\" + fontName + ".ttf";
  const float fontSize = CONFIG_VAL(guiConfig.defaultFontSize);
  auto font = io.Fonts->AddFontFromFileTTF(filepath.c_str(), fontSize);
  std::unordered_map<float, Font*> fontSizes{
      {fontSize, reinterpret_cast<Font*>(font)}};
  Font::fonts.insert({SID(fontName.c_str()), {filepath, fontSizes}});
  io.FontDefault = font;
  ASSERT(font != NULL);

  GLFWInput::RegisterMouseButtonCallback(ImGui_ImplGlfw_MouseButtonCallback);
  GLFWInput::RegisterScrollCallback(ImGui_ImplGlfw_ScrollCallback);
  GLFWInput::RegisterKeyCallback(ImGui_ImplGlfw_KeyCallback);
  GLFWInput::RegisterCharCallback(ImGui_ImplGlfw_CharCallback);
}

void GUIModule::Update(float deltaTime) {
  BROFILER_CATEGORY("GUI Update", Profiler::Color::PowderBlue);

  const bool empty = Font::loadFonts.empty();
  while (!Font::loadFonts.empty()) {
    auto [fontId, filepath, fontSize] = Font::loadFonts.top();
    auto font = reinterpret_cast<Font*>(
        ImGui::GetIO().Fonts->AddFontFromFileTTF(filepath.c_str(), fontSize));
    Font::fonts.find(fontId)->second.second.insert({fontSize, font});
    Font::loadFonts.pop();
  }
  if (!empty) ImGui_ImplOpenGL3_CreateDeviceObjects();

  // LOG_INFO(Isetta::Debug::Channel::GUI,
  //         "-------------GUI UPDATE 1-------------");
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  auto cursor = ImGui::GetMouseCursor();
  ImGui::NewFrame();
  // LOG_INFO(Isetta::Debug::Channel::GUI,
  //         "-------------GUI UPDATE 2-------------");
  glfwGetWindowSize(const_cast<GLFWwindow*>(winHandle), &winWidth, &winHeight);

  ImGui::SetMouseCursor(cursor);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
  ImGui::SetNextWindowBgAlpha(0.0f);
  ImGui::SetNextWindowPos(ImVec2{});
  ImGui::SetNextWindowSize(
      ImVec2{static_cast<float>(winWidth), static_cast<float>(winHeight)});
  ImGui::Begin(
      "###MainWindow", NULL,
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
          ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
          ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse |
          ImGuiWindowFlags_NoSavedSettings |
          ImGuiWindowFlags_NoFocusOnAppearing |
          ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);
  ImGui::PopStyleVar(2);

  // TODO Don't love this coupling
  LevelManager::Instance().loadedLevel->GUIUpdate();
  ImGui::End();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUIModule::ShutDown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

}  // namespace Isetta
