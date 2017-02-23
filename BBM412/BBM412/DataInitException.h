#pragma once
#include <stdexcept>

class DataInitException :
	public std::runtime_error
{
public:
	DataInitException(const std::string & msg);
	~DataInitException();
};

