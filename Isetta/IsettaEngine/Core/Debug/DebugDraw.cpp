/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Debug/DebugDraw.h"

#include "GLFW/include/GLFW/glfw3.h"
#include "glad/glad.h"

#include "Core/Debug/Logger.h"
#include "Core/Math/Matrix4.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"
#include "Core/Time/Clock.h"
#include "EngineLoop.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/RenderModule.h"
#include "Scene/Transform.h"
#include "brofiler/ProfilerCore/Brofiler.h"

namespace Isetta {
#define PLANE 1
#define PLANE_VERTICIES 4
#define PLANE_INDICIES 6
#define CUBE PLANE + PLANE_VERTICIES
#define CUBE_VERTICIES 8
#define CUBE_INDICIES 36
#define CUBE_WIRE_INDICIES 24
#define CUBE_VERTICIES 8
#define CIRCLE CUBE + CUBE_VERTICIES
#define CIRCLE_INDICIES 13
//#define CIRCLE_INDICIES 25
#define CIRCLE_VERTICIES 13
//#define CIRCLE_VERTICIES 25
#define SPHERE_SEGMENTS 4

#define RIGHT CIRCLE
#define UP CIRCLE + 6
#define FORWARD CIRCLE + CIRCLE_VERTICIES
const float DebugDraw::verticies[] = {
    0.0f, 0.0f, 0.0f,
    // PLANE - 4
    0.5f, 0.5f, 0.0f,    // top-front-right
    0.5f, -0.5f, 0.0f,   // bot-front-right
    -0.5f, -0.5f, 0.0f,  // bot-front-left
    -0.5f, 0.5f, 0.0f,   // top-front-left
    // CUBE - 8
    0.5f, 0.5f, 0.5f,     // top-front-right
    0.5f, -0.5f, 0.5f,    // bot-front-right
    -0.5f, -0.5f, 0.5f,   // bot-front-left
    -0.5f, 0.5f, 0.5f,    // top-front-left
    0.5f, -0.5f, -0.5f,   // bot-back-right
    0.5f, 0.5f, -0.5f,    // top-back-right
    -0.5f, -0.5f, -0.5f,  // bot-back-left
    -0.5f, 0.5f, -0.5f,   // top-back-left
    // CIRCLE - 13 (deprecated 25)
    1.0f, 0.0f, 0.0f,  // right
    // 0.966f, 0.259f, 0.0f,    //
    0.866f, 0.5f, 0.0f,  //
                         // 0.707f, 0.707f, 0.0f,    //
    0.5f, 0.866f, 0.0f,  //
    // 0.259f, 0.966f, 0.0f,    //
    0.0f, 1.0f, 0.0f,  // up
    //-0.259f, 0.966f, 0.0f,   //
    -0.5f, 0.866f, 0.0f,  //
    //-0.707f, 0.707f, 0.0f,   //
    -0.866f, 0.5f, 0.0f,  //
    //-0.966f, 0.259f, 0.0f,   //
    -1.0f, 0.0f, 0.0f,  // left
    //-0.966f, -0.259f, 0.0f,  //
    -0.866f, -0.5f, 0.0f,  //
    //-0.707f, -0.707f, 0.0f,  //
    -0.5f, -0.866f, 0.0f,  //
    //-0.259f, -0.966f, 0.0f,  //
    0.0f, -1.0f, 0.0f,  // down
    // 0.259f, -0.966f, 0.0f,   //
    0.5f, -0.866f, 0.0f,  //
                          // 0.707f, -0.707f, 0.0f,   //
    0.866f, -0.5f, 0.0f,  //
    // 0.966f, -0.259f, 0.0f,   //
    1.0f, 0.0f, 0.0f,  //
    0.0f, 0.0f, 1.0f,  // forward
};
const int DebugDraw::indices[] = {
    // PLANE
    3 + PLANE, 1 + PLANE, 0 + PLANE,  //
    3 + PLANE, 2 + PLANE, 1 + PLANE,  //
    // Cube - triangles
    /// Front
    3 + CUBE, 1 + CUBE, 0 + CUBE,  //
    3 + CUBE, 2 + CUBE, 1 + CUBE,  //
    /// BACK
    7 + CUBE, 4 + CUBE, 6 + CUBE,  //
    7 + CUBE, 5 + CUBE, 4 + CUBE,  //
    /// RIGHT
    0 + CUBE, 1 + CUBE, 4 + CUBE,  //
    0 + CUBE, 4 + CUBE, 5 + CUBE,  //
    /// LEFT
    7 + CUBE, 6 + CUBE, 2 + CUBE,  //
    7 + CUBE, 2 + CUBE, 3 + CUBE,  //
    /// TOP
    7 + CUBE, 0 + CUBE, 5 + CUBE,  //
    7 + CUBE, 3 + CUBE, 0 + CUBE,  //
    /// BOT
    6 + CUBE, 4 + CUBE, 1 + CUBE,  //
    6 + CUBE, 1 + CUBE, 2 + CUBE,  //
    // CUBE - lines
    0 + CUBE, 3 + CUBE, 3 + CUBE, 2 + CUBE,  // Front - 1
    2 + CUBE, 1 + CUBE, 1 + CUBE, 0 + CUBE,  // Front - 2
    4 + CUBE, 5 + CUBE, 5 + CUBE, 7 + CUBE,  // Back - 1
    7 + CUBE, 6 + CUBE, 6 + CUBE, 4 + CUBE,  // Back - 2
    0 + CUBE, 5 + CUBE, 1 + CUBE, 4 + CUBE,  // other
    2 + CUBE, 6 + CUBE, 3 + CUBE, 7 + CUBE,  //
    // CIRCLE - 25
    0 + CIRCLE, 1 + CIRCLE,               //
    2 + CIRCLE, 3 + CIRCLE, 4 + CIRCLE,   //
    5 + CIRCLE, 6 + CIRCLE, 7 + CIRCLE,   //
    8 + CIRCLE, 9 + CIRCLE, 10 + CIRCLE,  //
    11 + CIRCLE, 0 + CIRCLE,
    // 12 + CIRCLE, 13 + CIRCLE,  //
    // 14 + CIRCLE, 15 + CIRCLE, 16 + CIRCLE,  //
    // 17 + CIRCLE, 18 + CIRCLE, 19 + CIRCLE,  //
    // 20 + CIRCLE, 21 + CIRCLE, 22 + CIRCLE,  //
    // 23 + CIRCLE, 0 + CIRCLE,                //
    // LINES
    0, RIGHT,    //
    0, UP,       //
    0, FORWARD,  //
};
float DebugDraw::lineVerticies[] = {
    0.0f, 0.0f, 0.0f,  //
    0.0f, 0.0f, 0.0f,  //
};

const char* DebugDraw::vertexShaderSource =
    "#version 330 core\n"
    "layout(location = 0) in vec3 vPos;"
    "uniform mat4 modelViewProjectionMat;"
    "void main() { gl_Position = modelViewProjectionMat * "
    "vec4(vPos, 1.0); }";
const char* DebugDraw::fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;"
    "uniform vec4 color;"
    "void main() { FragColor = color; }";
int DebugDraw::modelViewProjectionLoc, DebugDraw::colorLoc;
unsigned int DebugDraw::VBO, DebugDraw::VAO, DebugDraw::EBO, DebugDraw::sVBO,
    DebugDraw::sVAO;
int DebugDraw::shaderProgram;

std::list<std::pair<float, Action<>>> DebugDraw::durationDraw;

void DebugDraw::StartUp() {
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  // Vertex Shader
  const int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // check for shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    LOG_ERROR(Debug::Channel::Graphics,
              {"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n", infoLog});
  }

  // Fragment Shader
  const int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // check for shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    LOG_ERROR(Debug::Channel::Graphics,
              {"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n", infoLog});
  }

  // Link Shaders
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    LOG_ERROR(Debug::Channel::Graphics,
              {"ERROR::SHADER::PROGRAM::LINKING_FAILED\n", infoLog});
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glUseProgram(shaderProgram);
  modelViewProjectionLoc =
      glGetUniformLocation(shaderProgram, "modelViewProjectionMat");
  colorLoc = glGetUniformLocation(shaderProgram, "color");

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glGenVertexArrays(1, &sVAO);
  glGenBuffers(1, &sVBO);

  glBindVertexArray(sVAO);
  glBindBuffer(GL_ARRAY_BUFFER, sVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(lineVerticies), NULL, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
void DebugDraw::Update() {
  BROFILER_CATEGORY("Debug Draw Update", Profiler::Color::Peru);

  auto it = durationDraw.begin();
  while (it != durationDraw.end()) {
    if (it->first < 0) {
      it = durationDraw.erase(it);
    } else {
      it->first -= EngineLoop::GetGameClock().GetDeltaTime();
      it->second();
      ++it;
    }
  }
}

void DebugDraw::Clear() { durationDraw.clear(); }

void DebugDraw::ShutDown() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &sVAO);
  glDeleteBuffers(1, &sVBO);
}

