//
// blocking_udp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <thread>

#include <boost/asio.hpp>

#include "bwutils.hpp"

using namespace std;
using boost::asio::ip::udp;

enum { max_length = 1456 };

void server(boost::asio::io_context& io_context, unsigned short port, const atomic_bool& force_break)
{
	udp::socket sock(io_context, udp::endpoint(udp::v4(), port));
	atomic_bool local_break(false);
	atomic_size_t bytes_rcvd(0);

	auto stats_func = [&bytes_rcvd, &force_break, &local_break]()
	{
		cout << "Time;ReceivingMbps;ReceivedBytes";
		auto time_prev = std::chrono::steady_clock::now();
		while (!force_break && !local_break)
		{
			this_thread::sleep_for(chrono::seconds(1));

			if (force_break || local_break)
				break;

			const size_t bytes = bytes_rcvd.exchange(0);
			const auto time_now = std::chrono::steady_clock::now();
			
			const auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(time_now - time_prev);
			time_prev = time_now;

			if (bytes == 0)
				continue;

			cout << print_time() << ";";
			cout << std::fixed << std::setprecision(3) << float(bytes * 8) / elapsed_ms.count() / 1000 << ";";
			cout << bytes << "\n";
		}
	};

	auto stats_logger = async(launch::async, stats_func);

	for (;;)
	{
		char data[max_length];
		udp::endpoint sender_endpoint;
		const size_t length = sock.receive_from(
			boost::asio::buffer(data, max_length), sender_endpoint);

		bytes_rcvd += length;
	}

	local_break = true;
	stats_logger.wait();
}

int udp_server(const std::string& port, const atomic_bool& force_break)
{
	try
	{
		boost::asio::io_context io_context;

		server(io_context, stoi(port), force_break);
	}
	catch (std::exception & e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}

