cmake_minimum_required(VERSION 3.15)
project(trader_infra LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

# deps: pthread, rt (for clock), optionally resolv

add_library(common STATIC
  src/common/config.cpp
  src/common/logger.cpp
)

target_include_directories(common PUBLIC src)

add_library(net STATIC
  src/net/tcp_client.cpp
  src/net/udp_receiver.cpp
)

target_include_directories(net PUBLIC src)

target_link_libraries(net PUBLIC common pthread)

add_executable(data_feeder
  src/data_feeder/main.cpp
)

target_link_libraries(data_feeder PRIVATE net common pthread)

add_executable(oms_rms
  src/oms_rms/main.cpp
  src/oms/oms.cpp
  src/rms/rms.cpp
)

target_link_libraries(oms_rms PRIVATE common pthread)

install(TARGETS data_feeder oms_rms RUNTIME DESTINATION bin)

# Tools (optional): TAP mock server and UDP sender
add_executable(tap_mock tools/tap_mock.cpp)
add_executable(udp_send tools/udp_send.cpp)