void DebugDraw::Point(const Math::Vector3 point, const Color& color, float size,
                      float duration, bool depthTest) {
#ifdef _EDITOR
  durationDraw.push_back(
      std::pair(duration, std::bind(DrawPoint, point, color, size, depthTest)));
#endif
}
void DebugDraw::Line(const Math::Vector3& start, const Math::Vector3& end,
                     const Color& color, float thickness, float duration,
                     bool depthTest) {
#ifdef _EDITOR
  durationDraw.push_back(std::pair(
      duration, std::bind(DrawLine, start, end, color, thickness, depthTest)));
#endif
}
void DebugDraw::Ray(const Math::Vector3& start, const Math::Vector3& dir,
                    const Color& color, float thickness, float duration,
                    bool depthTest) {
#ifdef _EDITOR
  Line(
      start,
      start +
          CameraComponent::Main()
                  ->GetProperty<CameraComponent::Property::FAR_PLANE, float>() *
              dir.Normalized(),
      color, thickness, duration, depthTest);
#endif
}
void DebugDraw::Plane(const Math::Matrix4& transformation, const Color& color,
                      float duration, bool depthTest) {
#ifdef _EDITOR
  durationDraw.push_back(std::pair(
      duration,
      std::bind(OpenGLDraw, transformation, color, 0, depthTest, []() {
        glDrawElements(GL_TRIANGLES, PLANE_INDICIES, GL_UNSIGNED_INT, (void*)0);
      })));
#endif
}
void DebugDraw::WirePlane(const Math::Matrix4& transformation,
                          const Color& color, float thickness, float duration,
                          bool depthTest) {
#ifdef _EDITOR
  durationDraw.push_back(std::pair(
      duration,
      std::bind(OpenGLDraw, transformation, color, thickness, depthTest,
                []() { glDrawArrays(GL_LINE_LOOP, PLANE, PLANE_VERTICIES); })));
#endif
}
void DebugDraw::Cube(const Math::Matrix4& transformation, const Color& color,
                     float duration, bool depthTest) {
#ifdef _EDITOR
  durationDraw.push_back(std::pair(
      duration,
      std::bind(OpenGLDraw, transformation, color, 0, depthTest, []() {
        glDrawElements(GL_TRIANGLES, CUBE_INDICIES, GL_UNSIGNED_INT,
                       (void*)(sizeof(int) * (PLANE_INDICIES)));
      })));
#endif
}
void DebugDraw::WireCube(const Math::Matrix4& transformation,
                         const Color& color, float thickness, float duration,
                         bool depthTest) {
#ifdef _EDITOR
  durationDraw.push_back(std::pair(
      duration,
      std::bind(OpenGLDraw, transformation, color, thickness, depthTest, []() {
        glDrawElements(GL_LINES, CUBE_WIRE_INDICIES, GL_UNSIGNED_INT,
                       (void*)(sizeof(int) * (PLANE_INDICIES + CUBE_INDICIES)));
      })));
#endif
}
void DebugDraw::Sphere(const Math::Matrix4& transformation, const Color& color,
                       float duration, bool depthTest) {
#ifdef _EDITOR
  throw std::exception("DebugDraw::Sphere => NOT IMPLEMENTED");

  durationDraw.push_back(std::pair(
      duration, std::bind(DrawSphere, transformation, color, depthTest)));
#endif
}
void DebugDraw::Sphere(const Math::Vector3& position, float radius,
                       const Color& color, float duration, bool depthTest) {
#ifdef _EDITOR
  throw std::exception("DebugDraw::Sphere => NOT IMPLEMENTED");
  Sphere(Math::Matrix4::Transform(position, Math::Vector3::zero,
                                  radius * Math::Vector3::one),
         color, duration, depthTest);
#endif
}
void DebugDraw::WireSphere(const Math::Matrix4& transformation,
                           const Color& color, float thickness, float duration,
                           bool depthTest) {
#ifdef _EDITOR
  durationDraw.push_back(std::pair(
      duration,
      std::bind(DrawWireSphere, transformation, color, thickness, depthTest)));
#endif
}
void DebugDraw::WireSphere(const Math::Vector3& position, float radius,
                           const Color& color, float thickness, float duration,
                           bool depthTest) {
#ifdef _EDITOR
  WireSphere(Math::Matrix4::Transform(position, Math::Vector3::zero,
                                      radius * Math::Vector3::one),
             color, thickness, duration, depthTest);
#endif
}
void DebugDraw::WireCapsule(const Math::Matrix4& transformation, float radius,
                            float height, const Color& color, float thickness,
                            float duration, bool depthTest) {
#ifdef _EDITOR
  durationDraw.push_back(
      std::pair(duration, std::bind(DrawWireCapsule, transformation, radius,
                                    height, color, thickness, depthTest)));
#endif
}
void DebugDraw::Grid(const Math::Matrix4& transformation, int lines,
                     const Color& color, float thickness, float duration) {
#ifdef _EDITOR
  durationDraw.push_back(std::pair(
      duration, std::bind(DrawGrid, transformation, lines, color, thickness)));
#endif
}
void DebugDraw::Axis(const Math::Matrix4& transformation, const Color& xColor,
                     const Color& yColor, const Color& zColor, float thickness,
                     float duration, bool depthTest) {
#ifdef _EDITOR
  Math::Vector3 start = static_cast<Math::Vector3>(transformation.GetCol(3));
  Math::Vector3 right = static_cast<Math::Vector3>(transformation.GetCol(0));
  Math::Vector3 up = static_cast<Math::Vector3>(transformation.GetCol(1));
  Math::Vector3 forward = static_cast<Math::Vector3>(transformation.GetCol(2));
  Line(start, start + right, xColor, thickness, duration, depthTest);
  Line(start, start + up, yColor, thickness, duration, depthTest);
  Line(start, start + forward, zColor, thickness, duration, depthTest);
#endif
}
void DebugDraw::AxisSphere(const Math::Matrix4& transformation,
                           const Color& xColor, const Color& yColor,
                           const Color& zColor, float thickness, float duration,
                           bool depthTest) {
#ifdef _EDITOR
  durationDraw.push_back(
      std::pair(duration, std::bind(DrawAxisSphere, transformation, xColor,
                                    yColor, zColor, thickness, depthTest)));
#endif
}
void DebugDraw::AxisSphere(const Math::Vector3& position, float radius,
                           const Color& xColor, const Color& yColor,
                           const Color& zColor, float thickness, float duration,
                           bool depthTest) {
#ifdef _EDITOR
  AxisSphere(Math::Matrix4::Transform(position, Math::Vector3::zero,
                                      radius * Math::Vector3::one),
             xColor, yColor, zColor, thickness, duration, depthTest);
#endif
}
void DebugDraw::OpenGLDraw(const Math::Matrix4& transformation,
                           const Color& color, float thickness, bool depthTest,
                           const Action<>& shape) {
  glUseProgram(shaderProgram);

  const Math::Matrix4 mvpMat =
      CameraComponent::Main()
          ->GetProperty<CameraComponent::Property::PROJECTION, Math::Matrix4>()
          .Transpose() *
      CameraComponent::Main()->transform->GetWorldToLocalMatrix() *
      transformation;
  glUniformMatrix4fv(modelViewProjectionLoc, 1, GL_FALSE,
                     mvpMat.Transpose().data);
  glUniform4fv(colorLoc, 1, color.rgba);
  GLError();

  if (thickness > 0) {
    glLineWidth(thickness);
  }

  if (depthTest) {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
  } else {
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
  }

  glBindVertexArray(VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  shape();

  glBindVertexArray(0);
}
void DebugDraw::GLError() {
  GLenum error;
  error = glGetError();
  if (error != GL_NO_ERROR) {
    LOG_INFO(Debug::Channel::Graphics, "GL_NO_ERROR");
  }
  if (error == GL_INVALID_OPERATION) {
    LOG_ERROR(Debug::Channel::Graphics, "GL_INVALID_OPERATION");
  }
  if (error == GL_INVALID_VALUE) {
    LOG_ERROR(Debug::Channel::Graphics, "GL_INVALID_VALUE");
  }
  if (error == GL_OUT_OF_MEMORY) {
    LOG_ERROR(Debug::Channel::Graphics, "GL_OUT_OF_MEMORY");
  }
  if (error == GL_STACK_OVERFLOW) {
    LOG_ERROR(Debug::Channel::Graphics, "GL_STACK_OVERFLOW");
  }
  if (error == GL_INVALID_FRAMEBUFFER_OPERATION) {
    LOG_ERROR(Debug::Channel::Graphics, "GL_INVALID_FRAMEBUFFER_OPERATION");
  }
  if (error == GL_OUT_OF_MEMORY) {
    LOG_ERROR(Debug::Channel::Graphics, "GL_OUT_OF_MEMORY");
  }
}

void DebugDraw::DrawPoint(const Math::Vector3 point, const Color& color,
                          float size, bool depthTest) {
  glUseProgram(shaderProgram);

  const Math::Matrix4 mvpMat =
      CameraComponent::Main()
          ->GetProperty<CameraComponent::Property::PROJECTION, Math::Matrix4>()
          .Transpose() *
      CameraComponent::Main()->transform->GetWorldToLocalMatrix();
  glUniformMatrix4fv(modelViewProjectionLoc, 1, GL_FALSE,
                     mvpMat.Transpose().data);
  glUniform4fv(colorLoc, 1, color.rgba);
  GLError();

  if (depthTest) {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
  } else {
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
  }

  glPointSize(size);
  glBindVertexArray(sVAO);

  lineVerticies[0] = point.x;
  lineVerticies[1] = point.y;
  lineVerticies[2] = point.z;
  glBindBuffer(GL_ARRAY_BUFFER, sVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(lineVerticies), lineVerticies,
               GL_DYNAMIC_DRAW);

  glDrawArrays(GL_POINTS, 0, 1);

  glBindVertexArray(0);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3, 0, GL_DYNAMIC_DRAW);
}
void DebugDraw::DrawLine(const Math::Vector3& start, const Math::Vector3& end,
                         const Color& color, float thickness, bool depthTest) {
  glUseProgram(shaderProgram);

  const Math::Matrix4 mvpMat =
      CameraComponent::Main()
          ->GetProperty<CameraComponent::Property::PROJECTION, Math::Matrix4>()
          .Transpose() *
      CameraComponent::Main()->transform->GetWorldToLocalMatrix();
  glUniformMatrix4fv(modelViewProjectionLoc, 1, GL_FALSE,
                     mvpMat.Transpose().data);
  glUniform4fv(colorLoc, 1, color.rgba);
  GLError();

  if (depthTest) {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
  } else {
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
  }

  glLineWidth(thickness);
  glBindVertexArray(sVAO);

  lineVerticies[0] = start.x;
  lineVerticies[1] = start.y;
  lineVerticies[2] = start.z;
  lineVerticies[3] = end.x;
  lineVerticies[4] = end.y;
  lineVerticies[5] = end.z;
  glBindBuffer(GL_ARRAY_BUFFER, sVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(lineVerticies), lineVerticies,
               GL_DYNAMIC_DRAW);

  glDrawArrays(GL_LINES, 0, 2);

  glBindVertexArray(0);
  glBufferData(GL_ARRAY_BUFFER, sizeof(lineVerticies), 0, GL_DYNAMIC_DRAW);
}
void DebugDraw::DrawSphere(const Math::Matrix4& transformation,
                           const Color& color, bool depthTest) {
  OpenGLDraw(transformation, color, 0, depthTest, []() { /* TODO(Jacob) */ });
}
void DebugDraw::DrawWireSphere(const Math::Matrix4& transformation,
                               const Color& color, float thickness,
                               bool depthTest) {
  float rx = transformation.Get(0, 0), ry = transformation.Get(1, 1),
        rz = transformation.Get(2, 2);
  const float rxSq = rx * rx;
  const float rySq = ry * ry;
  const float rzSq = rz * rz;
  float increment = 1.0f / SPHERE_SEGMENTS;
  for (int i = 1; i < SPHERE_SEGMENTS; i++) {
    float dist = i * increment;
    float distSq = dist * dist;
    rx = Math::Util::Sqrt(rxSq - distSq);
    ry = Math::Util::Sqrt(rySq - distSq);
    rz = Math::Util::Sqrt(rzSq - distSq);

    Math::Matrix4 xyScale = Math::Matrix4::Scale(Math::Vector3{rx, ry, 0}),
                  xzScale = Math::Matrix4::Scale(Math::Vector3{rx, 0, rz});

    Math::Matrix4 xzMat = xzScale * Math::Matrix4::xRot90;
    Math::Matrix4 xyTransF =
                      transformation *
                      Math::Matrix4::Translate(dist * Math::Vector3::forward) *
                      xyScale,
                  xyTransB =
                      transformation *
                      Math::Matrix4::Translate(dist * Math::Vector3::back) *
                      xyScale,
                  xzTransU =
                      transformation *
                      Math::Matrix4::Translate(dist * Math::Vector3::up) *
                      xzMat,
                  xzTransD =
                      transformation *
                      Math::Matrix4::Translate(dist * Math::Vector3::down) *
                      xzMat;
    OpenGLDraw(xyTransF, color, thickness, depthTest, []() {
      glDrawElements(GL_LINE_LOOP, CIRCLE_INDICIES, GL_UNSIGNED_INT,
                     (void*)(sizeof(int) * (PLANE_INDICIES + CUBE_INDICIES +
                                            CUBE_WIRE_INDICIES)));
    });
    OpenGLDraw(xyTransB, color, thickness, depthTest, []() {
      glDrawElements(GL_LINE_LOOP, CIRCLE_INDICIES, GL_UNSIGNED_INT,
                     (void*)(sizeof(int) * (PLANE_INDICIES + CUBE_INDICIES +
                                            CUBE_WIRE_INDICIES)));
    });
    OpenGLDraw(xzTransU, color, thickness, depthTest, []() {
      glDrawElements(GL_LINE_LOOP, CIRCLE_INDICIES, GL_UNSIGNED_INT,
                     (void*)(sizeof(int) * (PLANE_INDICIES + CUBE_INDICIES +
                                            CUBE_WIRE_INDICIES)));
    });
    OpenGLDraw(xzTransD, color, thickness, depthTest, []() {
      glDrawElements(GL_LINE_LOOP, CIRCLE_INDICIES, GL_UNSIGNED_INT,
                     (void*)(sizeof(int) * (PLANE_INDICIES + CUBE_INDICIES +
                                            CUBE_WIRE_INDICIES)));
    });
  }
  OpenGLDraw(transformation, color, thickness, depthTest, []() {
    glDrawElements(GL_LINE_LOOP, CIRCLE_INDICIES, GL_UNSIGNED_INT,
                   (void*)(sizeof(int) * (PLANE_INDICIES + CUBE_INDICIES +
                                          CUBE_WIRE_INDICIES)));
  });
  OpenGLDraw(transformation * Math::Matrix4::xRot90, color, thickness,
             depthTest, []() {
               glDrawElements(
                   GL_LINE_LOOP, CIRCLE_INDICIES, GL_UNSIGNED_INT,
                   (void*)(sizeof(int) * (PLANE_INDICIES + CUBE_INDICIES +
                                          CUBE_WIRE_INDICIES)));
             });
}
void DebugDraw::DrawWireCapsule(const Math::Matrix4& transformation,
                                float radius, float height, const Color& color,
                                float thickness, bool depthTest) {
  static const int halfCircle = 0.5f * CIRCLE_INDICIES;
  static const float squareScale = 0.5f * Math::Util::Sqrt(2);
  const float lineHeight = 0.5f * (height - 2 * radius);
  Math::Matrix4 up = Math::Matrix4::Translate(lineHeight * Math::Vector3::up);
  Math::Matrix4 down =
      Math::Matrix4::Translate(lineHeight * Math::Vector3::down);
  Math::Matrix4 scale = Math::Matrix4::Scale(Math::Vector3{radius});
  OpenGLDraw(transformation * up * scale, color, thickness, depthTest, []() {
    glDrawElements(GL_LINE_STRIP, halfCircle + 1, GL_UNSIGNED_INT,
                   (void*)(sizeof(int) * (PLANE_INDICIES + CUBE_INDICIES +
                                          CUBE_WIRE_INDICIES)));
  });
  OpenGLDraw(transformation * up * scale * Math::Matrix4::yRot90, color,
             thickness, depthTest, []() {
               glDrawElements(
                   GL_LINE_STRIP, halfCircle + 1, GL_UNSIGNED_INT,
                   (void*)(sizeof(int) * (PLANE_INDICIES + CUBE_INDICIES +
                                          CUBE_WIRE_INDICIES)));
             });
  OpenGLDraw(transformation * down * scale, color, thickness, depthTest, []() {
    glDrawElements(GL_LINE_STRIP, halfCircle + 1, GL_UNSIGNED_INT,
                   (void*)(sizeof(int) * (PLANE_INDICIES + CUBE_INDICIES +
                                          CUBE_WIRE_INDICIES + halfCircle)));
  });
  OpenGLDraw(transformation * down * scale * Math::Matrix4::yRot90, color,
             thickness, depthTest, []() {
               glDrawElements(
                   GL_LINE_STRIP, halfCircle + 1, GL_UNSIGNED_INT,
                   (void*)(sizeof(int) * (PLANE_INDICIES + CUBE_INDICIES +
                                          CUBE_WIRE_INDICIES + halfCircle)));
             });

  OpenGLDraw(transformation *
                 Math::Matrix4::Scale(
                     Math::Vector3{squareScale, 2 * lineHeight, squareScale}) *
                 Math::Matrix4::yRot45,
             color, thickness, depthTest,
             []() { glDrawArrays(GL_LINES, CUBE, 8); });

  OpenGLDraw(transformation * up * scale * Math::Matrix4::xRot90, color,
             thickness, depthTest, []() {
               glDrawElements(
                   GL_LINE_LOOP, CIRCLE_INDICIES, GL_UNSIGNED_INT,
                   (void*)(sizeof(int) * (PLANE_INDICIES + CUBE_INDICIES +
                                          CUBE_WIRE_INDICIES)));
             });
  OpenGLDraw(transformation * down * scale * Math::Matrix4::xRot90, color,
             thickness, depthTest, []() {
               glDrawElements(
                   GL_LINE_LOOP, CIRCLE_INDICIES, GL_UNSIGNED_INT,
                   (void*)(sizeof(int) * (PLANE_INDICIES + CUBE_INDICIES +
                                          CUBE_WIRE_INDICIES)));
             });
}
void DebugDraw::DrawGrid(const Math::Matrix4& transformation, int lines,
                         const Color& color, float thickness) {
  glUseProgram(shaderProgram);

  const Math::Matrix4 mvpMat =
      CameraComponent::Main()
          ->GetProperty<CameraComponent::Property::PROJECTION, Math::Matrix4>()
          .Transpose() *
      CameraComponent::Main()->transform->GetWorldToLocalMatrix() *
      transformation;
  glUniformMatrix4fv(modelViewProjectionLoc, 1, GL_FALSE,
                     mvpMat.Transpose().data);
  glUniform4fv(colorLoc, 1, color.rgba);
  GLError();

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.0f, 1.0f);

  glLineWidth(thickness);
  glBindVertexArray(sVAO);

  for (int i = -lines; i <= lines; ++i) {
    lineVerticies[0] = -lines;
    lineVerticies[1] = 0;
    lineVerticies[2] = i;
    lineVerticies[3] = lines;
    lineVerticies[4] = 0;
    lineVerticies[5] = i;
    glBindBuffer(GL_ARRAY_BUFFER, sVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVerticies), lineVerticies,
                 GL_DYNAMIC_DRAW);

    glDrawArrays(GL_LINES, 0, 2);
  }

  for (int i = -lines; i <= lines; ++i) {
    lineVerticies[0] = i;
    lineVerticies[1] = 0;
    lineVerticies[2] = -lines;
    lineVerticies[3] = i;
    lineVerticies[4] = 0;
    lineVerticies[5] = lines;
    glBindBuffer(GL_ARRAY_BUFFER, sVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVerticies), lineVerticies,
                 GL_DYNAMIC_DRAW);

    glDrawArrays(GL_LINES, 0, 2);
  }

  glBindVertexArray(0);
}
void DebugDraw::DrawAxisSphere(const Math::Matrix4& transformation,
                               const Color& xColor, const Color& yColor,
                               const Color& zColor, float thickness,
                               bool depthTest) {
  OpenGLDraw(transformation, zColor, thickness, depthTest, []() {
    glDrawElements(GL_LINE_LOOP, CIRCLE_INDICIES, GL_UNSIGNED_INT,
                   (void*)(sizeof(int) * (PLANE_INDICIES + CUBE_INDICIES +
                                          CUBE_WIRE_INDICIES)));
  });
  OpenGLDraw(transformation * Math::Matrix4::xRot90, yColor, thickness,
             depthTest, []() {
               glDrawElements(
                   GL_LINE_LOOP, CIRCLE_INDICIES, GL_UNSIGNED_INT,
                   (void*)(sizeof(int) * (PLANE_INDICIES + CUBE_INDICIES +
                                          CUBE_WIRE_INDICIES)));
             });
  OpenGLDraw(transformation * Math::Matrix4::yRot90, xColor, thickness,
             depthTest, []() {
               glDrawElements(
                   GL_LINE_LOOP, CIRCLE_INDICIES, GL_UNSIGNED_INT,
                   (void*)(sizeof(int) * (PLANE_INDICIES + CUBE_INDICIES +
                                          CUBE_WIRE_INDICIES)));
             });
}

#undef PLANE
#undef PLANE_VERTICIES
#undef PLANE_INDICIES
#undef CUBE
#undef CUBE_VERTICIES
#undef CUBE_INDICIES
#undef CUBE_WIRE_INDICIES
#undef CUBE_VERTICIES
#undef CIRCLE
#undef CIRCLE_INDICIES
#undef CIRCLE_VERTICIES
#undef SPHERE_SEGMENTS

#undef RIGHT
#undef UP
#undef FORWARD
}  // namespace Isetta
