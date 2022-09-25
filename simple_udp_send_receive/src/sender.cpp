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

using asio::ip::udp;

std::string make_daytime_string()
{
    using namespace std; // For time_t, time and ctime;
    time_t now = time(0);
    return ctime(&now);
}

class udp_server
{
public:
    udp_server(asio::io_context& io_context)
        : socket_(io_context, udp::endpoint(udp::v4(), 13))
    {
        start_receive();
    }

private:
    void start_receive()
    {
        socket_.async_receive_from(
            asio::buffer(recv_buffer_), remote_endpoint_,
            std::bind(&udp_server::handle_receive, this,
                std::placeholders::_1,
                std::placeholders::_2));
    }

    void handle_receive(const asio::error_code& error,
        std::size_t /*bytes_transferred*/)
    {
        if (!error)
        {
            std::shared_ptr<std::string> message(
                new std::string(make_daytime_string()));

            socket_.async_send_to(asio::buffer(*message), remote_endpoint_,
                std::bind(&udp_server::handle_send, this, message,
                    std::placeholders::_1,
                    std::placeholders::_2));

            start_receive();
        }
    }

    void handle_send(std::shared_ptr<std::string> /*message*/,
        const asio::error_code& /*error*/,
        std::size_t /*bytes_transferred*/)
    {
    }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 1> recv_buffer_;
};

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
        std::array<char, 1> send_buffer;
        while (thread_flag_)    
        {
            socket_.send_to(asio::buffer(send_buffer), target_endpoint_);
            std::this_thread::sleep_for(std::chrono::seconds(1));
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
        std::size_t /*bytes_transferred*/)
    {
        if (!error)
        {
            std::cout << "got message" << std::endl;
        }
        start_receive();
    }

private:
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 1> recv_buffer_;
};

int main()
{

    auto ip = asio::ip::address::from_string("127.0.0.1");
    auto endpoint = udp::endpoint(ip, 65500);
    auto recendpoint = udp::endpoint(udp::v4(), 65500);


    //try
    //{
        asio::io_context io_context;

        udp_subscriber subscriber(io_context, recendpoint);
        udp_publisher publisher(io_context, endpoint);
        subscriber.start_receive();

        //while (true);
        io_context.run();

    //}
    /*catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }*/

    return 0;
}