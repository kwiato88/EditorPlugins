dir .\Build\Debug\*Tests.exe | ForEach-Object { & $_.FullName }
dir .\Build\Release\*MT.exe | ForEach-Object { & $_.FullName }
