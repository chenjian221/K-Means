set(PROJECT_NAME Kmeans)
project(${PROJECT_NAME})
set(CMAKE_C_FLAGS "-std=c99")
add_executable(${PROJECT_NAME} kmeans.c)