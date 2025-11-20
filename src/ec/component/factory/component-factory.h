#pragma once
#include "ec/component/component.h"

struct IFactory : public IComponent
{
	virtual void create() = 0;
};