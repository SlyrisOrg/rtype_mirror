function WindowsBuild($BuildType) {
    $cmd="-DCMAKE_BUILD_TYPE=$($BuildType) -G `"Visual Studio 15 2017 Win64`" .."
    Remove-Item ./bin/*.dll -Force
    Remove-Item ./bin/rtype.exe -Force
    Remove-Item ./build -Force -Recurse
    mkdir build
    Set-Location build
    cmake $cmd
    cmake --build . --config $BuildType --target rtype
    Set-Location ..
    Set-Location bin
}
function LinuxBuild($BuildType) {
    $cmd="-DCMAKE_BUILD_TYPE=$($BuildType)"
    Write-Output $cmd
    Remove-Item ./bin/rtype -Force
    Remove-Item ./build -Force -Recurse
    mkdir build
    Set-Location build
    cmake $cmd ..
    make -j2 rtype
    Set-Location ..
    Set-Location bin
}

function Launch($BuildType) {
    if ($env:OS -ne $null) {
    Write-Output "Windows"
    WindowsBuild $BuildType
    } else {
        Write-Output "Linux"
        LinuxBuild $BuildType
    }
}

$cnt=$args.Count
if ($cnt -ne 1) {
    $BuildType="Debug"
    Write-Output "Build type : $($BuildType)"
} else {
    $BuildType=$args[0]
    Write-Output "Build type : $($BuildType)"
}

Launch $BuildType

