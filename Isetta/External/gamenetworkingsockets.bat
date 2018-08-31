
cd %~dp0\Isetta\External\GameNetworkingSockets
git submodule add https://github.com/google/protobuf
cd protobuf
git checkout origin/3.5.x
set PATH=%PATH%;"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE"
mkdir cmake_build
cd cmake_build
REM set curr_dir=%cd%
REM if not defined DevEnvDir (
REM  vcvarsall amd64
REM )
REM cd %^curr_dir%
cmake ^
-G "Visual Studio 15 2017 Win64" ^
-DCMAKE_BUILD_TYPE=Release ^
-Dprotobuf_BUILD_TESTS=OFF ^
-Dprotobuf_BUILD_SHARED_LIBS=ON ^
-DCMAKE_INSTALL_PREFIX="C:\Program Files\Protobuf SDK\protobuf-amd64" ..\cmake
devenv protobuf.sln /Build "Release|x64"

devenv protobuf.sln /Build "Release|x64" /Project INSTALL.vcxproj

cd ..\..
mkdir build
cd build
set PATH=%PATH%;”C:\Program Files\Protobuf SDK\protobuf-amd64\bin”
REM set curr_dir2=%cd%
REM if not defined DevEnvDir (
REM  vcvarsall amd64
REM )
REM cd %^curr_dir2%
cmake -G "Visual Studio 15 2017 Win64" ^
-DProtobuf_LIBRARIES="C:\Program Files\Protobuf SDK\protobuf-amd64\lib\libprotobuf.lib" ^
-DProtobuf_INCLUDE_DIR="C:\Program Files\Protobuf SDK\protobuf-amd64\include" ^
-DProtobuf_PROTOC_EXECUTABLE=..\protobuf\cmake_build\Release\protoc.exe ..
devenv GameNetworkingSockets.sln /Build "Release|x64"
pause