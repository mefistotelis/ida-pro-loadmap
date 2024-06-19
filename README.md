# IDA Pro plugin for loading .MAP symbol files

Loads a VC/Borland/Dede/GCC/IDA `.map` file into IDA Database.

## What is the MAP file

The `.map` file may be generated during compilation, and contain some of debug info (function names, global variables,
in some cases locals as well).

In the past, it was generated automatically by many compilers. For some programs, it was included in a pre-release
version, or even in final release.

Todays compiles still have ability to enable `.map` file generation, ie:

* Watcom linker has `OPTION MAP[=map_file]` command line switch
* GCC linker has `-Xlinker -Map=output.map` command line switch
* Microsoft VC linker has `/MAP[:filename]` command line switch

## More information

See src/LoadMap.cpp for credits, license and changelog.

## Installation

* Copy loadmap.dll to IDA plugins folder
* Open any PE/LE file project
* Click Load MAP with Shift to see options

## Building

The building requires IDA Pro SDK.

To rebuilt the project on Windows using GUI, copy it to IDA SDK `plugins/loadmap` folder, then open SLN file and build it with Visual Studio.

To rebuilt the project from command line, check how the Github Actions do that. You will need some GNU tools including make, and VC compiler from Visual Studio.

## Troubleshooting

If the plugin does not show in "Edit" -> "Plugins", then:
* Make sure you have a code project opened
* Check IDA "Output" console, there should be a message either confirming the load or with error
* If the "Output" console shows "Cannot load certain module", you probably lack Visual C++ Redistributable Package
* Check the "Issues" tab of this project on Github for more info

## Known issues

Currently it doesn't understand MAP files with 64-bit offsets - new versions of GCC produce files with such long offsets.
WA for this is to just remove excessive zeros from offsets in MAP file before loading it.

Currently the tool uses Windows API, so will not work with Linux or Mac OS version of IDA Pro. To fix that:
* reading keys would have to be switched to one from Qt
* memory mapped file usage would have to be implemented (or switched to C++ standard method, when it finally arrives)
