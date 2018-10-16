/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <unordered_map>
#include <utility>

#include "Core/Debug/Assert.h"
#include "Core/Memory/MemoryManager.h"
#include "Networking/NetworkIdentity.h"
#include "Util.h"
#include "yojimbo/yojimbo.h"

namespace Isetta {

#define NETWORK_TAG_LEN 5

class NetworkRegistry {
 public:
  static U16 GetMessageTypeCount() { return messageTypeCount; }
  static void RegisterMessageType(U64 size,
                                  Func<yojimbo::Message*, void*> factory,
                                  const char tag[NETWORK_TAG_LEN]) {
    factories[messageTypeCount] = std::pair(size, factory);
    tags[tag] = messageTypeCount;
    ++messageTypeCount;
  }
  static int RegisterServerCallback(const char tag[NETWORK_TAG_LEN],
                                    Action<int, yojimbo::Message*> func) {
    serverCallbacks[GetMessageTypeId(tag)].push_back(
        std::pair(functionCount, func));
    return functionCount++;
  }
  static void UnregisterServerCallback(const char tag[NETWORK_TAG_LEN],
                                       int handle) {
    int messageId = GetMessageTypeId(tag);
    serverCallbacks[messageId].remove_if(
        [handle](std::pair<U16, Action<U16, yojimbo::Message*>> item) {
          return item.first == handle;
        });
  }
  static int RegisterClientCallback(const char tag[NETWORK_TAG_LEN],
                                    Action<yojimbo::Message*> func) {
    clientCallbacks[GetMessageTypeId(tag)].push_back(
        std::pair(functionCount, func));
    return functionCount++;
  }
  static void UnregisterClientCallback(const char tag[NETWORK_TAG_LEN],
                                       int handle) {
    int messageId = GetMessageTypeId(tag);
    clientCallbacks[messageId].remove_if(
        [handle](std::pair<U16, Action<yojimbo::Message*>> item) {
          return item.first == handle;
        });
  }

  static Entity* GetNetworkEntity(const U32 id) {
    auto it = networkIdToEntityMap.find(id);
    if (it != networkIdToEntityMap.end()) {
      return it->second;
    }
    return NULL;
  }
  static U32 CreateNetworkId(NetworkIdentity networkIdentity) {
    /*if (!NetworkManager::ServerIsRunning()) {
      throw std::exception("Cannot create a new network id on a client");
    }*/
    U32 netId = nextNetworkId++;
    networkIdentity.id = netId;
    networkIdToEntityMap[netId] = networkIdentity.owner;
  }
  static U32 AssignNetworkId(U32 netId, NetworkIdentity networkIdentity) {
    if (networkIdToEntityMap.find(netId) != networkIdToEntityMap.end()) {
      throw std::exception(Util::StrFormat(
          "Multiple objects trying to assign to the same network id: %d",
          netId));
    } else if (networkIdentity.id > 0) {
      throw std::exception(
          Util::StrFormat("Trying to assign network id %d to existing network "
                          "object with id %d",
                          netId, networkIdentity.id));
    }
    networkIdentity.id = netId;
    networkIdToEntityMap[netId] = networkIdentity.owner;
  }
  static void RemoveNetworkId(NetworkIdentity networkIdentity) {
    if (!networkIdentity.id) {
      throw std::exception(Util::StrFormat(
          "Cannot remove network id on a nonexistent network object"));
    }
    networkIdToEntityMap.erase(networkIdentity.id);
    networkIdentity.id = NULL;
  }

 private:
  static int GetMessageTypeId(const char tag[NETWORK_TAG_LEN]) {
    return tags[tag];
  }
  static std::list<std::pair<U16, Action<yojimbo::Message*>>>
  GetClientFunctions(int type) {
    return clientCallbacks[type];
  }
  static std::list<std::pair<U16, Action<int, yojimbo::Message*>>>
  GetServerFunctions(int type) {
    return serverCallbacks[type];
  }

  static int messageTypeCount;
  static U16 functionCount;
  static U32 nextNetworkId;
  static std::unordered_map<int, std::pair<U64, Func<yojimbo::Message*, void*>>>
      factories;
  static std::unordered_map<const char*, int> tags;

