#pragma once
#include <exception>

class EmuException : std::exception
{
public:
	EmuException();
	~EmuException();
};

