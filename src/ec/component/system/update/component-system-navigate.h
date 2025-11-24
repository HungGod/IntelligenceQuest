#pragma once
#include "ec/component/system/component-system.h"
#include "ec/component/component-pathway.h"
#include "ec/component/controller/component-controller.h"

class ANavigate : public ISystem
{
public: 
	virtual void load_new_commands(nlohmann::json n_commands) = 0;
};