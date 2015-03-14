# ida-pro-loadmap
Plugin for IDA Pro disassembler which allows loading .map files.

Some compilers create .map files during compilation. These files contain function names and globals, so they may be used to partially restore debug symbols in non-debug builds, or those where symbols were stripped.

This plugin can name all functions and globals in the IDA project based on information from .map file.

Based on the idea of loadmap plugin by Toshiyuki Tega.

Note: IDA Pro SDK is required to compile the plugin from sources.

[Go to Downloads](https://github.com/mefistotelis/ida-pro-loadmap/blob/wiki/Downloads.md)
