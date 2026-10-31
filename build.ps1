Param(
    [Parameter(Mandatory=$false)]
    [Switch] $clean
)

# if user specified clean then remove all generated files
if ($clean.IsPresent)
{
    if (Test-Path -Path "build")
    {
        remove-item build -R
    }
    if (Test-Path -Path "extern")
    {
        # keep extern so restore is not forced every clean; delete only build artifacts
    }
}

$NDKPath = $null
if (Test-Path "./ndkpath.txt") {
    $NDKPath = Get-Content ./ndkpath.txt
} elseif (Test-Path env:ANDROID_NDK_HOME) {
    $NDKPath = $env:ANDROID_NDK_HOME
} else {
    throw "Android NDK not found. Create ndkpath.txt or set ANDROID_NDK_HOME."
}

if ($null -eq (Get-Command "qpm" -ErrorAction SilentlyContinue)) {
    throw "qpm not found in PATH. Install the QPM CLI."
}

& qpm restore
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

# Configure + build (QPM cmake integration expects the NDK toolchain)
cmake -S . -B build -G Ninja `
  "-DCMAKE_TOOLCHAIN_FILE=$NDKPath/build/cmake/android.toolchain.cmake" `
  -DANDROID_ABI=arm64-v8a `
  -DANDROID_PLATFORM=android-29 `
  -DCMAKE_BUILD_TYPE=Release

if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

cmake --build build --parallel
exit $LASTEXITCODE
