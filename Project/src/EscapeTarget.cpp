//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "EscapeTarget.h"
#include "BehaviorTree.h"
#include "GamePlay.h"

EscapeTarget::EscapeTarget( BehaviorTree *treeOwner ) : Task( treeOwner ), m_transformation( m_behaviorTree->getTransformation( ) ), m_body( m_behaviorTree->getBody( ) ), m_repulsion( 20 ), m_repulseTime( 0 ), m_repulseTimeTotal( 1000 ), m_onRepulse( false )
{
}

EscapeTarget::~EscapeTarget( )
{

}

void EscapeTarget::Start( )
{
    Task::Start( );
    m_velocity = m_blackboard->GetParameterFloat( "ESCPT_vel" );
    m_angle = m_blackboard->GetParameterFloat( "ESCPT_angle" );
    m_change = m_blackboard->GetParameterFloat( "ESCPT_change" );
    m_object = m_blackboard->GetParameterGameObj( "ESCPT_obj" );
    m_distance = m_blackboard->GetParameterFloat( "ESCPT_dist" );

    m_repulseTime = 0;
    m_onRepulse = false;
}

int EscapeTarget::Update( float deltaTime )
{
    if ( m_onRepulse == true )
    {
        m_repulseTime += deltaTime;
        if ( m_repulseTime  > m_repulseTimeTotal )
        {
            m_repulseTime = 0;
            m_onRepulse = false;
        }
    }

    m_angle = m_angle + ( ( rand( ) % 2 == 1 ) ? 1 : -1 ) * ( ( rand( ) % m_change ) * 3.1416f / 180.0f );
    glm::vec3 velocity = glm::vec3( ( m_velocity*deltaTime )*sinf( m_angle ), 0, ( m_velocity*deltaTime )*cosf( m_angle ) );

    if ( m_transformation->GetTranslation( ).x < GamePlay::GetGameplay( )->worldLimitLeft )
    {
        velocity = glm::vec3( 1, 0, 0 ) - velocity;
    }
    if ( m_transformation->GetTranslation( ).x > GamePlay::GetGameplay( )->worldLimitRight )
    {
        velocity = glm::vec3( -1, 0, 0 ) - velocity;
    }
    if ( m_transformation->GetTranslation( ).z < GamePlay::GetGameplay( )->worldLimitTop )
    {
        velocity = glm::vec3( 1, 0, 0 ) - velocity;
    }
    if ( m_transformation->GetTranslation( ).z > GamePlay::GetGameplay( )->worldLimitBottom )
    {
        velocity = glm::vec3( -1, 0, 0 ) - velocity;
    }
    Transformation *transformation = dynamic_cast<Transformation*>( m_object->GetComponent( TYPE_Transformation ) );
    float distance = powf( transformation->GetTranslation( ).x - m_transformation->GetTranslation( ).x, 2 ) + powf( transformation->GetTranslation( ).z - m_transformation->GetTranslation( ).z, 2 );
    glm::vec3 playerRepulsion;
    playerRepulsion = glm::normalize( m_transformation->GetTranslation( ) - transformation->GetTranslation( ) );
    playerRepulsion = playerRepulsion * ( m_repulsion * 1 / distance );
    if ( distance < powf( m_distance, 2 ) )
    {
        if ( m_onRepulse == true )
        {
            m_onRepulse = true;
            velocity = playerRepulsion - velocity;
        }
    }
    else
    {
        m_onRepulse = false;
    }
    m_angle = atan2( velocity.x, velocity.z );
    m_body->SetVelocity( velocity );

    return NSTATUS_RUNNING;
}