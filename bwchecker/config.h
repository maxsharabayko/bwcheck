struct config
{
	int bitrate						= 0;
	int num_messages				= 60;
	int message_size				= 1316; ////8 * 1024 * 1024;
	int stats_freq_ms				= 0;
	bool two_way					= false;
	std::string stats_file;
};