/*
 * Copyright (c) 2018 Isetta
 */
#include <afx.h>
#include <iostream>
#include "SID/sid.h"

int main() {
  // Example copied from https://github.com/TheAllenChou/string-id.git
  /*
The SID macro evaluates to a StringId object.
The SID_VAL macro evaluates to a StringId's underlying raw integer data.
These macros evaluate to constants at compiled-time if given compile-time
constant strings. StringId::GerValue() returns a StringId's underlying raw
integer data. There is intentionally no implicit conversion between StringId and
the underlying raw integer type.
*/

  // comparison
  const StringId sid0 = SID("123");
  const StringId sid1 = SID("abc");
  ASSERT(sid0 != sid1);

  // concatenation
  const StringId sid2 = SID("12");
  const StringId sid3 = sid2.Concat("3");
  ASSERT(sid0 == sid3);

  // switch cases
  const StringId sid4 = SID("print");
  switch (sid4.GetValue()) {
    case SID_VAL("print"):
      std::cout << "print" << std::endl;
      break;

    case SID_VAL("draw"):
      std::cout << "draw" << std::endl;
      break;
  }

  return 0;
}