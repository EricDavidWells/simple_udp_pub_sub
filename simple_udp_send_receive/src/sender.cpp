//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
#include <array>
#include <chrono>
#include <asio.hpp>
#include <thread>

#include <simple_udp_send_receive/addressbook.pb.h>

using asio::ip::udp;

class udp_publisher
{
public:
    udp_publisher(asio::io_context& io_context, udp::endpoint& endpoint) : socket_(io_context), target_endpoint_(endpoint)
    {
        socket_.open(udp::v4());
        thread_flag_.store(true);
        thread_ = std::thread([this]() {publish_loop(); });
    }

    void publish_loop()
    {

        tutorial::Person person;
        person.set_name("eric");

        std::vector<char> send_buffer (person.ByteSizeLong());
        person.SerializeToArray(send_buffer.data(), person.ByteSizeLong());

        while (thread_flag_)    
        {
            socket_.send_to(asio::buffer(send_buffer), target_endpoint_);
        }
    }

    ~udp_publisher()
    {
        thread_flag_.store(false);
        thread_.join();
    }

private:
    udp::socket socket_;
    std::thread thread_;
    std::atomic<bool> thread_flag_;
    udp::endpoint target_endpoint_;
};

class udp_subscriber
{
public:
    udp_subscriber(asio::io_context& io_context, udp::endpoint& endpoint) : socket_(io_context, endpoint)
    {
    }

    void start_receive()
    {
        socket_.async_receive_from(
            asio::buffer(recv_buffer_), remote_endpoint_,
            std::bind(&udp_subscriber::handle_receive, this,
                std::placeholders::_1,
                std::placeholders::_2));
    }

    void handle_receive(const asio::error_code& error,
        std::size_t nbytes)
    {
        if (!error)
        {
            tutorial::Person person;
            person.ParseFromArray(recv_buffer_.data(), nbytes);

            std::cout << "person name: " << person.name() << std::endl;
        }
        else
        {
            std::cout << "got error: " << error << std::endl;
        }
        start_receive();
    }

    ~udp_subscriber()
    {
        thread_flag_.store(false);
        thread_.join();
    }

private:
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 1000> recv_buffer_;
    std::thread thread_;
    std::atomic<bool> thread_flag_;
};

int main()
{

    std::string target_ip{ "127.0.0.1" };
    int port = 65500;

    auto ip = asio::ip::address::from_string(target_ip);
    auto endpoint = udp::endpoint(ip, port);
    auto recendpoint = udp::endpoint(asio::ip::address_v4::any(), port);

    try
    {
        asio::io_context io_context;
        udp_subscriber subscriber(io_context, recendpoint);
        udp_publisher publisher(io_context, endpoint);
        subscriber.start_receive();
        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}