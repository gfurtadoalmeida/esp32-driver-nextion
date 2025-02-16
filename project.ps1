$ProjectFolder = $PSScriptRoot
$EspIdfDockerImage = 'gfurtadoalmeida/esp32-docker-sonar:v5.4'

switch ($args[0]) {
    'build' {
        &docker.exe run --rm --env LC_ALL='C.UTF-8' -v ${ProjectFolder}:/project -w /project ${EspIdfDockerImage} idf.py build
    }
    'build-test' {
        &docker.exe run --rm --env LC_ALL='C.UTF-8' -v ${ProjectFolder}:/project -w /project ${EspIdfDockerImage} idf.py build -C ./test
    }
    'clean' {
        &docker.exe run --rm --env LC_ALL='C.UTF-8' -v ${ProjectFolder}:/project -w /project ${EspIdfDockerImage} idf.py fullclean
    }
    'clean-test' {
        &docker.exe run --rm --env LC_ALL='C.UTF-8' -v ${ProjectFolder}:/project -w /project ${EspIdfDockerImage} idf.py fullclean -C ./test
    }
    Default {
        Write-Host "Command not recognized. Valid commands:"
        Write-Host "`t* build: build the main project"
        Write-Host "`t* build-test: build the test project"
        Write-Host "`t* clean: clean the main project build files"
        Write-Host "`t* clean-test: clean the test project build files"
    }
}
