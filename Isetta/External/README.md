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

# Setting your IP address for networking
The current maximum number of supported clients is 4, but this is configurable in the engine and may be exposed in `Isetta/config.cfg` at a later time.

The networking client and server IP addresses are configurable via the config file at `Isetta/config.cfg` under Network Settings. The IP address of your computer can be found by running `ipconfig` in the command line on a Windows machine (there may be several IP addresses; you'll want to keep to the most "local" network available, since going over the internet usually requires extra steps).
- To run the client and server on a single computer, set `default_client_ip` to `0.0.0.0` and `default_server_ip` to `127.0.0.1`, and set both `connect_to_server` and `run_server` to `1`.
- To run the server on a computer that will be connected to by other computers, set `default_server_ip` to your computer's IP address and set `run_server` to `1`. If you also want to connect to the server from the same computer, set the `default_client_ip` to `0.0.0.0` and `connect_to_server` to `1`. To connect other computers to the server, set their `default_server_ip` values to the same `default_server_ip` value of the server computer, and set the `default_client_ip` values to each individual computer's IP address (matching the "locality" of the server's IP address). The client computers should have `connect_to_server` set to `1` and `run_server` set to `0`.

When running with separate computers for client and server, make sure you start the server process first before starting the client processes. This will be managed in the future, but while we still have a primitive test case you'll need to do things in the proper order.