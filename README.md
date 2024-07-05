# TCP-CHAT
Пользовательский проект по изучению [boost::asio](https://www.boost.org/doc/libs/master/doc/html/boost_asio.html). Проект представляет мессенджер для переписки между пользователями.
> [!NOTE]
> В проекте не предсумотрено сохранение переписок после отключения от сервера.

## Установка проекта
Для установки проекта вам потребуется библиотека boost:
- [для Windows](https://www.boost.org/doc/libs/master/more/getting_started/windows.html)
- [для Linux или MacOs](https://www.boost.io/doc/user-guide/getting-started.html)
> [!NOTE]
> Проект был написан в Visual Studio Code на MacOs, поэтому дальше будет написано про него. Если используется другие IDE, то смотрите документацию по ним.

### Как Visual Studio
Чтобы можно было запускать проект как в IDE Visual Studio нажатием на кнопку, надо:
- создать в основной каталоге проекта директорию build и в нем две директории Debug и Release
```vim
mkdir build && cd build && mkdir Debug && mkdir Release
```
- создать два файла в основном каталоге (создаются самим Visual Studio Code):
  - _.vscode/launch.json_
  ```json
  {
      "version": "0.2.0",
      "configurations": [
          
          {
              "name": "CMAKE Debug",
              "type": "cppdbg",
              "request": "launch",
              "program": "${workspaceFolder}/build/Debug/${projectName}",
              "args": [],
              "stopAtEntry": false,
              "cwd": "${workspaceFolder}",
              "environment": [],
              "externalConsole": false,
              "preLaunchTask": "build_debug",
              "MIMode": "${compilier}",
              "setupCommands": [
                  {
                      "description": "Включить автоматическое форматирование для gdb",
                      "text": "-enable-pretty-printing",
                      "ignoreFailures": true
                  }
              ]
          },
          {
              "name": "CMAKE Release",
              "type": "cppdbg",
              "request": "launch",
              "program": "${workspaceFolder}/build/Release/${projectName}",
              "args": [],
              "stopAtEntry": false,
              "cwd": "${workspaceFolder}",
              "environment": [],
              "externalConsole": false,
              "preLaunchTask": "build_release",
              "MIMode": "${compilier}",
              "setupCommands": [
                  {
                      "description": "Включить автоматическое форматирование для gdb",
                      "text": "-enable-pretty-printing",
                      "ignoreFailures": true
                  }
              ]
          }
      ]
  }
  ```
  - _.vscode/tasks.json_
  ```json
  {
      "version": "2.0.0",
      "tasks": [
          {
              "label": "build_debug_cmake",
              "type": "shell",
              "command": "cmake",
              "args": [
                  "-DCMAKE_BUILD_TYPE=Debug",
                  "../../"
              ],
              "options": {
                  "cwd": "${workspaceFolder}/build/Debug"
              }
          },
  
          {
              "label": "make_debug",
              "type": "shell",
              "command": "make",
              "options": {
                  "cwd": "${workspaceFolder}/build/Debug"
              }
          },
  
          {
              "label": "build_debug",
              "dependsOn": [
                  "build_debug_cmake",
                  "make_debug"
              ],
              
          },
          {
              "label": "build_release_cmake",
              "type": "shell",
              "command": "cmake",
              "args": [
                  "-DCMAKE_BUILD_TYPE=Release",
                  "../../"
              ],
              "options": {
                  "cwd": "${workspaceFolder}/build/Release"
              }
          },
          {
              "label": "make_release",
              "type": "shell",
              "command": "make",
              "options": {
                  "cwd": "${workspaceFolder}/build/Release"
              }
          },
          {
              "label": "build_release",
              "dependsOn": [
                  "build_release_cmake",
                  "make_release"
              ],
              
          },
      ]
  }
  ```
> [!IMPORTANT]
> Вместо **_${projectName}_** надо прописать имя проекта указанный в **_CmakeLists.txt_**
> 
> Вмеcто **_${compilier}_** надо прописать названия вашего компилятора. Пример, **_lldb_**

> [!TIP]
> Также можно посмотреть [видео](https://www.youtube.com/watch?v=C6zgk25-3AI&list=LL&index=10) по кофигурации проекта

### Настройка cmake файла
Чтобы создать исполняемый файл необходимо создать файлы для сборки. Для этого создаем в основном каталоге проекта **_CmakeLists.txt_**. Пример:
```cmake
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 20)
project(tcp_chat)

find_package(Boost REQUIRED)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

set(SOURCE 
        include/my_chat/session_type.hpp
        include/my_chat/server_type.hpp
        include/server.hpp
        src/server.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE} main.cpp)
target_link_libraries(${PROJECT_NAME} ${BOOST_LIBRARIES})
```
Для подключения библиотеки boost нужны строки:
```cmake
#something code
find_package(Boost REQUIRED)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
#something code
#add_executable(...)
target_link_libraries(${PROJECT_NAME} ${BOOST_LIBRARIES})
```

## Проверка работы программы
- Запускаем исполняемый файл
- Подключаемся к серверу (например, через telnet):
```vim
telnet localhost [port]
```
> [!NOTE]
> Вместо [port] надо написать порт нашего сервера. Пример, _telnet localhost 15500_

- Пишем сообщения :)
