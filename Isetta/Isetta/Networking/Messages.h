/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>
#include "yojimbo/yojimbo.h"

namespace Isetta {
struct HandleMessage : public yojimbo::Message {
  int handle;

  HandleMessage() { handle = 0; }

  // TODO(Caleb): choose a more reasonable range for the int serialization
  template <typename Stream>
  bool Serialize(Stream& stream) {
    serialize_int(stream, handle, 0, 64);

    return true;
  }

  YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
};

struct StringMessage : public yojimbo::Message {
  std::string string;

  StringMessage() { string = ""; }

  // TODO(Caleb): choose a more reasonable range for the int serialization
  template <typename Stream>
  bool Serialize(Stream& stream) {
    serialize_string(stream, (char*)string.c_str(), 512);

    return true;
  }

  YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
};

enum IsettaMessageType { HANDLE_MESSAGE, STRING_MESSAGE, NUM_MESSAGE_TYPES };

YOJIMBO_MESSAGE_FACTORY_START(IsettaMessageFactory, NUM_MESSAGE_TYPES);
YOJIMBO_DECLARE_MESSAGE_TYPE(HANDLE_MESSAGE, HandleMessage);
YOJIMBO_DECLARE_MESSAGE_TYPE(STRING_MESSAGE, StringMessage);
YOJIMBO_MESSAGE_FACTORY_FINISH();

class CustomAdapter : public yojimbo::Adapter {
 public:
  // TODO(Caleb): Change the CreateAllocator function to use our mem alloc
  // instead of TLSF

  // TODO(Caleb): something about the Linter with a const ref
  yojimbo::MessageFactory* CreateMessageFactory(yojimbo::Allocator* allocator) {
    return YOJIMBO_NEW(*allocator, IsettaMessageFactory, *allocator);
  }
};
}  // namespace Isetta
