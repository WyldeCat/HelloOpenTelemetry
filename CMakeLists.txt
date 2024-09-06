cmake_minimum_required(VERSION 3.28)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(HelloOpenTelemetry)

add_executable(hello_world src/main.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(protobuf)
find_package(gRPC)
find_package(opentelemetry-cpp CONFIG REQUIRED)


target_link_libraries(hello_world PRIVATE Threads::Threads ${OPENTELEMETRY_CPP_LIBRARIES})
