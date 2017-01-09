#include "GExceptions.h"

GException::GException(std::string msg):runtime_error{msg}
{
}
