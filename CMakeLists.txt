cmake_minimum_required(VERSION 3.11)
project(simple_udp_send_receive)

add_subdirectory(simple_udp_send_receive/vendor/protobuf)

add_library(protobuf_msgs 
	simple_udp_send_receive/generated/simple_udp_send_receive/addressbook.pb.cc)

target_include_directories(protobuf_msgs PUBLIC
	simple_udp_send_receive/generated/)

target_link_libraries(protobuf_msgs PUBLIC libprotobuf)

add_executable(sender
	simple_udp_send_receive/src/sender.cpp)

target_include_directories(sender PUBLIC
	simple_udp_send_receive/include
	simple_udp_Send_receive/vendor/asio/asio/include)

target_link_libraries(sender PUBLIC libprotobuf protobuf_msgs)

add_executable(receiver
	simple_udp_send_receive/src/receiver.cpp)

target_include_directories(receiver PUBLIC
	simple_udp_send_receive/include)

