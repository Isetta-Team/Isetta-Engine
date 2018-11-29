/*
 * Copyright (c) 2018 Isetta
 */
// #define _CRTDBG_MAP_ALLOC
// #include <crtdbg.h>
// #include <stdlib.h>

#include "Application.h"

using namespace Isetta;

/*! \mainpage Isetta Engine
Game engine development is a very wide field in the industry, but also a very
inaccessible one. Budding engineers are advised to just jump into development to
learn, and it's for this reason that the Isetta Engine project came to be.

Our team is building a game engine specialized for the twin-stick shooter genre.
Every step of the way we will be documenting our process through a dedicated
blog. Readers can hear from us personally and get an in-depth look at our
journey to create the engine. In addition, we will supplement our content
through regular interviews with industry professionals who have built their own
engines before.

Between our own hands-on process and sage advice from veteran engineers, we hope
to give newcomers a clearer representation of the engine-building process.
*/
int main() {
  // _CrtSetBreakAlloc(1199);
  // _CrtSetBreakAlloc(3607);
  Application::Start();
  return 0;
}
