#$env:projectRootPath = "D:\test\EditorPlugins\"
$env:projectRootPath = "D:\Projects\EditorPlugins\"
#$env:projectRootPathWith2Slash = "D:\\test\\EditorPlugins\\"
$env:projectRootPath = "D:\\Projects\\EditorPlugins\\"
dir .\Build\nppCtagPlugin\Release\*MT.exe | ForEach-Object { & $_.FullName }
dir .\Build\nppProjectPlugin\Release\*MT.exe | ForEach-Object { & $_.FullName }
