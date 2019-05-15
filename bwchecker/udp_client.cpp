//
// blocking_udp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

enum { max_length = 1024 };

int udp_client(const std::string& host, const std::string& port)
{
	using namespace std;
	try
	{
		boost::asio::io_context io_context;

		udp::socket s(io_context, udp::endpoint(udp::v4(), 0));

		udp::resolver resolver(io_context);
		udp::resolver::results_type endpoints =
			resolver.resolve(udp::v4(), host, port);

		const int bitrate = 6000000;
		const int message_size = 1456;
		const int num_messages = 1

		std::vector<char> message_to_send(message_size);
		std::generate(message_to_send.begin(), message_to_send.end(), [c = 0]() mutable { return c++; });

		auto time_prev = chrono::steady_clock::now();
		long time_dev_us = 0;
		const long msgs_per_s = static_cast<long long>(bitrate / 8) / message_size;
		const long msg_interval_us = msgs_per_s ? 1000000 / msgs_per_s : 0;


		for (int i = 0; (cfg.num_messages < 0 || i < cfg.num_messages) && !force_break; ++i)
		{
			if (cfg.bitrate)
			{
				const long duration_us = time_dev_us > msg_interval_us ? 0 : (msg_interval_us - time_dev_us);
				const auto next_time = time_prev + chrono::microseconds(duration_us);
				chrono::time_point<chrono::steady_clock> time_now;
				for (;;)
				{
					time_now = chrono::steady_clock::now();
					if (time_now >= next_time)
						break;
					if (force_break)
						break;
				}

				time_dev_us += (long)chrono::duration_cast<chrono::microseconds>(time_now - time_prev).count() - msg_interval_us;
				time_prev = time_now;
			}

			dst->write(message_to_send);
		}




		std::cout << "Enter message: ";
		char request[max_length];
		std::cin.getline(request, max_length);
		size_t request_length = std::strlen(request);
		s.send_to(boost::asio::buffer(request, request_length), *endpoints.begin());

		char reply[max_length];
		udp::endpoint sender_endpoint;
		size_t reply_length = s.receive_from(
			boost::asio::buffer(reply, max_length), sender_endpoint);
		std::cout << "Reply is: ";
		std::cout.write(reply, reply_length);
		std::cout << "\n";
	}
	catch (std::exception & e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}

