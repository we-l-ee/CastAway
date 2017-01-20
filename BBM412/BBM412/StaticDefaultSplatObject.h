#pragma once
#include "GDefaultSplatObject.h"
#include "pStaticObject.h"

class StaticDefaultSplatObject :
	public GDefaultSplatObject, public pStaticObject
{
public:

	StaticDefaultSplatObject(const string & obj);
	StaticDefaultSplatObject(const string & obj, const glm::vec3 & dis);

	~StaticDefaultSplatObject();
};

