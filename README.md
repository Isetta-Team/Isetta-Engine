# Isetta Engine
Please visit our website [https://isetta.io](https://isetta.io) for a full documentation of our journey in making this engine.

## Engine Architecture and Progress
![Architecture Diagram](ArchitectureDiagram.png?raw=true "Architecture Diagram")

## Getting started
First, let's make sure you have the right environment!
- [ ] 64-bit Windows systems ([How to check?](https://www.howtogeek.com/howto/21726/how-do-i-know-if-im-running-32-bit-or-64-bit-windows-answers/))
- [ ] [Visual Studio 2017](https://visualstudio.microsoft.com/)
	- [ ] With VC++ 2017 and Windows 10 SDK (10.0.17134.0). You can install them with *Visual Studio Installer*, just select "Game Development with C++" and the right Windows 10 SDK version.
- [ ] [CMake](https://cmake.org/download/) 3.10.2 or later

**Before you build** you must pull and build the submodules:
- [ ] Pull all submodules by using `git submodule update --init` in a commandline
- [ ] Now you build the submodules

### Building GLFW
We are using CMake GUI tool for building GLFW (v3.10.2 tested).

Instruction:
- Open a CMake Window
- Set the source path to `REPO/Isetta/External/GLFW`
- Set the binary path to `REPO/Isetta/External/GLFW/Build`
- Configure (select Visual Studio 15 2017 Win64 compiler), generate and then open the project
- Build under both Debug mode and Release mode

### Building Horde3D
Horde3D should be built **AFTER** GLFW built.

Instruction:
- Open a CMake Window
- Set the source path to `REPO/Isetta/External/Horde3D`
- Set the binary path to `REPO/Isetta/External/Horde3D/Build`
- Configure (select Visual Studio 15 2017 Win64 compiler)
- Set the `GLFW_INCLUDE_DIR` to `REPO/Isetta/External/GLFW/include`
- Set the `GLFW_LIBRARY_PATH` to `REPO/Isetta/External/GLFW/Build/src/Release/glfw3.lib`
- Uncheck `HORDE_BUILD_EXAMPLES`
- Generate and open the project
- Build under both Debug mode and Release mode

### Building ImGui
Instruction:
- Open the Isetta.sln
- In the solution explorer expand the folders: `Solution 'Isetta' > External > imgui`
- Right-click lib_glfw_opengl3, select `Build`

### Setup Done (for now...)
You can start playting with our (TODO) simplistic demo!

## Mission Statement
Game engine development is a very wide field in the industry, but also a very inaccessible one. 
Budding engineers are advised to just jump into development to learn, and it's for this reason 
that the Isetta Engine project came to be.

Our team is building a game engine specialized for the twin-stick shooter genre. Every step of
the way we will be documenting our process through a dedicated blog. Readers can hear from us 
personally and get an in-depth look at our journey to create the engine. In addition, we will 
supplement our content through regular interviews with industry professionals who have built 
their own engines before.

Between our own hands-on process and sage advice from veteran engineers, we hope to give newcomers
a clearer representation of the engine-building process.

This project will span from August to December of 2018 and all of our progress and code will 
be accessible via the git repo.