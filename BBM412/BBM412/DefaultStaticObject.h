#pragma once
#include "GObject.h"

#include "pStaticObject.h"
#include "GDefaultObject.h"

#include <sstream>

class DefaultStaticObject :
	public GDefaultObject, public pStaticObject
{
public:

	DefaultStaticObject(const string & obj);
	DefaultStaticObject(const string & obj, const glm::vec3 & dis );

	~DefaultStaticObject();
};

