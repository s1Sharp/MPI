cmake_minimum_required(VERSION 3.9.1)
project(MPI)

set(CMAKE_CXX_STANDARD 14)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic -lm -O3 -funroll-loops")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

set(EXE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/executable")
set(EXECUTABLE_OUTPUT_PATH  ${EXE_DIR})

# OpenMP & MPI
find_package(OpenMP)
find_package(MPI)

#target_link_libraries(task1 PUBLIC OpenMP::OpenMP_CXX)

add_executable(task1 task1.cpp)
target_link_libraries(task1 PUBLIC MPI::MPI_CXX)

add_executable(task2 task2.cpp)
target_link_libraries(task2 PUBLIC MPI::MPI_CXX)

add_executable(task3 task3.cpp)
target_link_libraries(task3 PUBLIC MPI::MPI_CXX)

add_executable(task4 task4.cpp)
target_link_libraries(task4 PUBLIC MPI::MPI_CXX)

add_executable(task5 task5.cpp)
target_link_libraries(task5 PUBLIC MPI::MPI_CXX)

add_executable(task6 task6.cpp)
target_link_libraries(task6 PUBLIC MPI::MPI_CXX)

add_executable(task7 task7.cpp)
target_link_libraries(task7 PUBLIC MPI::MPI_CXX)

add_executable(task8 task8.cpp)
target_link_libraries(task8 PUBLIC MPI::MPI_CXX)

add_executable(task9 task9.cpp)
target_link_libraries(task9 PUBLIC MPI::MPI_CXX)

add_executable(task10 task10.cpp)
target_link_libraries(task10 PUBLIC MPI::MPI_CXX)

add_executable(task11 task11.cpp)
target_link_libraries(task11 PUBLIC MPI::MPI_CXX)