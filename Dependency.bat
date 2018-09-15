set VisualStudioPath="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE"

cd Isetta\External\GLFW
set GLFW=%cd%
mkdir Build
cd Build

cmake ^
-G "Visual Studio 15 2017 Win64" ^
..

set PATH=%PATH%;%VisualStudioPath%
devenv GLFW.sln /Build "Debug|x64"
devenv GLFW.sln /Build "Release|x64"

cd ..\..
cd Horde3D
mkdir Build
cd Build

cmake ^
-G "Visual Studio 15 2017 Win64" ^
-DGLFW_INCLUDE_DIR=%GLFW%/include ^
-DGLFW_LIBRARY_PATH=%GLFW%/Build/src/Release/glfw3.lib ^
-DHORDE_BUILD_EXAMPLES=OFF ^
..

devenv Horde3D.sln /Build "Debug|x64"
devenv Horde3D.sln /Build "Release|x64"

pause