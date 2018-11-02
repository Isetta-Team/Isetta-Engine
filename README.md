# Isetta Engine
Please visit our website [https://isetta.io](https://isetta.io) for a full documentation of our journey in making this engine.

## Engine Architecture and Progress
![Architecture Diagram](ArchitectureDiagram.png?raw=true "Architecture Diagram")

## Getting started
First, let's make sure you have the right environment!
- [ ] A 64-bit Windows operating system ([How to check?](https://www.howtogeek.com/howto/21726/how-do-i-know-if-im-running-32-bit-or-64-bit-windows-answers/))
- [ ] [Visual Studio 2017](https://visualstudio.microsoft.com/)
	- [ ] Install VC++ 2017 and Windows 10 SDK (10.0.17134.0). You can install them with *Visual Studio Installer*, just select "Game Development with C++" and the correct Windows 10 SDK version.
	- [ ] You may need to restart your computer
- [ ] [CMake](https://cmake.org/download/) 3.10.2 or later
	- Make sure you add it to your PATH (either check the box during the installation process of follow [this](https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10/))

**Before you build** you must pull and build the submodules:
- [ ] Pull all submodules by using `git submodule update --init --recursive` in a commandline
- [ ] Build the submodules
	- [ ] GLFW & Horde3D. Choose one way to do it:
		- Run the `Dependency.bat` batch script in the repo, if it failed, see below
		- Build manually:
			- For GLFW:
				1. Open a CMake Window
				2. Set the source path to `REPO/Isetta/External/GLFW`
				3. Set the binary path to `REPO/Isetta/External/GLFW/Build`
				4. Click "configure" (select Visual Studio 15 2017 Win64 compiler), "generate" and then "open" the project
				5. Build under both Debug mode and Release mode
			- For Horde3D (should be built **AFTER** GLFW):
				1. Open a CMake Window
				2. Set the source path to `REPO/Isetta/External/Horde3D`
				3. Set the binary path to `REPO/Isetta/External/Horde3D/Build`
				4. Configure (select Visual Studio 15 2017 Win64 compiler)
				5. Set the `GLFW_INCLUDE_DIR` to `REPO/Isetta/External/GLFW/include`
				6. Set the `GLFW_LIBRARY_PATH` to `REPO/Isetta/External/GLFW/Build/src/Release/glfw3.lib`
				7. Uncheck `HORDE_BUILD_EXAMPLES`
				8. Generate and open the project
				9. Build under both Debug mode and Release mode
	- [ ] ImGui
		1. Open the Isetta.sln
		2. In the solution explorer expand the folders: `Solution 'Isetta' > External > imgui`
		3. Right-click lib_glfw_opengl3, select `Build`
	- [ ] Yojimbo
		1. Go to the yojimbo folder and run `.\premake5.exe solution`, it will createa nd run a Visual Studio solution
		2. Build the yojimbo project (only that project, not the entire solution!) for both the Release and Debug configurations

### Setup Done (for now...)
You can start playting with our first demo game! You need a controller for it.
1. Set build configuration to "Debug DLL"
2. Build "IsettaEngine" 
3. Set "IsettaTestBed" as start up project
4. Set start up level to "Level1" (you can refer to the next 3 sections on how to do it)
5. Build and run and start playing! There will also be a bunch of values for you to tweak

### Build Configurations
There are currently 3 configuration options with this solution of the engine: Debug, Debug DLL, and Release. To use Debug or Release, the startup project must be the IsettaEngine (this is done by right clicking the IsettaEngine project and selecting "Set as Startup Project"). These configurations will build and run the engine, so the levels/tests we have within their. To run any level within IsettaTestbed, the configuration should be put in Debug DLL to build the engine into a DLL, the startup project should then be IsettaTestbed.

### Create a User Config
If you want any personal options when using the engine (such as selecting a level for your computer to run) without changing the `config.cfg` you will need a `user.cfg`. You will need a `user.cfg` for each project (ie. IsettaEngine, IsettaTest, and IsettaTestbed), this fill needs to be placed at the root directory of the project (not the .sln).

### Selectable Levels
In your `user.cfg` add the lines: 
```
# Level Settings
start_level = LEVEL_NAME
# End
```
`LEVEL_NAME` can then be replaced with any of these options:
- When running IsettaEngine:
	- `InEngineTestLevel`: Level with a fly camera and light
	- `CollisionsLevel`: Level testing our collision intersections
	- `BVHLevel`: Level testing our dynamic AABB tree
- When running IsettaTestBed:
	- `ExampleLevel`: Level with an animating model and example component
	- `Level1`: First demo twin-stick shooter game we created!
	- `DebugLevel`: Level demoing our debug drawing capabilities
	- `GUILevel`: Level demoing our GUI capabilities

### Building the Engine
There are currently 3 configuration options with this solution of the engine: Debug, Debug DLL, and Release. To use Debug or Release, the startup project must be the IsettaEngine (this is done by right clicking the IsettaEngine project and selecting "Set as Startup Project"). These configurations will build and run the engine, so the levels/tests we have within their. To run any level within IsettaTestbed, the configuration should be put in Debug DLL to build the engine into a DLL, the startup project should then be IsettaTestbed.

### Create a User Config
If you want any personal options when using the engine (such as selecting a level for your computer to run) without changing the `config.cfg` you will need a `user.cfg`. You will need a `user.cfg` for each project (ie. IsettaEngine, IsettaTest, and IsettaTestbed), this fill needs to be placed at the root directory of the project (not the .sln).

### Selectable Levels
In your `user.cfg` add the lines: 
```
# Level Settings
start_level = LEVEL_NAME
# End
```
`LEVEL_NAME` can then be replaced with any of these options:
- InEngineTestLevel (only in IsettaEngine): Level with a fly camera and light
- CollisionsLevel (only in IsettaEngine): Level testing our collision intersections
- ExampleLevel (only in IsettaTestbed): Level with an animating model and example component
- Level1 (only in IsettaTestbed): First demo twin-stick shooter game we created!
- DebugLevel (only in IsettaTestbed): Level demoing our debug drawing capabilities
- GUILevel (only in IsettaTestbed): Level demoing our GUI capabilities

## Going Through Time
Assuming you have checked out master, you are looking at our latest work. If you would like to view the engine from the start you can use the tags feature to see the code week to week. If you are interested in a specific feature, there are branches for each of the major systems of development for you to `checkout` (the majority of development happened on those branches, we slipped up sometimes and made changes on staging).

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