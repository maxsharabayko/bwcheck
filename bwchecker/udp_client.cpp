//
// blocking_udp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>
#include <boost/asio.hpp>

#include "config.hpp"
#include "bwutils.hpp"

using boost::asio::ip::udp;

enum { max_length = 1456 };


int udp_client(const std::string& host, const std::string& port,
	const config& cfg, std::atomic_bool &force_break)
{
	using namespace std;
	atomic_bool local_break(false);

	future<void> stats_logger;
	cout << "Time;SendingMbps;SendingBytes\n";

	for (int test_run = 0; test_run <= cfg.test_runs; ++test_run)
	{
		try
		{
			boost::asio::io_context io_context;

			udp::socket s(io_context, udp::endpoint(udp::v4(), 0));

			udp::resolver resolver(io_context);
			udp::resolver::results_type endpoints =
				resolver.resolve(udp::v4(), host, port);

			std::vector<char> message_to_send(cfg.message_size);
			std::generate(message_to_send.begin(), message_to_send.end(), [c = 0]() mutable { return c++; });

			auto time_prev = chrono::steady_clock::now();
			long time_dev_us = 0;
			const long msgs_per_s = static_cast<long long>(cfg.bitrate / 8) / cfg.message_size;
			const long msg_interval_us = msgs_per_s ? 1000000 / msgs_per_s : 0;

			atomic_size_t bytes_snd(0);

			auto stats_func = [&bytes_snd, &force_break, &local_break]()
			{
				auto time_prev = std::chrono::steady_clock::now();
				while (!force_break && !local_break)
				{
					this_thread::sleep_for(chrono::seconds(1));

					if (force_break || local_break)
						break;

					const size_t bytes = bytes_snd.exchange(0);
					const auto time_now = std::chrono::steady_clock::now();

					const auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(time_now - time_prev);
					time_prev = time_now;

					if (bytes == 0)
						continue;

					cout << print_time() << ";";
					cout << std::fixed << std::setprecision(3) << float(bytes * 8) / elapsed_ms.count() / 1000 << ";";
					cout << bytes << "\n" << flush;
				}
			};

			stats_logger = async(launch::async, stats_func);

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

				message_to_send[0] = '0' + i % 74;

				bytes_snd += s.send_to(boost::asio::buffer(message_to_send, message_to_send.size()), *endpoints.begin());
			}

		}
		catch (std::exception & e)
		{
			std::cerr << "Exception: " << e.what() << "\n";
		}

		local_break = true;
		stats_logger.wait();
		local_break = false;

		this_thread::sleep_for(chrono::seconds(cfg.test_run_interval_s));
	}

	return 0;
}

