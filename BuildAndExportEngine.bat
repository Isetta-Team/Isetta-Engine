set engine_path=.
set game_path=../Isetta-Game

REM Reset our local files to the develop branch's HEAD
git -C %engine_path% reset --hard
git -C %engine_path% checkout develop
git -C %engine_path% pull

REM Set some helpful variables for later
set git_commit_id=git -C %engine_path% rev-parse --short HEAD
set VisualStudioPath="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE"
set PATH=%PATH%;%VisualStudioPath%

REM Reset our game to the develop branch as well
git -C %game_path% reset --hard
git -C %game_path% checkout develop
git -C %game_path% pull

REM Build all of the engine DLLs
devenv %engine_path%\Isetta\Isetta.sln /Project %engine_path%\Isetta\IsettaEngine\IsettaEngine.vcxproj /Rebuild "Debug DLL|x64"
devenv %engine_path%\Isetta\Isetta.sln /Project %engine_path%\Isetta\IsettaEngine\IsettaEngine.vcxproj /Rebuild "Release DLL|x64"
devenv %engine_path%\Isetta\Isetta.sln /Project %engine_path%\Isetta\IsettaEngine\IsettaEngine.vcxproj /Rebuild "ReleaseEditor DLL|x64"

REM If any builds fail, we escape
if NOT %errorlevel% == 0 (
    exit /b %errorlevel%
)

REM Run the ExportHeaders batch file
call %engine_path%\ExportHeaders.bat NoStop

REM Copy all of the Includes and Engine files over to our game directory
xcopy Includes %game_path%\TwinStickGame\Engine /s /y /I

REM Push up the changed files to develop
git -C %game_path% add *
git -C %game_path% commit -m "%git_commit_id%"
git -C %game_path% push

pause