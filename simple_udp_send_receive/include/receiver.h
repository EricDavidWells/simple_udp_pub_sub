#pragma once

#include <ctime>
#include <iostream>
#include <string>
#include <array>
#include <chrono>
#include <asio.hpp>
#include <thread>

#include <common.h>
#include <addressbook.pb.h>

using asio::ip::udp;

class udp_subscriber
{
public:
    udp_subscriber(asio::io_context& io_context, udp::endpoint& endpoint) : socket_(io_context, endpoint)
    {
        //start_receive();
        thread_flag_.store(true);
        thread_ = std::thread([this]() {receive_loop(); });
    }

    void receive_loop()
    {
        tutorial::Person person;
        while (thread_flag_)
        {
            auto nbytes = socket_.receive_from(asio::buffer(recv_buffer_), remote_endpoint_);
            person.ParseFromArray(recv_buffer_.data(), nbytes);
            person.PrintDebugString();
        }

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
