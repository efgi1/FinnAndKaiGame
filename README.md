# FinnAndKaiGame

changed USER/.conan/settings to add VS 17
mkdir build && cd build
conan install .. --profile FinnAndKaiGame --build=missing
cmake ..