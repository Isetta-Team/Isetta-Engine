/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/MeshComponent.h"

#include "Core/Config/Config.h"
#include "Core/Debug/Assert.h"
#include "Scene/Transform.h"
#include "Util.h"
#include "brofiler/ProfilerCore/Brofiler.h"

namespace Isetta {
RenderModule* MeshComponent::renderModule{nullptr};

MeshComponent::MeshComponent(std::string_view resourceName) {
  ASSERT(renderModule != nullptr);
  renderModule->meshComponents.push_back(this);
  renderResource = LoadResourceFromFile(resourceName);
}

MeshComponent::~MeshComponent() {
  ASSERT(renderModule != nullptr);
  renderModule->meshComponents.remove(this);
}

void MeshComponent::UpdateTransform() const {
  PROFILE
  Transform::SetH3DNodeTransform(renderNode, *transform);
}

H3DRes MeshComponent::LoadResourceFromFile(std::string_view resourceName) {
  H3DRes renderResource =
      h3dAddResource(H3DResTypes::SceneGraph, resourceName.data(), 0);

  RenderModule::LoadResourceFromDisk(
      renderResource, Util::StrFormat("MeshComponent::LoadResourceFromFile => "
                                      "Cannot load the resource from %s",
                                      resourceName.data()));

  return renderResource;
}

void MeshComponent::OnEnable() {
  if (renderNode == 0) {
    renderNode = h3dAddNodes(H3DRootNode, renderResource);
    int count = h3dFindNodes(renderNode, "", H3DNodeTypes::Joint);
    for (int i = 0; i < count; ++i) {
      H3DNode node = h3dGetNodeFindResult(i);
      const char* name = h3dGetNodeParamStr(node, H3DNodeParams::NameStr);
      joints.insert({SID(name), node});
    }
  } else {
    h3dSetNodeFlags(renderNode, 0, true);
  }
}

void MeshComponent::OnDisable() {
  h3dSetNodeFlags(renderNode, H3DNodeFlags::Inactive, true);
}

void MeshComponent::OnDestroy() { h3dRemoveNode(renderNode); }

std::tuple<Math::Vector3, Math::Quaternion>
MeshComponent::GetJointWorldTransform(std::string jointName) {
  if (joints.find(SID(jointName.c_str())) == joints.end()) {
    throw std::logic_error(
        "MeshComponent::GetJointWorldTransform => No such joint in this mesh");
  }
  H3DNode jointNode = joints.at(SID(jointName.c_str()));
  const float* data[4];
  const float** testPtr = &data[0];
  Math::Matrix4 abs;
  h3dGetNodeTransMats(jointNode, nullptr, testPtr);

  memcpy(abs.data, *testPtr, 16 * sizeof(float));

  abs = abs.Transpose();
  Math::Vector3 pos = abs.GetCol(3).GetVector3();
  Math::Quaternion q = Math::Quaternion::FromLookRotation(
      abs.GetCol(0).GetVector3(), abs.GetCol(1).GetVector3());
  return {pos, q};
}

}  // namespace Isetta
