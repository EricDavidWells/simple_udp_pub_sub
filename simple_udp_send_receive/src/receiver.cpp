#include <receiver.h>

using asio::ip::udp;

int main()
{

    auto ip = asio::ip::address::from_string(simple_udp_send_receive::TARGET_IP);
    auto recendpoint = udp::endpoint(asio::ip::address_v4::any(), simple_udp_send_receive::PORT);

    try
    {
        asio::io_context io_context;
        udp_subscriber subscriber(io_context, recendpoint);

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