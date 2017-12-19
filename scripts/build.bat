 rd /s /q .\build\
 for /f %i in ('dir /bin *.dll') do echo %i
 mkdir build
 cd build
 dir
 cmake -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 15 2017 Win64" ..
 cmake --build . --config Debug --target game-template
 cd ..
 cd bin