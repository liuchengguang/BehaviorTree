//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "ShootRandom.h"
#include "BehaviorTree.h"

ShootRandom::ShootRandom( BehaviorTree *treeOwner ) : Task( treeOwner ), m_transformation( m_behaviorTree->getTransformation( ) ), m_body( m_behaviorTree->getBody( ) )
{

}

ShootRandom::~ShootRandom( )
{

}

void ShootRandom::Start( )
{
    Task::Start( );
}

int ShootRandom::Update( float deltaTime )
{
    return NSTATUS_RUNNING;
}