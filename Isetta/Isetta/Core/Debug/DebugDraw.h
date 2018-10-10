/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "Core/Color.h"
#include "Core/IsettaAlias.h"
#include "Core/Math/Matrix4.h"

namespace Isetta {
class Color;
namespace Math {
class Vector3;
// class Matrix4;
}  // namespace Math
}  // namespace Isetta

namespace Isetta {
/**
 * @brief static class for drawing on the screen and other debug drawing
 * features
 *
 */
class DebugDraw {
 public:
  static void StartUp();
  static void Update();
  static void ShutDown();

  static void Point(const Math::Vector3 point,
                    const Color& color = Color::white, float size = 1.0f,
                    float duration = 0.0f, bool depthTest = true);
  static void Line(const Math::Vector3& start, const Math::Vector3& end,
                   const Color& color = Color::white, float thickness = 1.0f,
                   float duration = 0.0f, bool depthTest = true);
  static void Ray(const Math::Vector3& start, const Math::Vector3& dir,
                  const Color& color = Color::white, float thickness = 1.0f,
                  float duration = 0.0f, bool depthTest = true);
  static void Plane(
      const Math::Matrix4& transformation = Math::Matrix4::identity,
      const Color& color = Color::white, float duration = 0.0f,
      bool depthTest = true);
  static void WirePlane(
      const Math::Matrix4& transformation = Math::Matrix4::identity,
      const Color& color = Color::white, float thickness = 1.0f,
      float duration = 0.0f, bool depthTest = true);
  static void Cube(
      const Math::Matrix4& transformation = Math::Matrix4::identity,
      const Color& color = Color::white, float duration = 0.0f,
      bool depthTest = true);
  static void WireCube(
      const Math::Matrix4& transformation = Math::Matrix4::identity,
      const Color& color = Color::white, float thickness = 1.0f,
      float duration = 0.0f, bool depthTest = true);
  static void Sphere(const Math::Matrix4& transformation,
                     const Color& color = Color::white, float duration = 0,
                     bool depthTest = true);
  static void Sphere(const Math::Vector3& position, float radius,
                     const Color& color = Color::white, float duration = 0,
                     bool depthTest = true);
  static void WireSphere(const Math::Matrix4& transformation,
                         const Color& color = Color::white,
                         float thickness = 1.0f, float duration = 0,
                         bool depthTest = true);
  static void WireSphere(const Math::Vector3& position, float radius,
                         const Color& color = Color::white,
                         float thickness = 1.0f, float duration = 0,
                         bool depthTest = true);
  static void Grid(const Math::Matrix4& transformation = Math::Matrix4::zero,
                   const Color& color = Color::lightGrey,
                   float thickness = 1.0f);
  static void Axis(
      const Math::Matrix4& transformation = Math::Matrix4::identity,
      const Color& xColor = Color::red, const Color& yColor = Color::yellow,
      const Color& zColor = Color::blue, float thickness = 2.0,
      float duration = 0, bool depthTest = true);
  static void AxisSphere(
      const Math::Matrix4& transformation = Math::Matrix4::identity,
      const Color& xColor = Color::red, const Color& yColor = Color::yellow,
      const Color& zColor = Color::blue, float thickness = 2.0,
      float duration = 0, bool depthTest = true);
  static void AxisSphere(const Math::Vector3& position, float radius,
                         const Color& xColor = Color::red,
                         const Color& yColor = Color::yellow,
                         const Color& zColor = Color::blue,
                         float thickness = 1.0, float duration = 0,
                         bool depthTest = true);

 private:
  static const float verticies[];
  static const int indices[];
  static float lineVerticies[];

  static const char *vertexShaderSource, *fragmentShaderSource;

  static int projectionLoc, viewLoc, modelLoc, colorLoc;
  static unsigned int VBO, VAO, EBO, sVBO, sVAO;
  static int shaderProgram;

  static void OpenGLDraw(const Math::Matrix4& transformation,
                         const Color& color, float thickness, bool depthTest,
                         const Action<>& shape);
  static void GLError();

  static std::list<std::pair<float, Action<>>> durationDraw;
};
}  // namespace Isetta
