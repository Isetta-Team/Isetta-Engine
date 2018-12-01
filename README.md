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

`Debug DLL` will build the `.dll` for you to use and export. To use your version of the engine in another solution/project, build with `Debug DLL` and use those `.dll` and `.lib` built files in your project (follow [Isetta-Game](https://github.com/Isetta-Team/Isetta-Game) instructions for more information). You will also need to export your changed header files, external library headers, resources, etc. Use the `ExportHeaders.bat` batch script, located in the top level of the repo, which will export all of the necessary files to a folder labeled includes. You can alter the batch script as needed to include more/less of the engine.

### Configuration
The configuration settings of the engine are placed with `config.cfg` within the IsettaEngine folder.
For personal configuration settings, create a `user.cfg` in the same folder as `config.cfg`
If you want any personal options when using the engine (such as selecting a level for your computer to run) without changing the `config.cfg` you will need a `user.cfg`. You will need a `user.cfg` for each project (ie. IsettaEngine, IsettaTest, and IsettaTestbed), this fill needs to be placed at the root directory of the project (not the .sln).

### Selectable Levels
In your `user.cfg` add the lines: 
```
# Level Settings
start_level = LEVEL_NAME
```
`LEVEL_NAME` can then be replaced with any of these options:
- When running IsettaEngine:

|	Level Name			|	Level Description																|	Level Inputs	|
|		:-:				|			:-:																		|		:-:			|
|	`NoCameraLevel`		|	Level that may be used when the user doesn't have a camera in their own level	|	N/a 			|

- When running IsettaTestBed:

|	Level Name			|	Level Description																|	Level Inputs	|
|		:-:				|			:-:																		|		:-:			|
|	`AILevel`			|	Level showing how navigation module works in the engine and how to use the particle system                                                               |	`WASD`: Move the cube around <br> `J`: Spawn one navigation agent <br> `K`: Spawn one hundred navigation agent <br> `L`: Turn off the fire <br> `O`: Turn on the fire <br> `P`: Remove the top left target				|
|	`AudioLevel`		|	Level with 2D and 3D audio looping and one shot 								|	`NUM2`: 2D audio play/pause loop <br> `NUM3`: 3D audio play/pause one shot	|
|	`BVHLevel`			|	Level testing our dynamic AABB tree												|	`FlyController` <br> `LEFT_CLICK`: Raycast colliders <br> `KP_5`: Spawn 100 entities each with their sphere collider <br> `KP_6`: Spawn 1 entity with sphere collider <br> `KP_4`: Destroy an entity spawn <br> `KP_ENTER`: Show/Hide AABB of spheres <br> `SPACE`: Start/Stop spheres from moving <br> `KP_7`: Decrease spheres' movement range <br> `KP_9`: Increase spheres' movement range |
|	`CollisionsLevel`	|	Level testing our collision intersections										|	`FlyController` <br> `LEFT_CLICK`: Raycast colliders
|   `CollisionSolverLevel`	|	Level testing our collision solving system							|   `FlyController` <br> `KeyTransform` <br> `1`: Select the box collider to move <br> `2`: Select the sphere collider to move <br> `3`: Select the capsule collider to move <br> `9`: Decrease the mass of the sphere collider by 10 (can go negative) <br> `0`: Increase the mass of the sphere collider by 10
|	`Debug Level`		|	Level demoing our debug drawing capabilities									|	`FlyController` <br> `V`: Draw ray in a circle while pressed <br> `B`: Draw plane while pressed	|
|	`EditorLevel`		|	Level showing the editor components: inspector, heirarchy, and console and level loading menu  |	`ESC`: close window <br> `F1`:level menu <br> `CTRL+SHIFT+`: `H`-hierarchy, `I`-inspector, `F`-frame reporter, `C`-console, `M`-menu	|
|	`EmptyLevel`		|	Empty level to be used as a starting point for user created levels				|	N/a				|
|	`EventLevel`		|	Level demoing our event messaging system with sender and listener components	|	`A`: Raise a queued event <br> `S`: Raise an immediate event <br> `D` Raise three queued events with different priorities and timeframes				|
|	`ExampleLevel`			|	Level with an animating model and example component								|	`FlyController` <br> `UP_ARROW`: Move the wire box forward <br> `DOWN_ARROW`: Move the wire box backward <br> `LEFT_ARROW`: Make the wire box turn left <br> `RIGHT_ARROW`: Make the wire box turn Right |
|	`GUILevel`			|	Level demoing some of our GUI capabilities										|	N/a				|
|	`InputLevel`		|	Level demoing some of the input capabilities 									|	`A`: press/release messages <br> `MOUSE_LEFT`: message on 1st click <br> `GAMEPAD_{X,B,MOUSE_MIDDLE}`: message while pressed/down				|
|	`LevelLoadingLevel`		|	Level showing a menu to browse levels and load specific level 										|	`ESC`: close window				|
|	`MeshAnimLevel`		|	Level with a mesh that is being animated 										|	`FlyController`				|
|	`NetworkLevel`		|	Level demoing some of our networking capabilities. The `default_server_ip` in config should be set to your LAN IP for this level to work. |	`FlyController` <br> `Y`: Spawn a zombie across the network <br> `H`: Despawn a zombie across the network (can only be done by the spawner!) <br> `U`: Spawn a zombie and set its parent to the previously spawned zombie <br> `I`: Parent the previously spawned zombie to the zombie spawned before it <br> `K`: Unparent the previously spawned zombie from its parent <br> `P`: Send a generic handle message with integer 0 <br>  `O`: Send a generic handle message with integer 1 <br> `Mouse Left`: Send a generic handle message with integer 2 <br> `F1`: Start host <br> `F2`: Start pure server <br> `F3`: Start client <br> `NUM1`: Start broadcasting messages to all computers in LAN <br> `NUM2`: Start listening to broadcast messages <br> `Keypad9`: Switch to another level using `NetworkLoadLevel` <br> `Keypad7`: Switch back to NetworkLevel using `NetworkLoadLevel` |
|	`PrimitiveLevel`	|	Level displaying all the types of primitive objects 							|	`FlyController`				|
|	`SkeletonLevel`		|	Level displaying a mesh and entities used to follow the skeleton 				|	N/a				|
|	`Halves`			|	First demo twin-stick shooter game we created! Only support gamepads			| `LFET_STICK`: move around <br> `RIGHT_STICK`: shoot |
|	`KnightGame`		|	Game with a knight and a sword, can you take down the most training dummies?	|	`LEFT_STICK`: moves knight <br> `RIGHT_TRIGGER`: raises/lowers sword <br> `RIGHT_STICK`: spin it in a complete circle for flame protection	|
|	`Week10MiniGame`	|	First demo game with networking. Fool your enemy with your sword young man!		|	`NUM1`: Start as a host <br> `NUM2`: Start as a client <br> `R`: Ready for the fight <br> `AD`: Move left or right <br> `Up/Down Arrow`: Switch the sword position <br> `Space`: Stab!				|


## Dependencies/Plugins
- [imgui](https://github.com/Isetta-Team/imgui)
- [yojimbo](https://github.com/Isetta-Team/yojimbo)
- [brofiler](https://github.com/Isetta-Team/brofiler)
- [GLFW](https://github.com/glfw/glfw)
- [Horde3D](https://github.com/horde3d/Horde3D)
- [FMOD](https://www.fmod.com/api)
- [SID (String ID](https://github.com/TheAllenChou/string-id)

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
