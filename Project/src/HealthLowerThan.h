//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#pragma once

#include "Query.h"
#include "GameObject.h"
#include "Transformation.h"

class HealthLowerThan : public Query
{
public:
    HealthLowerThan( BehaviorTree *treeOwner, bool reevaluate );
    ~HealthLowerThan( );

    int Update( float deltaTime );
private:
    float m_healthVal;
};