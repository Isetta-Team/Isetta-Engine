/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>
#include "Core/Debug/Assert.h"
#include "Core/Memory/MemoryManager.h"
#include "yojimbo/yojimbo.h"

namespace Isetta {

class NetworkRegistry {
 public:
  static int GetCount() { return count; }
  static int GetId(const char tag[5]) { return tags[tag]; }
  static Action<int, yojimbo::Server*, yojimbo::Message*> ServerFunc(int type) {
    return serverFuncs[type];
  }
  static Action<yojimbo::Client*, yojimbo::Message*> ClientFunc(int type) {
    return clientFuncs[type];
  }

 private:
  static void RegisterMessageType(
      unsigned long long size, Func<yojimbo::Message*, void*> factory,
      const char tag[5], Action<yojimbo::Client*, yojimbo::Message*> clientFunc,
      Action<int, yojimbo::Server*, yojimbo::Message*> serverFunc) {
    factories[count] = std::pair(size, factory);
    tags[tag] = count;
    clientFuncs[count] = clientFunc;
    serverFuncs[count] = serverFunc;
    ++count;
  }

  static int count;
  static std::unordered_map<
      int, std::pair<unsigned long long, Func<yojimbo::Message*, void*>>>
      factories;
  static std::unordered_map<const char*, int> tags;
  static std::unordered_map<int, Action<yojimbo::Client*, yojimbo::Message*>>
      clientFuncs;
  static std::unordered_map<int,
                            Action<int, yojimbo::Server*, yojimbo::Message*>>
      serverFuncs;

  NetworkRegistry() = default;

  friend class NetworkRegistryHelper;
  friend class NetworkMessageFactory;
};

class NetworkRegistryHelper {
 public:
  NetworkRegistryHelper(
      unsigned long long size, Func<yojimbo::Message*, void*> factory,
      const char tag[5], Action<yojimbo::Client*, yojimbo::Message*> clientFunc,
      Action<int, yojimbo::Server*, yojimbo::Message*> serverFunc) {
    NetworkRegistry::RegisterMessageType(size, factory, tag, clientFunc,
                                         serverFunc);
  }
};

class NetworkMessageFactory : public yojimbo::MessageFactory {
 public:
  NetworkMessageFactory(yojimbo::Allocator& allocator, int num_message_types)
      : MessageFactory(allocator, num_message_types){};

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

#define RPC_MESSAGE_DEFINE(MessageClass)         \
  class MessageClass : public yojimbo::Message { \
   private:                                      \
    static MessageClass* Create(void* memory) {  \
      return new (memory) MessageClass();        \
    }                                            \
    class NetworkMessageFactory;                 \
    friend class NetworkMessageFactory;          \
    static NetworkRegistryHelper MessageClass##_NetworkRegistryHelper;

#define RPC_CLIENT_FUNC \
 public:                \
  static void RpcClient(yojimbo::Client* client, yojimbo::Message* message)
#define RPC_SERVER_FUNC                                         \
 public:                                                        \
  static void RpcServer(int clientIdx, yojimbo::Server* server, \
                        yojimbo::Message* message)
#define RPC_MESSAGE_FINISH(MessageClass, Tag)                               \
 public:                                                                    \
  YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();                                    \
  }                                                                         \
  ;                                                                         \
  NetworkRegistryHelper MessageClass::MessageClass##_NetworkRegistryHelper( \
      sizeof(MessageClass), MessageClass::Create, Tag,                      \
      MessageClass::RpcClient, MessageClass::RpcServer);

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
        NetworkMessageFactory(*allocator, NetworkRegistry::GetCount());
  }
};
}  // namespace Isetta
