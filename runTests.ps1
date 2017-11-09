dir .\Build\nppCtagPlugin\Debug\*Tests.exe | ForEach-Object { & $_.FullName }
dir .\Build\nppHeaderSourceSwitcher\Debug\*Tests.exe | ForEach-Object { & $_.FullName }
dir .\Build\nppIncludeBrowser\Debug\*Tests.exe | ForEach-Object { & $_.FullName }
#dir .\Build\nppOpenFile\Debug\*Tests.exe | ForEach-Object { & $_.FullName }
dir .\Build\nppCtagPlugin\Release\*MT.exe | ForEach-Object { & $_.FullName }
