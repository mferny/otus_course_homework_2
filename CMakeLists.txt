cmake_minimum_required(VERSION 3.19)

if(DEFINED ENV{GITHUB_RUN_NUMBER})
    project(otus_homework_2 VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
else()
    project(otus_homework_2)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")

set(SRC_MAIN main.cpp)
add_executable(otus_homework_2 ${SRC_MAIN})

install(TARGETS otus_homework_2 RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT timoshenkonas@gmail.com)

include(CPack)
