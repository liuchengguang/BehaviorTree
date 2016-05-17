//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "ChaseRanged.h"
#include "BehaviorTree.h"
#include "GamePlay.h"
#include "GameObjectFactory.h"
#include "ArtificialIntelligence.h"

ChaseRanged::ChaseRanged( BehaviorTree *treeOwner ) : Task( treeOwner ), m_transformation( m_behaviorTree->getTransformation( ) ), m_body( m_behaviorTree->getBody( ) )
, m_state( STATE_CHASE ), m_time( 0 ), m_timeRecover( 0 ), m_onWaitRepath( false ), m_onPlayerDeath( false ), m_waitTryRepath( 0 )
{
}

ChaseRanged::~ChaseRanged( )
{
}

void ChaseRanged::Start( )
{
    Task::Start( );
    m_velocity = m_blackboard->GetParameterFloat( "ESCPT_vel" ) *1.25f ;
    m_angle = m_blackboard->GetParameterFloat( "ESCPT_angle" );
    m_change = m_blackboard->GetParameterFloat( "ESCPT_change" );
    m_object = m_blackboard->GetParameterGameObj( "ESCPT_obj" );
    m_distance = m_blackboard->GetParameterFloat( "ESCPT_dist" );
    m_timeMax = m_blackboard->GetParameterFloat( "ESCPT_time" );
	m_timeRecoverMax = m_blackboard->GetParameterFloat("ESCPT_recover") * 0.75;
    m_state = STATE_CHASE;
    m_onPlayerDeath = false;
}

int ChaseRanged::Update( float deltaTime )
{
    if ( m_onPlayerDeath )
    {
        return NSTATUS_FAIL;
    }

    if ( m_onWaitRepath )
    {
        m_waitRePathTime -= deltaTime;
        if ( m_waitRePathTime <0 )
        {
            m_state = STATE_CHASE;
            m_onWaitRepath = false;
            setTargetPath( );
        }
    }

    glm::vec3 velocity;
    Transformation *transformation = dynamic_cast<Transformation*>( m_object->GetComponent( TYPE_Transformation ) );
    float distance = powf( transformation->GetTranslation( ).x - m_transformation->GetTranslation( ).x, 2 ) + powf( transformation->GetTranslation( ).z - m_transformation->GetTranslation( ).z, 2 );
    glm::vec3 playerRepulsion;
    playerRepulsion = glm::normalize( m_transformation->GetTranslation( ) - transformation->GetTranslation( ) );
    playerRepulsion.y = 0;

    if ( m_state == STATE_CHASE )
    {
        if ( m_path.size( ) != 0 )
        {
            if ( abs( currentTarget.x - m_transformation->GetTranslation( ).x ) < 0.5 && abs( currentTarget.z - m_transformation->GetTranslation( ).z ) < 0.5 )
            {
                currentTarget = m_path.front( );
                m_path.pop_front( );

                glm::vec3 ownPos( m_transformation->GetTranslation( ).x, 0, m_transformation->GetTranslation( ).z );
                glm::vec3 targetPos( currentTarget.x, 0, currentTarget.z );
                glm::vec3 result = targetPos - ownPos;
                result = glm::normalize( result );
                result.y = 0;
                m_body->SetVelocity( result*m_velocity*deltaTime );
            }

            m_waitTryRepath += deltaTime;
            if ( m_waitTryRepath > WAIT_TRY_REPATH )
            {
                m_waitTryRepath = 0;
                if ( (powf( m_targetPosition.x - transformation->GetTranslation( ).x, 2 ) + powf( m_targetPosition.y - transformation->GetTranslation( ).z, 2 )) > powf( 10, 2 ) )
                {
                    m_state = STATE_CHASE;
                    m_onWaitRepath = false;
                    setTargetPath( );
                }
            }
        }
        else
        {
            if ( !m_onWaitRepath )
            {
                m_onWaitRepath = true;
                m_waitRePathTime = 1;
            }
        }

        if ( distance < powf( m_distance, 2 ) )
        {
            velocity = playerRepulsion;
            velocity.y = 0;
            m_body->SetVelocity( velocity*m_velocity*deltaTime );
            m_angle = atan2( velocity.x, velocity.z );

            GameObject *snowball = GameObjectFactory::GetGameObjectFactory( )->CreateFromArchetype( "EnemySnowball" );
            Transformation *transformationBall = dynamic_cast<Transformation*>(snowball->GetComponent( TYPE_Transformation ));
            Body *bodyBall = dynamic_cast<Body*>(snowball->GetComponent( TYPE_Body ));
            transformationBall->SetTranslation( glm::vec3( m_transformation->GetTranslation( ).x - playerRepulsion.x*4, 5, m_transformation->GetTranslation( ).z - playerRepulsion.z*4 ) );
            glm::vec3 ballSpeed = glm::vec3( -playerRepulsion.x*20, 0, -playerRepulsion.z*20 );
            bodyBall->SetVelocity( ballSpeed );
            m_body->SetVelocity( ballSpeed/20.0f );
            m_stateLast = m_state;
            m_state = STATE_RETREAT;
        }
    }

    if ( m_state == STATE_RETREAT )
    {
        m_time += deltaTime;
        if ( m_time > m_timeMax )
        {
            m_state = STATE_CHASE;
            m_time = 0;
        }

        m_angle = m_angle + ((rand( ) % 2 == 1) ? 1 : -1) * ((rand( ) % m_change) * 3.1416f / 180.0f);
        velocity = glm::normalize( glm::vec3( (m_velocity*deltaTime)*sinf( m_angle ), 0, (m_velocity*deltaTime)*cosf( m_angle ) ) );
        velocity.y = 0;
        m_body->SetVelocity( velocity*m_velocity*deltaTime );
    }

    return NSTATUS_RUNNING;
}

