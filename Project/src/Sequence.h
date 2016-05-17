//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#pragma once

#include "NodeBranch.h"

class Sequence : public NodeBranch
{
public:
	Sequence(BehaviorTree *treeOwner);
	~Sequence();

	int Update(float deltaTime);
    void onCollision( GameObject *other );
    void onHit( );
    void onPlayerDeath( );
private:
};