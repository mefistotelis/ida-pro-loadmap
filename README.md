# IDA Pro plugin

Loads a VC/Borland/Dede/GCC/IDA map file into IDA Database.
The ".map" file may be generated during compilation, and contain some of debug info (function names, global variables).

This plugin was compiled with IDA 7.0 SDK, compiler - Visual Studio 2010; it is 32-bit.

See src/LoadMap.cpp for credits, license and changelog.

## Installation

* Copy loadmap.dll to IDA plugins folder
* Open any PE/LE file project
* Click Load MAP with Shift to see options

## Building

To rebuilt the project on Windows, copy it to IDA SDK 'plugins/loadmap' folder, then open SLN file and build it with Visual Studio.

## Known issues

Currently it doesn't understand MAP files with 64-bit offsets - new versions of GCC produce files with such long offsets.
WA for this is to just remove excessive zeros from offsets in MAP file before loading it.
