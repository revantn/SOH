#include "CardShuffler.h"

//#include<stdio.h>
#include <CCFileUtils.h>
#include <string>
#include <cocos2d.h>
#ifdef _WINDOWS
#include <Windows.h>
#include <chrono>
#include <ctime>

#elif PLATFORM_ANDROID
#include <time.h>
#endif

FILE *CardShuffler::m_file = NULL;

int bytesToInt(unsigned char* b)
{

    int val = 0;

    int j = 0;

	for (int i = 0;  i <sizeof(int); i++)
    {
        val += (b[i] & 0xFF) << (8*j);
    }
    return val;
}

void CardShuffler::init()
{
	//int x = 0;
//#ifdef _GEN_RUNTIME_
//	index = 0;
//	srand(timeGetTime());
//	x = rand() + 1;
//	//MyLog::getLogger()->logIn("initttttttttt %d\n",x);
//	for(int i = 0; i < 52; i++)
//	{
//		_atemp[i] = 1;
//	}
//#else

#ifdef PLATFORM_ANDROID
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	srand(now.tv_nsec);

#else
	auto temp = std::chrono::system_clock::now().time_since_epoch().count();
	srand(temp);
#endif
	//auto x = rand() % 30000;
	//std::string path = cocos2d::FileUtils::getInstance()->fullPathForFilename("shuffler.bin");
	//ssize_t filesize;
	//cocos2d::FileUtils::getInstance()->getFileSize(path);
	//sscards = cocos2d::FileUtils::getInstance()->getFileData(path, "rb", &filesize);
	//path += "shuffler.bin";
	//m_file = NULL;
	/*
#ifdef PLATFORM_ANDROID
	__android_log_print(ANDROID_LOG_DEBUG, "cocos2d-x debug info", "%s", path.c_str());
#endif
	m_file = fopen( path.c_str(), "rb");

	if(!m_file)
	{
#ifdef PLATFORM_ANDROID
	__android_log_print(ANDROID_LOG_DEBUG, "File Not founddddddddddd", "%s", path.c_str());
#endif
		exit(-1);
	}
	long offset = x * 52 * sizeof(int);
	fseek(m_file, offset ,SEEK_SET);
#endif*/
	//long offset = x * 52 * sizeof(int);
	//seekTo = offset;
}
CardShuffler::~CardShuffler()
{
	//fclose(m_file);
}


//bool CardShuffler::validate(int card)
//{
//	
//	if(card < 0)
//		return false;
//	if(_atemp[card] == 1)
//	{
//		_atemp[card] = 0;
//		//MyLog::getLogger()->logIn("\ncardeeeeeeeeeeeee %d\n", card);
//		return true;
//	}
//	return false;
//}

//int CardShuffler::genRandomNO()
//{
//		auto time_ms = timeGetTime();
//		int x = (time_ms * x) % 52;	
//		//if(index == SuitEND )
//		//	return SuitEND;
//		if(validate(x))
//		{
//			return x;
//		}
//		//if( x <= 0)
//		{
//			srand(timeGetTime());
//			x = rand()% 52;	
//			//MyLog::getLogger()->logIn("xxxxxx %d\n", x);
//			return -1;
//		}
//		//index++;
//	//	MyLog::getLogger()->logIn("innnnnnnnnnn %d\n", index);
//		return -1;
//
//}


void CardShuffler::shuffleCards(int *cardPack)
{
#ifdef _GEN_RUNTIME_
	int _iTemp;
	for(int i=0 ;i < SUITEND; i++)
	{
		_iTemp = genRandomNO();

		if(_iTemp != -1)
		{
			*cardPack = _iTemp;
			cardPack++;
			//MyLog::getLogger()->logIn("itempppppppp %d",i);
		}
		else
			i--;

	}
#else
		//int t = fread(cardPack, sizeof(int), 52, m_file);
	auto x = rand() % 30000;
	ssize_t filesize = 0;
	std::string path = cocos2d::FileUtils::getInstance()->fullPathForFilename("shuffler.bin");
	unsigned char* cards = cocos2d::FileUtils::getInstance()->getFileData(path, "rb", &filesize);
	long offset = x * 52 * sizeof(int);
	auto index = 0;
	for (int i = offset; index < 52; ++i)
	{
		//int value = bytesToInt(cards + (i * sizeof(int)), sizeof(int));
#ifdef PLATFORM_ANDROID
	//__android_log_print(ANDROID_LOG_DEBUG, "File Not founddddddddddd", "%d", *(cards + (i * sizeof(int))));
#endif
	cardPack[index] = *(cards + offset + (index * sizeof(int)));
	index++;
	}
	free(cards);
#endif

}
