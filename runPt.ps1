#$env:projectRootPath = "D:\test\EditorPlugins\"
[string]$currentPath = pwd
$env:projectRootPath = $currentPath + "\"
dir .\Build\nppCtagPlugin\Release\*PT.exe | ForEach-Object { & $_.FullName }
