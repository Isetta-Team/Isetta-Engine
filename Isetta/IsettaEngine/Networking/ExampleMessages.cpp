/*
 * Copyright (c) 2018 Isetta
 */

#include "ExampleMessages.h"

namespace Isetta {

void InitExampleMessages() {
  RPC_MESSAGE_INIT(HandleMessage, "HNDL");
  RPC_MESSAGE_INIT(StringMessage, "STRN");
}

}