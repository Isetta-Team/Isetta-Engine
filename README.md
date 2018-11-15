# Isetta Engine
The Isetta Engine is a C++ game engine developed over 3 months for the twin-stick shooter genre.
Please visit our website [https://isetta.io](https://isetta.io) for a full documentation of our journey in making this engine.

## Engine Architecture and Progress
![Architecture Diagram](ArchitectureDiagram.png?raw=true "Architecture Diagram")

## Getting Started
### Environment
- A 64-bit Windows operating system ([How to check?](https://www.howtogeek.com/howto/21726/how-do-i-know-if-im-running-32-bit-or-64-bit-windows-answers/))
- [Visual Studio 2017](https://visualstudio.microsoft.com/)
	- Install VC++ 2017 and Windows 10 SDK (10.0.17134.0). With *Visual Studio Installer*, select "Desktop development with C++" and the correct Windows 10 SDK version.
	- You may need to restart your computer
- [CMake](https://cmake.org/download/) 3.10.2 or later
	- Make sure you add it to your PATH (either check the box during the installation process of follow [this](https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10/))

To clone the engine:
```
git clone --recursive https://github.com/Isetta-Team/Isetta-Engine.git
```

## Compiling
**Before you build** you must pull and build the submodules:
- Pull all submodules by using `git submodule update --init --recursive`
- Build the submodules
	- [ ] GLFW & Horde3D. Either:
		- Run the `Dependency.bat` batch script in the repo (requires CMake in path), if it failed, see below
		- Build manually:
			- [ ] GLFW:
				1. Open a CMake Window
				2. Set the source path to `REPO/Isetta/External/GLFW`
				3. Set the binary path to `REPO/Isetta/External/GLFW/Build`
				4. Click "configure" (select Visual Studio 15 2017 Win64 compiler), "generate" and then "open" the project
				5. Build under both Debug and Release configurations
			- [ ] Horde3D (should be built **AFTER** GLFW):
				1. Open a CMake Window
				2. Set the source path to `REPO/Isetta/External/Horde3D`
				3. Set the binary path to `REPO/Isetta/External/Horde3D/Build`
				4. Configure (select Visual Studio 15 2017 Win64 compiler)
				5. Set the `GLFW_INCLUDE_DIR` to `REPO/Isetta/External/GLFW/include`
				6. Set the `GLFW_LIBRARY_PATH` to `REPO/Isetta/External/GLFW/Build/src/Release/glfw3.lib`
				7. Uncheck `HORDE_BUILD_EXAMPLES`
				8. Generate and open the project
				9. Build under both Debug and Release configurations
	- [ ] ImGui
		1. Open the Isetta.sln
		2. In the solution explorer expand the folders: `Solution 'Isetta' > External > imgui`
		3. Right-click lib_glfw_opengl3, select `Build`
	- [ ] Yojimbo
		1. Go to the yojimbo folder and run `.\premake5.exe solution`
		2. Build the yojimbo project (only that project, not the entire solution!) for both the Release and Debug configurations

## Using the Engine
You can start playing with our first demo game! You'll need a controller for it.
1. Set build configuration to `Debug DLL`
2. Set "IsettaTestBed" as start up project
3. Build "IsettaEngine" 
4. Set `start_level` in `config.cfg` or `user.cfg` to "Level1" 
	- Refer [below](#Configuration) for configuration settings
6. Build and run and start playing! There will also be a bunch of values for you to tweak

### Build Configurations
There are currently 3 configuration options with this solution of the engine: 
- Debug: `Debug` settings and level is built/run in-engine (not using engine dll)
- Debug DLL: `Debug` settings and level uses engine DLL (separate project from engine source)
- Release: `Release` settings (optimized code) and level is built/run in-engine (not using engine dll)
  
There are different levels available in the projects of the solution; ones in IsettaEngine have most recent/testing features, ones in IsettaTestbed are functional tech demos and games. To run any level built within the engine, use Debug or Release and set startup project as IsettaEngine (right-click the IsettaEngine project and select "Set as Startup Project"). To run any level within IsettaTestbed, the configuration must be Debug DLL (to build the engine into a DLL), the startup project should then be IsettaTestbed.

`Debug DLL` will build the `.dll` for you to use and export. To use your version of the engine in another solution/project, build with `Debug DLL` and use those `.dll` and `.lib` built files in your project (follow [Isetta-Game](https://github.com/Isetta-Team/Isetta-Game) instructions for more information). You will also need to export your changed header files, use this command in a command line: `rsync -a --include '*/' --include '*.h' --exclude '*' Isetta/IsettaEngine/ Target_Directory/`. This command is to be run in the top level of the git folder, but source and target directory can be changed as needed.

### Configuration
The configuration settings of the engine are placed with `config.cfg` within the IsettaEngine folder.
For personal configuration settings, create a `user.cfg` in the same folder as `config.cfg
If you want any personal options when using the engine (such as selecting a level for your computer to run) without changing the `config.cfg` you will need a `user.cfg`. You will need a `user.cfg` for each project (ie. IsettaEngine, IsettaTest, and IsettaTestbed), this fill needs to be placed at the root directory of the project (not the .sln).

### Selectable Levels
In your `user.cfg` add the lines: 
```
# Level Settings
start_level = LEVEL_NAME
```
`LEVEL_NAME` can then be replaced with any of these options:
- When running IsettaEngine:
	- `InEngineTestLevel`: Level with a fly camera, light, and model loading
	- `CollisionsLevel`: Level testing our collision intersections
	- `BVHLevel`: Level testing our dynamic AABB tree
	- `EditorLevel`: Level showing the editor components: inspector, heirarchy, and console
	- `EmptyLevel`: Empty level to be used as a starting point for user created levels
	- `EventTestLevel`: Level testing our event messaging system
- When running IsettaTestBed:
	- `ExampleLevel`: Level with an animating model and example component
	- `Level1`: First demo twin-stick shooter game we created!
	- `DebugLevel`: Level demoing our debug drawing capabilities
	- `GUILevel`: Level demoing our GUI capabilities

## Dependencies/Plugins
- [imgui](https://github.com/Isetta-Team/imgui)
- [yojimbo](https://github.com/Isetta-Team/yojimbo)
- [brofiler](https://github.com/Isetta-Team/brofiler)
- GLFW
- Horde3D
- FMOD
- SID

## Stepping Time
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
