#pragma once
#include "ec/component/component.h"

struct ISystem : IComponent
{
	virtual void execute() = 0;
};