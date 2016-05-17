//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "Wander.h"
#include "BehaviorTree.h"
#include "GamePlay.h"

Wander::Wander( BehaviorTree *treeOwner ) : Task( treeOwner ), m_transformation( m_behaviorTree->getTransformation( ) ), m_body( m_behaviorTree->getBody( ) )
, m_angle( (rand( )%360) * 3.1416f / 180.0f )
{
}

Wander::~Wander( )
{
}

void Wander::Start( )
{
    Task::Start( );
    m_velocity = m_blackboard->GetParameterFloat( "WAN_vel" );
    m_change = m_blackboard->GetParameterFloat( "WAN_change" );
    m_timeChangeDirectionMax = ((2.0f + float( rand( )%3 )) / 1000.0f);
    m_timeChangeDirection = 0;
    m_direction = (rand( )%2 == 1 ? 1: -1 );
}

int Wander::Update( float deltaTime )
{
    m_timeChangeDirection += deltaTime;
    if ( m_timeChangeDirection > m_timeChangeDirectionMax )
    {
        m_timeChangeDirectionMax = ((2.0f + float( rand( )%3 )) / 1000.0f);
        m_direction *= -1;
    }

    m_angle = m_angle + m_direction * ((rand( ) % m_change) * 3.1416f / 180.0f);
    glm::vec3 velocity = glm::vec3( ( m_velocity*deltaTime )*sinf( m_angle ), 0, ( m_velocity*deltaTime )*cosf( m_angle ) );
    m_body->SetVelocity( velocity );

    m_status = NSTATUS_RUNNING;
    return m_status;
}

void Wander::onCollision( GameObject *other )
{
    m_angle += 3.1416f;
}