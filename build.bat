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
make DRIVE_LETTER="%DRIVE_LETTER%" PROJECT_DIR="%PROJECT_PATH%"
if %errorlevel% neq 0 (
	echo "Unsuccessful compile"
)
if %errorlevel% equ 0 (
	set PATH=%PATH%;%OGRE_PATH%/build/bin;%BOOST_PATH%/lib;%ZLIB_PATH%;%FREEIMAGE_PATH%
	Game.exe
)