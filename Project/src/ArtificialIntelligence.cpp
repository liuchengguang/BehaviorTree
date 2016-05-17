//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "ArtificialIntelligence.h"
#include "InputSystem.h"

ArtificialIntelligence *ArtificialIntelligence::AI = NULL;

ArtificialIntelligence::ArtificialIntelligence( )
{
    camera = NULL;
    if ( AI == NULL )
    {
        AI = this;
    }
    m_behaviorTreeArray.clear( );
    m_playerController.clear( );

    InputSystem::INPUT_SYSTEM->AddListener( this ); 
    nearPlayerDistance = 15;
}

ArtificialIntelligence::~ArtificialIntelligence( )
{
    AI = NULL;
}

ArtificialIntelligence* ArtificialIntelligence::GetAI( )
{
    return AI;
}

void ArtificialIntelligence::SetPause( bool val )
{
    m_pause = val;
}

void ArtificialIntelligence::PushBehaviorTree( BehaviorTree *behaviorTree )
{
    m_behaviorTreeArray.push_back( behaviorTree );
}

void ArtificialIntelligence::PushPlayerController( PlayerController *playerController )
{
    m_playerController.push_back( playerController );
}

void ArtificialIntelligence::RemoveBehaviorTree( BehaviorTree*behaviorTree )
{
    m_behaviorTreeArray.remove( behaviorTree );
}

void ArtificialIntelligence::RemovePlayerController( PlayerController*playerController )
{
    m_playerController.remove( playerController );
}

void ArtificialIntelligence::Init( )
{
    //pathfinder.UpdateMap( );
}

void ArtificialIntelligence::Update( float deltaTime )
{
    if ( m_pause == true )
    {
        return;
    }

    for each ( BehaviorTree *behaviorTree in m_behaviorTreeArray )
    {
        behaviorTree->Update( deltaTime );
    }
    for each ( PlayerController *playerController in m_playerController )
    {
        playerController->Update( deltaTime );
    }
	if (camera != NULL)
	{
		camera->Update(deltaTime);
	}
}

void ArtificialIntelligence::OnMsgEvent( Msg const&msg )
{
    int currentController = InputSystem::INPUT_SYSTEM->currentController;
    if ( currentController < m_playerController.size( ) )
    {
        int idx = 0;
        for each ( PlayerController *playerController in m_playerController )
        {
            if ( idx  == currentController )
            {
                playerController->OnMsgEvent( msg );
                break;
            }
            idx++;
        }
    }
}

std::list<GameObject *> ArtificialIntelligence::getPlayers()
{
	std::list<GameObject *> players;
	for each (PlayerController *playerController in m_playerController)
	{
		players.push_back(playerController->GetGameObject());
	}
	return players;
}

std::list<GameObject *> ArtificialIntelligence::getControlPoints( )
{
    std::list<GameObject *> controlPoints;
    GameObject *controlPoint;
    for each ( BehaviorTree *behaviorTree in m_behaviorTreeArray )
    {
        controlPoint = behaviorTree->GetGameObject( );
        if ( controlPoint->GetTag( ).compare( "ControlPoint" ) == 0 )
        {
            controlPoints.push_back( controlPoint );
        }
    }
    return controlPoints;
}

bool ArtificialIntelligence::IsCollision(float worldX, float worldY)
{
	return pathfinder.IsCollisionWorld(worldX, worldY);
}

void ArtificialIntelligence::onPlayerDeath( )
{
    for each (BehaviorTree *behaviorTree in m_behaviorTreeArray)
    {
        behaviorTree->onPlayerDeath( );
    }
    if ( camera != NULL )
    {
        camera->refreshPlayers( );
    }
}

void ArtificialIntelligence::onPlayerCreate( )
{
    if ( camera != NULL )
    {
        camera->refreshPlayers( );
    }
}