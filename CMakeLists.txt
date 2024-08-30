project(HelloOpenTelemetry)
cmake_minimum_required(VERSION 3.28)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(opentelemetry-cpp CONFIG REQUIRED)

add_executable(hello_world src/main.cpp)

target_link_libraries(hello_world PRIVATE Threads::Threads ${OPENTELEMETRY_CPP_LIBRARIES})
