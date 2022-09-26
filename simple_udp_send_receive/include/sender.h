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

        std::vector<char> send_buffer(person.ByteSizeLong());
        person.SerializeToArray(send_buffer.data(), person.ByteSizeLong());

        while (thread_flag_)
        {
            socket_.send_to(asio::buffer(send_buffer), target_endpoint_);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
