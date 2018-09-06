#include "Core\Graphics\RenderNode.h"

#include <Horde3DUtils.h>
#include <stdexcept>
#include "Core\Math\Vector3.h"

namespace Isetta {
void RenderNode::SetTransform(const Isetta::Math::Vector3& position,
                              const Isetta::Math::Vector3& rotation,
                              const Isetta::Math::Vector3& scale) {
  h3dSetNodeTransform(renderResource, position.x, position.y, position.z,
                      rotation.x, rotation.y, rotation.z, scale.x, scale.y,
                      scale.z);
}


}  // namespace Isetta