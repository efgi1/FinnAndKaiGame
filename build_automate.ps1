if (Test-Path ./build)
{
    Set-Location ./build
}
else {
    New-Item -ItemType Directory ./build && Set-Location ./build
}
conan install .. -g cmake_multi -s build_type=Release
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
