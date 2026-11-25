@echo off
cd /d "%~dp0"
 powershell -NoProfile -Command "Get-ChildItem -File | Where-Object { $_.LastWriteTime -gt (Get-Date) } | ForEach-Object { $_.LastWriteTime = Get-Date }"
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release || exit /b 1
cmake --build build --parallel 2 || exit /b 1
ctest --test-dir build --output-on-failure || exit /b 1
