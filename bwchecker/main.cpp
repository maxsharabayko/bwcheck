#include <atomic>
#include <signal.h>
// Third party libraries

#include "CLI/CLI.hpp"

#include "config.hpp"

int tcp_server(const std::string& port);
int tcp_client(const std::string& host, const std::string& port);
int udp_server(const std::string& port);
int udp_client(const std::string& host, const std::string& port,
	const config& cfg, std::atomic_bool& force_break);

// Example implementation
// https://stackoverflow.com/questions/40831436/cant-explain-poor-bandwidth-performance-using-boost-asio-tcp-sockets


std::atomic_bool force_break(false);


void OnINT_ForceExit(int)
{
	std::cerr << "\n-------- REQUESTED INTERRUPT!\n";
	force_break = true;
}


int main(int argc, char** argv) {

	CLI::App app("SRT xtransmit tool.");
	app.set_help_all_flag("--help-all", "Expand all help");
	CLI::Option* use_tcp = app.add_flag("--tcp", "Use TCP");

	std::string host;
	std::string port;
	config cfg;
	CLI::App* sc_client = app.add_subcommand("client", "Connect to the server")->fallthrough();
	sc_client->add_option("host", host, "Host");
	sc_client->add_option("port", port, "Port");
	sc_client->add_option("--msgsize", cfg.message_size, "Destination URI");
	sc_client->add_option("--bitrate", cfg.bitrate, "Bitrate to generate");
	sc_client->add_option("--num", cfg.num_messages, "Number of messages to send (-1 for infinite)");
	CLI::App* sc_server = app.add_subcommand("server", "Start server")->fallthrough();
	sc_server->add_option("port", port, "Port");

	app.require_subcommand(1);

	CLI11_PARSE(app, argc, argv);

	signal(SIGINT,  OnINT_ForceExit);
	signal(SIGTERM, OnINT_ForceExit);

	// TODO: Callback for subcommands
	// https://cliutils.gitlab.io/CLI11Tutorial/chapters/an-advanced-example.html
	if (sc_client->parsed())
	{
		if (*use_tcp)
			return tcp_client(host, port);
		return udp_client(host, port, cfg, force_break);
	}
	
	if (sc_server->parsed())
	{
		if (*use_tcp)
			return tcp_server(port);

		return udp_server(port);
	}

	return 0;
}





