cmake_minimum_required(VERSION 3.11)
project(simple_udp_send_receive)

find_package(protobuf CONFIG REQUIRED)

file(GLOB ProtoFiles "${CMAKE_CURRENT_LIST_DIR}/simple_udp_send_receive/*.proto")
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${ProtoFiles})

add_library(protobuf_msgs 
	${ProtoSources} ${ProtoHeaders})

target_include_directories(protobuf_msgs PUBLIC
	${CMAKE_BINARY_DIR})

target_link_libraries(protobuf_msgs PUBLIC protobuf::libprotobuf)

add_executable(sender
	simple_udp_send_receive/src/sender.cpp)

target_include_directories(sender PUBLIC
	simple_udp_send_receive/include
	simple_udp_send_receive/vendor/asio/asio/include)

target_link_libraries(sender PUBLIC protobuf::libprotobuf protobuf_msgs)

add_executable(receiver
	simple_udp_send_receive/src/receiver.cpp)

target_include_directories(receiver PUBLIC
	simple_udp_send_receive/include
	simple_udp_send_receive/vendor/asio/asio/include)

target_link_libraries(receiver PUBLIC protobuf::libprotobuf protobuf_msgs)

add_executable(main
	simple_udp_send_receive/src/main.cpp)

target_include_directories(main PUBLIC
	simple_udp_send_receive/include
	simple_udp_send_receive/vendor/asio/asio/include)

target_link_libraries(main PUBLIC protobuf::libprotobuf protobuf_msgs)
