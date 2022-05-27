# Ruiqi Lei
# This script is intended to merge sources into a single .C file.

$outFileName = "../output.c";
$headers = @("list.h", "definition.h", "main.h", "report.h", "input.h", "dispatcher.h");
$include = @("#include <stdio.h>", "#include <stdlib.h>", "#include <string.h>", "");
$include | Out-File $outFileName;

# Collect headers.
foreach ($header in $headers) {
    $content = Get-Content $header;
    foreach ($line in $content) {
        if ($line.StartsWith("#") -and (-not $line.Contains("MAX")) -and (-not $line.Contains("//"))) {
            continue;
        }
        $line | Out-File -FilePath $outFileName -Append;
    }
}

# Main function.
$content = Get-Content "main.c";
foreach ($line in $content) {
    if (!$line.StartsWith("#include")) {
        $line | Out-File -FilePath $outFileName -Append;
    }
}

# Collect sources.
$sources = [System.Collections.ArrayList]::new();
foreach ($file in (Get-ChildItem).Name) {
    if (((Get-ChildItem $file).Extension -eq ".c") -and ($file -ne "main.c")) {
        $_ = $sources.Add($file);
    }
}
$sources.Sort();

foreach ($source in $sources) {
    $content = Get-Content $source;
    foreach ($line in $content) {
        if ($line.StartsWith("#include") -or $line.StartsWith("extern")) {
            continue;
        }
        $line | Out-File -FilePath $outFileName -Append;
    }
}

$content = Get-Content $outFileName;
$whiteLine = 0;
$null | Out-File -FilePath $outFileName;
for ($i = 0; $i -lt $content.Length; $i++) {
    $line = $content[$i];
    if ($line.CompareTo("") -eq 0) {
        $whiteLine++;
    } else {
        $whiteLine = 0;
    }
    if ($whiteLine -le 1) {
        $line | Out-File -FilePath $outFileName -Append;
    }
}
