/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/RenderNode.h"

#include <Horde3DUtils.h>
#include <stdexcept>
#include "Core/Math/Matrix4.h"
#include "Core/Math/Vector3.h"

namespace Isetta {
void RenderNode::SetHordeTransform(const Isetta::Math::Vector3& position,
                                   const Isetta::Math::Vector3& rotation,
                                   const Isetta::Math::Vector3& scale) {
  h3dSetNodeTransform(renderNode, position.x, position.y, position.z,
                      rotation.x, rotation.y, rotation.z, scale.x, scale.y,
                      scale.z);
}

}  // namespace Isetta
