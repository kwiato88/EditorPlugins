# Editor Plugins

Package of Notepad++ plugins I use for SW development.  
Page: https://kwiato88.github.io/EditorPlugins/


## Open file plugin

Recursively search for file by regex in specified directories.  
Functions:
 * list and jump to file with name matching given regex.


## Include browser plugin

Parses files/modules includes hierarchy in specified directories.
Supported languages: C/C++, ttcn3.  
Functions:

 * parse includes hierarchy for specified directory;
 * clear parsed hierarchy;
 * view and jump to file/module in includes hierarchy for current file/module;
 * provides interface for other plugins.


## Include browser plugin client

Includes static library that should be used to send commands 
to include browser plugin from other plugin. Library uses boost libraries 
in version 1.61.0. Includes also example plugin using provided library. 
Client plugin needs to link required boost libraries.


## Header/Source switcher plugin

Switch between header and source file. Supported languages: C/C++.  
Allowed file extensions mapping:

 * .cpp, .cc -> .hpp, .hh
 * .hpp, .hh -> .cpp, .cc
 * .c -> .h
 * .h -> .c

Allowed directories mapping:

 * Source -> Include
 * Include -> Source
 * src -> inc
 * inc -> src


## CTags plugin

### Functionality:
 * jump to selected tag;
 * navigate through tags;
 * support many tag files;
 * generate tag file for many source directories;
   (recursive generation for directories);
 * show children tags for selected tag (c, c++);
 * show tag hierarchy for selected tag (c, c++);
 * search tag based on its attributes (c, c++);
 * supports extension fields;
 * remembers tag files paths after application restart;
 * provides interface for other plugins;
 * tag files should include absolute paths.

### Instalation:
 * copy plugin binary to Notepad++ plugins directory;
 * open and close Noteoad++ (default plugin configuration file will be generated);
 * open plugin configuration file ('nppCTagPlugin.ini' in plugins configuration directory).
   Do not edit it while Noteoad++ is running;
 * add 'ctagsPath' variable with path to 'ctags.exe' under 'Tags' section. Example:
```
[Tags]
ctagsPath=D:\ctags\ctags.exe
```
 * optionaly change other parameters

### Configuration:
Plugin has a configuration file (nppCTagPlugin.ini) in Notepad++ plugins
configuration directory. __Do not modify configuration file while Notepad++
is running__. All you changes will be overridden by plugin on exit.

__Example configuration file:__
```
[Tags]
ctagsPath=D:\ctags\ctags.exe
readTagsPath=D:\ctags\readtags.exe
selectTagsView=gridView
tagsReader=internal
filterFileScoped=false
useCache=false
[Fields]
access=true
inherits=true
kind=true
prototype=false
typeref=true
file=true
[Logs]
enabled=true
severity=info
[TagFiles]
0=D:\tagsFile1.txt
1=D:\tagsFile2.txt
```

__Description:__  

 * [Tags]
   * ctagsPath : Path to 'ctags.exe'. If parameter is missing in
     configuration file plugin will assume 'ctags.exe' is in
     directory specified in environment variable 'PATH'
    
   * readTagsPath : Path to 'readtags.exe'. If parameter is missing in
     configuration file plugin will assume 'readtags.exe' is in
     directory specified in environment variable 'PATH'.

   * selectTagsView : Select tags dialog type. Values: 'gridView' or
     'listView' (default: 'gridView')

   * tagsReader : tag file reader type. Values: 'internal'
     or 'readTagsExe' (default: 'interal').
     * 'internal' - use plugin
       internal implementation to search in tag file (faster then other option)
     * 'readTagsExe' - use external tool to read tag file.
       See 'readTagsPath' configuration parameter

   * filterFileScoped : when jump to tag don't show tags with file scope
     and defined in different file than current file(default: false)
 
   * useCache : load tags from defined files to memory. Use loaded
     tags instead of searching through files every time. Tags cache
     is updated when last load time is older than last write time for
     tag file (default: false). It is advised to use tag info and
     tag hierarchy function with 'useCache' set to 'true' due to
     performance issues.

 * [Fields] : specify which extension fields are to be generated
    by generate tags function. Specify which extension fields
    are to be displayed in select tags view (applies to 'gridView' only)

 * [Logs] : plugin logs are stored in file 'nppCTagPlugin.logs'
   in plugins configuration dir
   * enabled : enable plugin logging. Values: 'true' or 'false' (default: false)

   * severity: logging severity. Values: 'debug', 'info', 'warning', 'error' (default: 'info')

 * [TagFiles] : Last used tags files. Section updated by plugin


 ## CTags plugin client

Includes static library that should be used to send commands to CTags plugin 
from other plugin. Library uses boost libraries in version 1.61.0. 
Includes also example plugin using provided library. Client plugin needs 
to link required boost libraries.