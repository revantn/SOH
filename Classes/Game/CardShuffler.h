#ifndef _CARDSHUFFLER_H_
#define _CARDSHUFFLER_H_
#include <stdio.h>

const char cardRandomResPack[] = "shuffler.bin";
class CardShuffler
{
public:
	CardShuffler(){};
	~CardShuffler();

	void init();
	//int genRandomNO();
	//bool validate(int card);
	void shuffleCards(int* cardPack);
private:
	//int _atemp[52];
	//int x ;
	//int index;
	//long time_ms ;
	long seekTo;
	unsigned char * sscards;
	static FILE *m_file;





};


#endif