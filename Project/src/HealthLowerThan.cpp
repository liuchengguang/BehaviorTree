//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "HealthLowerThan.h"
#include "BehaviorTree.h"

HealthLowerThan::HealthLowerThan( BehaviorTree *treeOwner, bool reevaluate ) : Query( treeOwner, reevaluate )
{
}

HealthLowerThan::~HealthLowerThan( )
{

}

int HealthLowerThan::Update( float deltaTime )
{
    m_healthVal = m_blackboard->GetParameterFloat( "HLWT_val" );
    if ( 100 < m_healthVal )
    {
        return NSTATUS_SUCCESS;
    }
    return NSTATUS_FAIL;
}