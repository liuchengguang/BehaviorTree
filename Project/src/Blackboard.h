//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#pragma once

#include "GameObject.h"
#include <string>
#include <map>

class Blackboard
{
public:
	Blackboard();
	~Blackboard();

	void SetParameterInt(std::string key, int value);
	void SetParameterFloat(std::string key, float value);
	void SetParameterGameObj(std::string key, GameObject *value);

	int GetParameterInt(std::string key);
	float GetParameterFloat(std::string key);
	GameObject *GetParameterGameObj(std::string key);
private:
	std::map<std::string, void*> m_parameters;
};