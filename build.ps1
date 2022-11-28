if (Test-Path ./build)
{
    Set-Location ./build
}
else {
    New-Item -ItemType Directory ./build && Set-Location ./build
}
conan install .. -g cmake_multi -s build_type=Release --profile game_release --build=missing
conan install .. -g cmake_multi -s build_type=Debug --profile game_debug --build=missing
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
cmake --build . --config Debug