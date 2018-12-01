@echo off
REM Default Base Path (command-line arg 1)
set base=Isetta\
REM Custom Folder Names
set include=Includes\
set header=
set resources=Resources\
set build=Build\
set external=External\
set tools=Tools\

REM Engine Parameters
set engine=IsettaEngine\
set resourceFolders=fonts primitives lights particles pipelines shaders textures

set externalFolders[0].folder=brofiler\ProfilerCore\
set externalFolders[0].files=Brofiler.h
set externalFolders[1].folder=FMOD\inc\
set externalFolders[1].files=*.h*
set externalFolders[2].folder=Horde3D\Horde3D\Bindings\C++\
set externalFolders[2].files=*.h
set externalFolders[3].folder=imgui\
set externalFolders[3].files=imgui.h imconfig.h
set externalFolders[4].folder=SID\
set externalFolders[4].files=sid.h
set externalFolders[5].folder=GLFW\include\GLFW\
set externalFolders[5].files=*.h
set externalFolders[6].folder=yojimbo\
set externalFolders[6].files=yojimbo.h

if NOT "%1"=="" base=%1

REM Header Files
robocopy %base%\%engine% %include%\%header% /S *.h /XD Custom

REM Resource Files
(for %%f in (%resourceFolders%) do ( 
   call robocopy %base%\%engine%\Resources\%%f %include%\%resources%\%%f /S *
))

REM Build Files
robocopy "%base%\Build\IsettaEngine-x64-Debug DLL" %include%\%build%\Debug\ *.dll *.lib *.pdb
robocopy "%base%\Build\IsettaEngine-x64-Release DLL" %include%\%build%\Release\ *.dll *.lib
robocopy "%base%\Build\IsettaEngine-x64-ReleaseEditor DLL" %include%\%build%\ReleaseEditor\ *.dll *.lib

REM Tool Files (can add more like resources)
robocopy %base% %include%%tools% *.exe

REM External Files
set "x=0" 
:loopExternal
if defined externalFolders[%x%].folder (
	if defined externalFolders[%x%].files  (
	   call robocopy %base%\External\%%externalFolders[%x%].folder%% %include%\%external%\%%externalFolders[%x%].folder%% %%externalFolders[%x%].files%%
	   set /a "x+=1"
	   GOTO :loopExternal 
	)
)

cmd /k
