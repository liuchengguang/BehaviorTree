//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "Message.h"
#include "GameEngine.h"
#include "Sprite.h"
#include "BehaviorTree.h"
#include "PlayerController.h"
#include "Camera.h"
#include "AStar.h"

class ArtificialIntelligence : public IGameSystem
{
public:
    ArtificialIntelligence( );
    virtual ~ArtificialIntelligence( );

    static ArtificialIntelligence* GetAI( );
    virtual void Init( );
    virtual void Update( float deltaTime );
    virtual void OnMsgEvent( Msg const&msg );
    void PushBehaviorTree( BehaviorTree* );
    void PushPlayerController ( PlayerController* );
    void RemoveBehaviorTree ( BehaviorTree* );
    void RemovePlayerController ( PlayerController* );
	std::list<GameObject *> getPlayers();
    std::list<GameObject *> getControlPoints( );
    bool IsCollision( float worldX, float worldY );
    void onPlayerDeath( );
    void onPlayerCreate( );
    void SetPause( bool val );

    Camera* camera;
    AStar pathfinder;
    int nearPlayerDistance;

private:
    std::list<BehaviorTree*> m_behaviorTreeArray;
    std::list<PlayerController*> m_playerController;
    bool m_pause;

    //constant reference to graphics
    static ArtificialIntelligence* AI;
};