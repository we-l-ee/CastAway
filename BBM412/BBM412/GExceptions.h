#pragma once
#include <stdexcept>

class GException :public std::runtime_error
{
public:
	GException(std::string msg);

	GException()=default;

	~GException()=default;

};
