#$env:projectRootPath = "D:\Projects\EditorPlugins\"
[string]$currentPath = pwd
$env:projectRootPath = $currentPath + "\"
$env:projectRootPathWith2Slash = ($env:projectRootPath -Replace "\\","\\")
.\prepareTagFiles.ps1 $env:projectRootPath
Write-Host
Write-Host "Run module tests" -ForegroundColor Magenta

dir .\Build\nppCtagPlugin\Release\*MT.exe | ForEach-Object { Write-Host; Write-Host "Run tests from $_" -ForegroundColor Magenta; & $_.FullName }
dir .\Build\nppProjectPlugin\Release\*MT.exe | ForEach-Object { Write-Host; Write-Host "Run tests from $_" -ForegroundColor Magenta; & $_.FullName }
