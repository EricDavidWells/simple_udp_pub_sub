cmake_minimum_required(VERSION 3.11)
project(simple_udp_send_receive)


add_executable(sender
	simple_udp_send_receive/src/sender.cpp)

target_include_directories(sender PUBLIC
	simple_udp_send_receive/include
	simple_udp_Send_receive/vendor/asio/asio/include)

add_executable(receiver
	simple_udp_send_receive/src/receiver.cpp)

target_include_directories(receiver PUBLIC
	simple_udp_send_receive/include)
