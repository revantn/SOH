#ifndef __HELLOWORLD_FILE_OPERATION__
#define __HELLOWORLD_FILE_OPERATION__

#include <string>

class FileOperation 
{
public:
	static void saveFile(char* buffer);
	static void readFile(char* buffer);
	static std::string getFilePath();
};

#endif
