/*************************************************************************
> File Name: Utility.h
> Project Name: Hearthstone++
> Author: Young-Joong Kim
> Purpose: Utility for Hearthstone Game Agent
> Created Time: 2017/10/04
> Copyright (c) 2017, Young-Joong Kim
*************************************************************************/
#ifndef HEARTHSTONEPP_UTILITY_H
#define HEARTHSTONEPP_UTILITY_H

#include <Windows.h>

#include <mutex>
#include <random>

namespace Hearthstonepp
{
	class RandomInt
	{
	public:
		RandomInt(int num);
		int get();

	private:
		std::default_random_engine gen;
		std::uniform_int_distribution<int> dist;
	};

	class InteractBuffer
	{
	public:
		InteractBuffer();

		int ReadBuffer(BYTE *arr, int maxLength);
		int WriteBuffer(BYTE *arr, int size);

	private:
		HANDLE hRead, hWrite;
	};
}

#endif HEARTHSTONEPP_UTILITY_H