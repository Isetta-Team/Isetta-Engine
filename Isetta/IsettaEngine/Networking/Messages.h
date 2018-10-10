/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>
#include "Core/Debug/Assert.h"
#include "Core/Memory/MemoryManager.h"
#include "yojimbo/yojimbo.h"

namespace Isetta {

class RPCRegistry {
 private:
  static void RegisterMessageType(const int size,
                                  Func<yojimbo::Message*, void*> f) {
    sizes[count] = size;
    factories[count] = f;
    ++count;
  }

  static int count;
  static std::unordered_map<int, int> sizes;
  static std::unordered_map<int, Func<yojimbo::Message*, void*>> factories;

  RPCRegistry() = default;

  friend class RPCRegistryHelper;
  friend class RPCMessageFactory;
};

class RPCRegistryHelper {
 public:
  RPCRegistryHelper(const int size, Func<yojimbo::Message*, void*> f) {
    RPCRegistry::RegisterMessageType(size, f);
  }
};

class RPCMessageFactory : public yojimbo::MessageFactory {
 public:
  RPCMessageFactory(yojimbo::Allocator& allocator, int num_message_types)
      : MessageFactory(allocator, num_message_types){};

  yojimbo::Message* CreateMessageInternal(int type) {
    yojimbo::Message* message;
    yojimbo::Allocator& allocator = GetAllocator();
    message = RPCRegistry::factories[type](
        allocator.Allocate(RPCRegistry::sizes[type], __FILE__, __LINE__));
    if (!message) return NULL;
    SetMessageType(message, type);
    return message;
  }
};

#define RPC_START(MessageClass)                  \
  class MessageClass : public yojimbo::Message { \
   private:                                      \
    static MessageClass* Create(void* memory) {  \
      return new (memory) MessageClass();        \
    }                                            \
    class RPCMessageFactory;                     \
    friend class RPCMessageFactory;              \
    static RPCRegistryHelper MessageClass##_RPCRegistryHelper;

#define RPC_FINISH(MessageClass)         \
 public:                                 \
  YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS(); \
  }                                      \
  ;

/**
 * @brief Code-generated struct to be used for sending integer values across the
 * network.
 *
 */
RPC_START(HandleMessage)
HandleMessage() { handle = 0; }

// TODO(Caleb): choose a more reasonable range for the int serialization
template <typename Stream>
bool Serialize(Stream& stream) {
  serialize_int(stream, handle, 0, 64);

  return true;
}

public:
int handle;
RPC_FINISH(HandleMessage)

/**
 * @brief Code-generated struct to be used for sending string messages across
 * the network.
 *
 */
RPC_START(StringMessage)

StringMessage() { string = ""; }

// TODO(Caleb): choose a more reasonable range for the int serialization
template <typename Stream>
bool Serialize(Stream& stream) {
  serialize_string(stream, const_cast<char*>(string.c_str()), 512);

  return true;
}

 public:
  std::string string;
RPC_FINISH(StringMessage)

/**
 * @brief Enumeration of the message types that can be created by the
 * IsettaMessageFactory class.
 *
 */
enum IsettaMessageType { HANDLE_MESSAGE, STRING_MESSAGE, NUM_MESSAGE_TYPES };

/// Code-generate the IsettaMessageFactory class, which creates Message objects
/// upon request that can be sent over the network.
// YOJIMBO_MESSAGE_FACTORY_START(IsettaMessageFactory, NUM_MESSAGE_TYPES);
// YOJIMBO_DECLARE_MESSAGE_TYPE(HANDLE_MESSAGE, HandleMessage);
// YOJIMBO_DECLARE_MESSAGE_TYPE(STRING_MESSAGE, StringMessage);
// YOJIMBO_MESSAGE_FACTORY_FINISH();

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
    // return YOJIMBO_NEW(*allocator, IsettaMessageFactory, *allocator);
    return new (allocator->Allocate(sizeof(RPCMessageFactory), __FILE__,
                                    __LINE__)) RPCMessageFactory(*allocator, 2);
  }
};
}  // namespace Isetta
