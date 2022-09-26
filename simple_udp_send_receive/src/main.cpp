
#include <common.h>
#include <sender.h>
#include <receiver.h>

using asio::ip::udp;

int main()
{

    auto ip = asio::ip::address::from_string(simple_udp_send_receive::TARGET_IP);
    auto endpoint = udp::endpoint(ip, simple_udp_send_receive::PORT);
    auto recendpoint = udp::endpoint(asio::ip::address_v4::any(), simple_udp_send_receive::PORT);

    try
    {
        asio::io_context io_context;
        udp_subscriber subscriber(io_context, recendpoint);
        udp_publisher publisher(io_context, endpoint);

        while (true)
        {
            io_context.run_one();
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}