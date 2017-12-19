Param(
[string]$compiler="msvc",
[string]$type="Debug",
[string]$oldtarget="rtype.exe")

function print_settings($compiler,$type)
{
  Write-Host "compiler: $compiler"
  Write-Host "type: $type"
}

function remove_old_build($oldtarget, $type)
{
    Remove-Item ./build-$type -Force -Recurse
    Remove-Item ./test-result -Force -Recurse
    Remove-Item ./bin/$type -Force -Recurse
    Remove-Item ./bin/*.dll -Force
    Remove-Item ./bin/$oldtarget -Force
}

function build($type)
{
    mkdir build-$type
    cd build-$type
    $cmd = "-DCMAKE_BUILD_TYPE=$type"
    cmake $cmd -G "Visual Studio 15 2017 Win64" ..
    cmake --build . --config $type
}

function run_xunit($type)
{
    cd ..
    cd bin/$type
    foreach ($file in get-ChildItem *.exe) {
    $var = $file.name
    echo $var
    & ./$var --gtest_output="xml:$var-windows-$type-result.xml"
    }
    cd ../..
}

function run_ctest($type)
{
   cd build-$type
   ctest --no-compress-output -T Test -C $type
   cd ..
}

function publish_result($type)
{
    mkdir test-result
    cd test-result
    mkdir ctest
    cd ..
    cp bin/$type/*.xml test-result/
    cp build-$type/Testing/*/*.xml test-result/ctest
}

function run($compiler, $type, $oldtarget) {
   remove_old_build $oldtarget $type
   build $type
   run_xunit $type
   run_ctest $type
   publish_result $type
}

print_settings $compiler $type
run $compiler $type $oldtarget

