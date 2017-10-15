$env:projectRootPath = "D:\test\EditorPlugins\"
$env:projectRootPathWith2Slash = "D:\\test\\EditorPlugins\\"
dir .\Build\nppCtagPlugin\Release\*MT.exe | ForEach-Object { & $_.FullName }
