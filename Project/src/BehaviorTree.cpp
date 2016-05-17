//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "Selector.h"
#include "Sequence.h"
#include "WalkToPosition.h"
#include "Wander.h"
#include "Inverter.h"
#include "NodeBlackboard.h"

#include "GameObject.h"
#include "ArtificialIntelligence.h"
#include "BehaviorTree.h"
#include "Task.h"
#include "Query.h"
#include <iostream>

#include "GamePlay.h"
#include "ScriptingSystem.h"
#include "ControlPoint.h"

BehaviorTree::BehaviorTree() : m_baseBranch(NULL), onStop(false)
{
	ArtificialIntelligence::GetAI()->PushBehaviorTree(this);
}

BehaviorTree::~BehaviorTree( )
{
	ArtificialIntelligence::GetAI()->RemoveBehaviorTree(this);
    delete m_baseBranch;
}

Blackboard *BehaviorTree::getBlackboard( )
{
    return &m_blackboard;
}

void BehaviorTree::Initialize( )
{
    m_transformation = dynamic_cast<Transformation*>(GetGameObject( )->GetComponent( TYPE_Transformation ));
    m_body = dynamic_cast<Body*>(GetGameObject( )->GetComponent( TYPE_Body ));
    m_currentBranch = NULL;

    m_players = ArtificialIntelligence::GetAI()->getPlayers();
	GameObject *player = NULL;
	if (m_players.size() > 0)
	{
        player = m_players.front( );
	}
    m_controlPoints = ArtificialIntelligence::GetAI( )->getControlPoints( );

	m_blackboard.SetParameterFloat( "NEO_dist", 15 );
	m_blackboard.SetParameterGameObj( "NEO_obj", player );
    m_blackboard.SetParameterFloat( "WTP_posX", 0 );
    m_blackboard.SetParameterFloat( "WTP_posZ", -20 );
    m_blackboard.SetParameterFloat( "WAN_vel", 350 );
    m_blackboard.SetParameterFloat( "WAN_angle", 0 );
    m_blackboard.SetParameterFloat( "WAN_change", 30 );
    m_blackboard.SetParameterFloat( "ESCPT_angle", 0 );
    m_blackboard.SetParameterFloat( "ESCPT_change", 10 );
    m_blackboard.SetParameterFloat( "ESCPT_vel", 350 );
    m_blackboard.SetParameterFloat( "ESCPT_dist", 10 );
    m_blackboard.SetParameterFloat( "ESCPT_time", 3 );
    m_blackboard.SetParameterFloat( "ESCPT_recover", 1 );
    m_blackboard.SetParameterFloat( "HLWT_val", 50 );
    m_blackboard.SetParameterFloat( "REPT_val", 50 );
    m_blackboard.SetParameterFloat( "REP_val", 3 );
	m_blackboard.SetParameterGameObj( "MLE_obj", player );
    m_blackboard.SetParameterFloat( "MLE_vel", 350 );
    m_blackboard.SetParameterFloat( "MLE_dist", 10 );
    m_blackboard.SetParameterFloat( "MLE_time", 3 );
    m_blackboard.SetParameterFloat( "MLE_recover", 1 );
    m_blackboard.SetParameterGameObj( "CON_obj", player );
    m_blackboard.SetParameterFloat( "CON_vel", 350 );
}

std::list<GameObject* > BehaviorTree::GetPlayers( )
{
    m_players = ArtificialIntelligence::GetAI( )->getPlayers( );
    return m_players;
}

void BehaviorTree::setBaseBranch ( Node *baseNode )
{
    m_baseBranch = baseNode;
}

void BehaviorTree::Update( float deltaTime )
{
	if (onStop)
	{
		return;
	}

    if ( m_baseBranch == NULL )
    {
        return;
    }

    int status;
    if ( m_currentBranch == NULL )
    {
        m_currentBranch = m_baseBranch;
    }
    else
    {
        status = m_currentBranch->Update( deltaTime );
        switch ( status )
        {
        case NSTATUS_RUNNING:
            break;
        case NSTATUS_FAIL:
        case NSTATUS_SUCCESS:
            Node *branchParent = m_currentBranch->GetParent( );
            if ( branchParent == NULL )
            {
                m_currentBranch = NULL;
            }
            else
            {
                m_currentBranch = branchParent;
            }
            break;
        }
    }
}

void BehaviorTree::UpdateBranchPointer( Node *nodeBranch )
{
    m_currentBranch = nodeBranch;
}

void BehaviorTree::SetOnAttackMeleeSlot( std::string const &_script, std::string const &function )
{
    onAttackMeleeSlot = function;
    script = _script;
}

void BehaviorTree::OnAttackMelee( )
{
    if ( !onAttackMeleeSlot.empty( ) )
    {
        ScriptingSystem::scriptingSystem->CallFunction( "CallMethod", "uss>", GetGameObject( ), onAttackMeleeSlot.c_str( ), script.c_str( ) );
    }
}

void BehaviorTree::setEnemyControlPoint( float deltaTime )
{
    if ( m_currentBranch != NULL && ((NodeBranch *)m_currentBranch)->GetCurrentNode( ) != NULL )
    {
        ((ControlPoint *)((NodeBranch *)m_currentBranch)->GetCurrentNode( ))->onEnemyControl( deltaTime );
    }
}

ControlPoint* BehaviorTree::GetControlPoint() const{
	if (m_currentBranch == nullptr)
		return nullptr;
	return ((ControlPoint *)((NodeBranch *)m_currentBranch)->GetCurrentNode());
}

void BehaviorTree::onCollision( GameObject *other )
{
    if ( m_baseBranch != NULL )
    {
        m_baseBranch->onCollision( other );
    }
}

void BehaviorTree::onHit( )
{
    if ( m_baseBranch != NULL )
    {
        m_baseBranch->onHit( );
    }
}

void BehaviorTree::onPlayerDeath( )
{
    if ( m_baseBranch != NULL )
    {
        m_baseBranch->onPlayerDeath( );
    }
}