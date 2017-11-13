Remove-Item ./bin/*.dll -Force
Remove-Item ./bin/rtype.exe -Force
Remove-Item ./build -Force -Recurse
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 15 2017 Win64" ..
cmake --build . --config Debug --target rtype
cd ..
cd bin