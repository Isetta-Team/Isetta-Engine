# Building GLFW

We are using CMake GUI tool for building GLFW (v3.10.2 tested).

Instruction:
- Open a CMake Window
- Set the source path to `REPO/Isetta/External/GLFW`
- Set the binary path to `REPO/Isetta/External/GLFW/Build`
- Configure (select Visual Studio 15 2017 Win64 compiler), generate and then open the project
- Build under both Debug mode and Release mode

# Building Horde3D

Horde3D should be built AFTER GLFW built.

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

# Building brofiler, FMOD, SID
Should be setup after running, 'git submodule update --init'

# Building yojimbo
- Run `git submodule update --init --recursive` in the git project or in the yojimbo folder
- Go to the yojimbo folder and run `.\premake5.exe solution` to create a Visual Studio solution
- Build the yojimbo project inside of Yojimbo.sln for both the Release and Debug configurations