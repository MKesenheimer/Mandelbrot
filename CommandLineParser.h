#pragma once
#include <string>

class CommandLineParser
{
public:
	/// liest den string nach dem identifier option, d.h. fuer arg** = -i path wird "path"
	/// zurueckgegeben, wenn option = "-i"
	static char* getCmdOption(char** begin, char ** end, const std::string& option);

	/// true or false, falls die Kommandozeilenoption exisitert
	static bool cmdOptionExists(char** begin, char** end, const std::string& option);

	/// ersetzt \ durch / und // durch /
	static void normalizePath(std::string& path);
};