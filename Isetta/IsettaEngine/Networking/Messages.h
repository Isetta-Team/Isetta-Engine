/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <unordered_map>
#include <utility>

#include "Core/Debug/Assert.h"
#include "Networking/NetworkManager.h"
#include "yojimbo/yojimbo.h"

namespace Isetta {

class NetworkMessageFactory : public yojimbo::MessageFactory {
 public:
  NetworkMessageFactory(yojimbo::Allocator* allocator, int num_message_types)
      : MessageFactory(allocator, num_message_types) {}

  yojimbo::Message* CreateMessageInternal(int type) {
    yojimbo::Message* message;
    yojimbo::Allocator& allocator = GetAllocator();
    message = NetworkManager::Instance().factories[type].second(
        allocator.Allocate(NetworkManager::Instance().factories[type].first,
                           __FILE__, __LINE__));
    if (!message) return NULL;
    SetMessageType(message, type);
    return message;
  }
};

template <typename T>
class NetworkMessageRegistry {
 protected:
  static bool registered;
};

template <typename T>
bool NetworkMessageRegistry<T>::registered =
    NetworkManager::Instance().RegisterMessageType<T>(sizeof(T), T::Create);

#define DEFINE_NETWORK_MESSAGE(NAME)                                             \
  class NAME : public yojimbo::Message,                                      \
               public Isetta::NetworkMessageRegistry<NAME> {                 \
   public:                                                                   \
    bool IsRegisteredInNetworkManager() const { return registered; }         \
    static inline NAME* Create(void* memory) { return new (memory) NAME(); } \
    static std::string GetMessageName() { return #NAME; }

#define DEFINE_NETWORK_MESSAGE_END               \
 public:                                 \
  YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS(); \
  }                                      \
  ;

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
    nextAvailable = reinterpret_cast<Size>(memPointer);
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
                              NetworkManager::Instance().GetMessageTypeCount());
  }
};
}  // namespace Isetta
