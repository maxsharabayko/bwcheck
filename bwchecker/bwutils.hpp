#pragma once


inline std::string print_time()
{
	using namespace std;
	time_t time = chrono::system_clock::to_time_t(chrono::system_clock::now());
	tm* tm = localtime(&time);
	time_t usec = time % 1000000;

	char tmp_buf[512];
#ifdef _WIN32
	strftime(tmp_buf, 512, "%T.", tm);
#else
	strftime(tmp_buf, 512, "%T.", tm);
#endif
	ostringstream out;
	out << tmp_buf << setfill('0') << setw(6) << usec << " ";
	return out.str();
}
