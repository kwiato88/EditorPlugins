Write-Host "Generate tag files" -ForegroundColor Magenta
[string]$projectRoot = $args[0]
[string]$projectRootWith2Slash = ($projectRoot -replace "\\","\\")
[string]$tagFilesPath = $projectRoot + "\nppCtagPlugin\Tests\TestSourceCode\tags*.txt"
$tagFiles = Get-ChildItem -Path ($tagFilesPath)
foreach ($file in $tagFiles)
{
    if(-not ((Get-Content $file) -eq $null))
    {
        $outFile = $file.Directory.FullName + "\tmp_" + $file.Name
        if(-not (Test-Path $outFile))
        {
            Write-Host "  Generate $outFile"
            $(Get-Content $file).Replace("PROJECT_ROOT", $projectRootWith2Slash) | Set-Content $outFile
        }
        else
        {
            Write-Host "  Skip $outFile. File already exists"
        }
    }
}
Write-Host
