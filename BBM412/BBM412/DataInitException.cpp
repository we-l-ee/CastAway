#include "DataInitException.h"



DataInitException::DataInitException(const std::string & msg):
	runtime_error(msg)
{
}


DataInitException::~DataInitException()
{
}