  static std::unordered_map<
      int, std::list<std::pair<U16, Action<yojimbo::Message*>>>>
      clientCallbacks;
  static std::unordered_map<
      int, std::list<std::pair<U16, Action<int, yojimbo::Message*>>>>
      serverCallbacks;

  static std::unordered_map<U32, Entity*> networkIdToEntityMap;

  NetworkRegistry() = default;

  friend class NetworkingModule;
  friend class NetworkManager;
  friend class NetworkMessageFactory;
  friend class NetworkIdentity;
};

class NetworkMessageFactory : public yojimbo::MessageFactory {
 public:
  NetworkMessageFactory(yojimbo::Allocator* allocator, int num_message_types)
      : MessageFactory(allocator, num_message_types) {}

  yojimbo::Message* CreateMessageInternal(int type) {
    yojimbo::Message* message;
    yojimbo::Allocator& allocator = GetAllocator();
    message = NetworkRegistry::factories[type].second(allocator.Allocate(
        NetworkRegistry::factories[type].first, __FILE__, __LINE__));
    if (!message) return NULL;
    SetMessageType(message, type);
    return message;
  }
};

class IdMessage : public yojimbo::Message {};

#define RPC_MESSAGE_DEFINE(MessageClass)         \
  class MessageClass : public yojimbo::Message { \
   public:                                       \
    static MessageClass* Create(void* memory) {  \
      return new (memory) MessageClass();        \
    }

#define RPC_MESSAGE_FINISH               \
 public:                                 \
  YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS(); \
  }                                      \
  ;

#define RPC_MESSAGE_INIT(MessageClass, Tag)                  \
  NetworkRegistry::RegisterMessageType(sizeof(MessageClass), \
                                       MessageClass::Create, Tag);

class NetworkAllocator : public yojimbo::Allocator {
 public:
  // Network allocation is currently assumed to be LSR
  NetworkAllocator(void* memory, Size size) {
    ASSERT(size > 0);

    // SetErrorLevel(yojimbo::ALLOCATOR_ERROR_NONE);

    memPointer = memory;
    nextAvailable = reinterpret_cast<Size>(memPointer);
    endAddress = size + nextAvailable;
  }

  // TODO(Caleb): Clean up this hacky copy constructor
  NetworkAllocator(const NetworkAllocator& a) {
    memPointer = a.memPointer;
    nextAvailable = a.nextAvailable;
  }

  void* Allocate(Size size, const char* file, int line) {
    void* p = reinterpret_cast<void*>(nextAvailable);

    if (nextAvailable + size > endAddress) {
      // SetErrorLevel(yojimbo::ALLOCATOR_ERROR_OUT_OF_MEMORY);
      throw std::exception("Bad memory!");  // TODO(Caleb) better exception
    }

    // TrackAlloc(p, size, file, line);  // This causes a 64 byte memory leak

    nextAvailable += size;

    return p;
  }

  void Free(void* p, const char* file, int line) {
    if (!p) {
      return;
    }

    // TrackFree(p, file, line);  // This causes a 64 byte memory leak

    // Do nothing I guess? This is only supposed to be an LSR allocator
  }

 private:
  void* memPointer;
  Size nextAvailable;
  Size endAddress;
};

/**
 * @brief Container class for several utilities used by the networking module.
 *
 */
class CustomAdapter : public yojimbo::Adapter {
 public:
  // TODO(Caleb): Change the CreateAllocator function to use our mem alloc
  // instead of TLSF
  // Actually, TLSF might be good if it all remains in LSR

  // TODO(Caleb): something about the Linter with a const ref
  /**
   * @brief Creates a MessageFactory object that can be used to generate Message
   * objects specified by the IsettaMessageType enum.
   *
   * @param allocator Memory allocator used to allocate memory for the created
   * Message objects.
   * @return yojimbo::MessageFactory*
   */
  yojimbo::MessageFactory* CreateMessageFactory(yojimbo::Allocator* allocator) {
    return new (
        allocator->Allocate(sizeof(NetworkMessageFactory), __FILE__, __LINE__))
        NetworkMessageFactory(allocator,
                              NetworkRegistry::GetMessageTypeCount());
  }
};
}  // namespace Isetta
