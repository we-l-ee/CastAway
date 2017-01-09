#pragma once
#include "GDirecLight.h"
#include "GExceptions.h"

#include <fstream>
#include <iostream>

class Sun :
	public GDirecLight
{

public:
	Sun();

	virtual void calculateLightening() override;

	~Sun();
};

