#include <iostream>
#include <ctime>
#include <string>
#include "rand_string.h"

const int SIZE_CHAR = 11; //生成9 + 1位C Style字符串
const char CCH[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

std::string rand_str()
{	
    srand((unsigned)time(0));
   	char ch[SIZE_CHAR + 1] = {0};
   
   	for (int i = 0; i < SIZE_CHAR; ++i)
   	{
   		int x = rand() / (RAND_MAX / (sizeof(CCH) - 1));
   		ch[i] = CCH[x];
   	}
	
	for (int i = 0; i < SIZE_CHAR; ++i)
	{
		ch[i] = ch[i+1];  	
	}     
    return ch;
}