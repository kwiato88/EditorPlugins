$env:projectRootPath = "D:\test\EditorPlugins\"
dir .\Build\nppCtagPlugin\Release\*PT.exe | ForEach-Object { & $_.FullName }
