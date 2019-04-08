#$env:projectRootPath = "D:\Projects\EditorPlugins\"
[string]$currentPath = pwd
$env:projectRootPath = $currentPath + "\"
$env:projectRootPathWith2Slash = ($env:projectRootPath -Replace "\\","\\")
.\prepareTagFiles.ps1 $env:projectRootPath
Write-Host "Run module tests" -ForegroundColor Magenta
Write-Host

dir .\Build\nppCtagPlugin\Release\*MT.exe | ForEach-Object { & $_.FullName }
dir .\Build\nppProjectPlugin\Release\*MT.exe | ForEach-Object { & $_.FullName }
