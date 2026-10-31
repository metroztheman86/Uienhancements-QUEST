#!/usr/bin/env bash
set -euo pipefail

# Local Linux/macOS helper (PowerShell scripts are the primary path on Windows).
: "${ANDROID_NDK_HOME:?Set ANDROID_NDK_HOME to an installed Android NDK (r27+ recommended)}"

if ! command -v qpm >/dev/null 2>&1; then
  echo "qpm CLI not found. Install with: cargo install qpm-cli --locked" >&2
  exit 1
fi

qpm restore

cmake -S . -B build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake" \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_PLATFORM=android-29 \
  -DCMAKE_BUILD_TYPE=Release

cmake --build build --parallel

echo "Built: build/libUIEnhancements.so"
echo "Next: package with the createqmod script or zip mod.json + the .so"
