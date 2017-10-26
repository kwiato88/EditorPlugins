# Editor Plugins

Package of Notepad++ plugins I use for SW development.  
Plugins binaries can be donwloaded [here]({{ "/assets/nppPluginsBin.zip" | absolute_url }}).

## Open file plugin

Recursively search for file by regex in specified directory.  
Functions:
 * list and jump to file with name matching given regex

## Include browser plugin

Parses files/modules includes hierarchy in specified directories.
Supported languages: C/C++, ttcn3.  
Functions:

 * parse includes hierarchy for specified directory
 * clear parsed hierarchy
 * list and jump to files/modules included in current file/module
 * list and jump to files/modules including current file/module

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
 * search tag bases on its attributes (c, c++);
 * supports extension fields;
 * remembers tag files paths after application restart.

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

   * selectTagsView : Select tags view type. Values: 'gridView' or
     'listView' (default: 'gridView')

   * tagsReader : tag file reader type. Values: 'internal'
     or 'readTagsExe' (default: 'interal').
     * 'internal' - use plugin
       internal implementation to search in tag file
     * 'readTagsExe' - use external tool to read tag file.
       See 'readTagsPath' configuration parameter

   * filterFileScoped : when jump to tag don't show tags with file scope
     if currently in different file (default: false)
 
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
   * enabled : enable plugin logging (default: false)

   * severity: logging severity. Values: 'debug', 'info', 'warning', 'error' (default: 'info')

 * [TagFiles] : Last used tags files. Section updated by plugin
