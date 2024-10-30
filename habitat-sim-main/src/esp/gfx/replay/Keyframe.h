// Copyright (c) Meta Platforms, Inc. and its affiliates.
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef ESP_GFX_REPLAY_KEYFRAME_H_
#define ESP_GFX_REPLAY_KEYFRAME_H_

#include "esp/assets/Asset.h"
#include "esp/assets/RenderAssetInstanceCreationInfo.h"

namespace esp {
namespace gfx {
namespace replay {

using RenderAssetInstanceKey = int32_t;

/**
 * @brief serialization/replay-friendly Transform class
 */
struct Transform {
  Magnum::Vector3 translation;
  Magnum::Quaternion rotation;
  bool operator==(const Transform& rhs) const {
    return translation == rhs.translation && rotation == rhs.rotation;
  }
};

/**
 * @brief The dynamic state of a render instance that is tracked by the replay
 * system every frame.
 */
struct RenderAssetInstanceState {
  Transform absTransform;  // localToWorld
  // note we don't currently support runtime changes to lightSetupKey
  bool operator==(const RenderAssetInstanceState& rhs) const {
    return absTransform == rhs.absTransform;
  }
};

/**
 * @brief Metadata associated with an instance.
 */
struct InstanceMetadata {
  int objectId = ID_UNDEFINED;
  int semanticId = ID_UNDEFINED;

  bool operator==(const InstanceMetadata& rhs) const {
    return objectId == rhs.objectId && semanticId == rhs.semanticId;
  }
};

/**
 * @brief Definition of a rigged articulated object.
 */
struct RigCreation {
  int id = ID_UNDEFINED;
  std::vector<std::string> boneNames;
};

/**
 * @brief The dynamic state of a rigged articulated object that is tracked by
 * the replay system every frame.
 */
struct RigUpdate {
  int id = ID_UNDEFINED;
  std::vector<Transform> pose;  // localToWorld
};

/**
 * @brief A serialization/replay-friendly render keyframe class. See @ref
 * Recorder.
 */
struct Keyframe {
  std::vector<esp::assets::AssetInfo> loads;
  std::vector<RigCreation> rigCreations;
  std::vector<std::pair<RenderAssetInstanceKey,
                        esp::assets::RenderAssetInstanceCreationInfo>>
      creations;
  std::vector<RenderAssetInstanceKey> deletions;
  std::vector<std::pair<RenderAssetInstanceKey, InstanceMetadata>> metadata;
  std::vector<std::pair<RenderAssetInstanceKey, RenderAssetInstanceState>>
      stateUpdates;
  std::vector<RigUpdate> rigUpdates;
  std::unordered_map<std::string, Transform> userTransforms;
  std::vector<LightInfo> lights;
  bool lightsChanged = false;

  ESP_SMART_POINTERS(Keyframe)
};

}  // namespace replay
}  // namespace gfx
}  // namespace esp

#endif
