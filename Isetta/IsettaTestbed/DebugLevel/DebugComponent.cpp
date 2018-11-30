/*
 * Copyright (c) 2018 Isetta
 */
#include "DebugComponent.h"

using namespace Isetta;

void DebugComponent::Update() {
  DebugDraw::Point(2 * Math::Vector3::left, Color::magenta, 20);
  DebugDraw::Line(Math::Vector3::zero, 3 * Math::Vector3::one);
  // Rotating ray in a circle
  if (Input::IsKeyPressed(KeyCode::V)) {
    static float angle = 0.0f;
    angle += 0.4f * EngineLoop::GetGameClock().GetDeltaTime();
    if (angle >= 2 * Math::Util::PI) {
      angle = 0;
    }
    DebugDraw::Ray(
        Math::Vector3::zero,
        Math::Vector3{Math::Util::Cos(angle), 0, Math::Util::Sin(angle)},
        Color::cyan, 2);
  }
  // Display plane
  if (Input::IsKeyPressed(KeyCode::B)) {
    DebugDraw::Plane(Math::Matrix4::identity, Color::blue, 2);
  }
  DebugDraw::WirePlane(Math::Matrix4::identity);
  DebugDraw::Cube(Math::Matrix4::Translate(Math::Vector3{2.8, 1.1, 0}) *
                      Math::Matrix4::Scale(2.2 * Math::Vector3::one),
                  Color::brown);
  DebugDraw::WireCube(Math::Matrix4::Translate(Math::Vector3{0, 0, -2}));
  DebugDraw::WireSphere(Math::Vector3::up, 1, Color::red);
  DebugDraw::WireCapsule(Math::Matrix4::Translate(Math::Vector3{-1, 4, 1}), 0.5,
                         2, Color::blue);
  DebugDraw::AxisSphere(Math::Vector3::up, 1);
  DebugDraw::Axis();
  DebugDraw::Grid();
}