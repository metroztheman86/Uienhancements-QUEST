# Packages the built shared library into a .qmod for 1.40.8_7379
$ErrorActionPreference = "Stop"

$modName = "UIEnhancements"
$soName = "libUIEnhancements.so"
$soPath = "build/$soName"

if (-not (Test-Path $soPath)) {
    throw "Missing $soPath – run the build script first."
}

# Let QPM generate mod.json from the template when available
if (Get-Command qpm -ErrorAction SilentlyContinue) {
    & qpm qmod create
}

# Fallback manual package (works even without the qpm qmod subcommand)
New-Item -ItemType Directory -Force -Path package | Out-Null
Copy-Item $soPath "package/$soName" -Force

if (Test-Path "mod.json") {
    Copy-Item "mod.json" "package/mod.json" -Force
} else {
    # Expand template tokens
    $json = Get-Content "mod.template.json" -Raw
    $json = $json -replace '\$\{mod_name\}', $modName
    $json = $json -replace '\$\{mod_id\}', $modName
    $json = $json -replace '\$\{version\}', "0.1.0"
    $json = $json -replace '\$\{binary\}', $soName
    Set-Content -Path "package/mod.json" -Value $json -NoNewline
}

if (Test-Path "$modName.qmod") { Remove-Item "$modName.qmod" }
Compress-Archive -Path "package/*" -DestinationPath "$modName.zip" -Force
Rename-Item "$modName.zip" "$modName.qmod"
Remove-Item package -Recurse -Force
Write-Host "Created $modName.qmod"
