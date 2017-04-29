@echo off
cls

set DRIVE_LETTER=%1:
set GAME_PATH=%DRIVE_LETTER%/Game_Engine
set CMAKE=C:/Program Files/CMake
set TDM=%DRIVE_LETTER%/Game_Engine/TDM-GCC-64
set PATH=%TDM%/bin;%CMAKE%/bin;c:/windows
set PROJECT_PATH=.
set OGRE_PATH=%GAME_PATH%/Ogre1.8.1
set BOOST_PATH=%GAME_PATH%/boost
set FREEIMAGE_PATH=%GAME_PATH%/FreeImage/lib
set ZLIB_PATH=%GAME_PATH%/zlib-1.2.11/lib
set OIS_PATH=%GAME_PATH%/ois
set LUA_PATH=%GAME_PATH%/lua-5.3.4
set LUA_BRIDGE_PATH=%GAME_PATH%/LuaBridge
set BASS_PATH=%GAME_PATH%/bass2.4
set CEGUI_PATH=%GAME_PATH%/cegui-0.8.7
set PATH=%PATH%;%OIS_PATH%/lib;%OGRE_PATH%/build/bin;%BOOST_PATH%/lib;%ZLIB_PATH%;%FREEIMAGE_PATH%;%BASS_PATH%;%CEGUI_PATH%/bin;%LUA_BRIDGE_PATH%
make -j 4 DRIVE_LETTER="%DRIVE_LETTER%" OIS_PATH="%OIS_PATH%" PROJECT_DIR="%PROJECT_PATH%" OGRE_PATH="%OGRE_PATH%" BASS_PATH="%BASS_PATH%" LUA_BRIDGE_PATH="%LUA_BRIDGE_PATH%"
if %errorlevel% neq 0 (
	echo "Unsuccessful compile"
)
if %errorlevel% equ 0 (
	Game.exe
)
