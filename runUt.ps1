#$env:projectRootPath = "D:\test\EditorPlugins\"
$env:projectRootPath = "D:\Projects\EditorPlugins\"
#$env:projectRootPathWith2Slash = "D:\\test\\EditorPlugins\\"
$env:projectRootPath = "D:\\Projects\\EditorPlugins\\"
dir .\Build\nppCtagPlugin\Debug\*Tests.exe | ForEach-Object { & $_.FullName }
dir .\Build\nppHeaderSourceSwitcher\Debug\*Tests.exe | ForEach-Object { & $_.FullName }
dir .\Build\nppIncludeBrowser\Debug\*Tests.exe | ForEach-Object { & $_.FullName }
#dir .\Build\nppOpenFile\Debug\*Tests.exe | ForEach-Object { & $_.FullName }