void ChaseRanged::setTargetPath( )
{
    Transformation *transformation = dynamic_cast<Transformation*>( m_object->GetComponent( TYPE_Transformation ) );
    m_targetPosition = glm::vec2( transformation->GetTranslation( ).x, transformation->GetTranslation( ).z );
    glm::vec2 ownRowCol = ArtificialIntelligence::GetAI( )->pathfinder.ConvertToMap( m_transformation->GetTranslation( ).x, m_transformation->GetTranslation( ).z );
    glm::vec2 targetRowCol = ArtificialIntelligence::GetAI( )->pathfinder.ConvertToMap( transformation->GetTranslation( ).x, transformation->GetTranslation( ).z );
    m_path = ArtificialIntelligence::GetAI( )->pathfinder.FindPath( ownRowCol.x, ownRowCol.y, targetRowCol.x, targetRowCol.y );

    currentTarget = m_path.front( );
    m_path.pop_front( );

    glm::vec3 ownPos( m_transformation->GetTranslation( ).x, 0, m_transformation->GetTranslation( ).z );
    glm::vec3 targetPos( currentTarget.x, 0, currentTarget.z );
    glm::vec3 result = targetPos - ownPos;
    result = glm::normalize( result );
    result.y = 0;
    m_body->SetVelocity( result );
    m_onWaitRepath = false;
}

void ChaseRanged::onCollision( GameObject *other )
{
    if ( other->GetTag( ).compare( "Player" ) != 0 )
    {
        Transformation* otherTrans = dynamic_cast<Transformation*>(other->GetComponent( TYPE_Transformation ));
        glm::vec3 velocity = glm::vec3( m_transformation->GetTranslation( ).x - otherTrans->GetTranslation( ).x, 0, m_transformation->GetTranslation( ).z - otherTrans->GetTranslation( ).z );
        m_body->SetVelocity( velocity*1.0f );
    }

    if ( m_state == STATE_CHASE )
    {
        m_onWaitRepath = true;
        m_waitRePathTime = rand( )%3 + 1;
    }
}

void ChaseRanged::onHit(  )
{
    if ( m_state == STATE_CHASE )
    {
        m_onWaitRepath = true;
        m_waitRePathTime = rand( )%2 + 1;
    }
}

void ChaseRanged::onPlayerDeath( )
{
    m_onPlayerDeath = true;
}