#pragma once
#include<iostream>

class Utility
{
public:
	Utility();
	~Utility();

	static int GetRand(int min, int max);//生成位于[min,max]之间的随机数
};

