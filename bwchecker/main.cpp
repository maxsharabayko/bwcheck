// Third party libraries
#include "CLI/CLI.hpp"

int tcp_server(const std::string& port);
int tcp_client(const std::string& host, const std::string& port);

// Example implementation
// https://stackoverflow.com/questions/40831436/cant-explain-poor-bandwidth-performance-using-boost-asio-tcp-sockets


int main(int argc, char** argv) {

	CLI::App app("SRT xtransmit tool.");
	app.set_help_all_flag("--help-all", "Expand all help");

	std::string host;
	std::string port;
	CLI::App* sc_client = app.add_subcommand("client", "Connect to the server");
	sc_client->add_option("host", host, "Host");
	sc_client->add_option("port", port, "Port");
	CLI::App* sc_server = app.add_subcommand("server", "Start server");
	sc_server->add_option("port", port, "Port");

	app.require_subcommand(1);

	CLI11_PARSE(app, argc, argv);

	// TODO: Callback for subcommands
	// https://cliutils.gitlab.io/CLI11Tutorial/chapters/an-advanced-example.html
	if (sc_client->parsed())
	{
		return tcp_client(host, port);
	}
	
	if (sc_server->parsed())
	{
		return tcp_server(port);
	}
	
	return 0;
}





