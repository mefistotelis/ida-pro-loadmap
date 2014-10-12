IDA Pro plugin
Loads a VC/Borland/Dede/GCC/IDA map file into IDA Database.
The ".map" file may be generated during compilation, and contain some of debug info (function names, global variables).

This plugin was compiled with IDA 6.2 SDK, compiler - GCC 4.8.0; it is 32-bit.
IDA SDK required small patch to cooperate with that gcc - it is within src.

See src/LoadMap.cpp for credits, license and changelog.

Installation:
* Copy LoadMAP.plw to IDA plugins folder
* Open any PE/LE file project
* Click Load MAP with Shift to see options

Known issues:
Currently it doesn't understand MAP files with 64-bit offsets - new versions of GCC produce files with such long offsets.
WA for this is to just remove excessive zeros from offsets in MAP file before loading it.
