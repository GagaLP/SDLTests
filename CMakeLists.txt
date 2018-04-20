cmake_minimum_required(VERSION 3.7)
project(SDLTests)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SDLTests_SOURCE_DIR}/cmake")


#include_directories(/usr/local/include)
#link_libraries(/usr/local/lib)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-I/usr/local/include -L/usr/local/lib -lsdl2 -lsdl2_ttf -lsdl2_image -lsdl2_mixer")

#set(SOURCE_FILES main.c OtherTests.c)

add_executable(KeyImputTests KeyImputTests.c ${SOURCE_FILES})
add_executable(ObjectsInC ObjectsInC.c ${SOURCE_FILES})
add_executable(Test Test.c ${SOURCE_FILES})
add_executable(Animation Animation.c ${SOURCE_FILES})
add_executable(Rauschen Rauschen.c ${SOURCE_FILES})
add_executable(MoveObjects MoveObjects.c ${SOURCE_FILES})
add_executable(MoveObjectsBetter MoveObjectsBetter.c ${SOURCE_FILES})
add_executable(Messageboxes Messageboxes.c ${SOURCE_FILES})
add_executable(ShowText ShowText.c ${SOURCE_FILES})
add_executable(playMPThree playMPThree.c ${SOURCE_FILES})
add_executable(PlayWavFile PlayWavFile.c ${SOURCE_FILES})
add_executable(TextInputEvent TextInputEvent.c ${SOURCE_FILES})

#target_link_libraries(Main ${SDL2_LIBRARY})
#target_link_libraries(OtherTests ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})
#target_link_libraries(Main)
#target_link_libraries(OtherTests)
#target_link_libraries(OtherTests ${SDL2_LIBRARY})


