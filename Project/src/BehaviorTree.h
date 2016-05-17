//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#pragma once
#include "ObjectComponent.h"
#include "NodeBranch.h"
#include "Blackboard.h"
#include "Transformation.h"
#include "Body.h"
#include <list>

class ControlPoint;

class BehaviorTree : public ObjectComponent
{
public:
    BehaviorTree( );
    ~BehaviorTree( );

    Transformation *getTransformation( ) { return m_transformation; }
    Body *getBody( ) { return m_body; }

    Blackboard *getBlackboard( );
    virtual void Initialize( );
    void Update( float );
    void UpdateBranchPointer( Node *nodeBranch );
    void setBaseBranch ( Node *baseNode );
    std::list<GameObject* > GetPlayers( );
    std::list<GameObject* > GetControlPoints( ) { return m_controlPoints; }
    void SetOnAttackMeleeSlot( std::string const &_script, std::string const &function );
    void OnAttackMelee( );
    void setEnemyControlPoint( float deltaTime );
    void onCollision( GameObject *other );
    void onHit( );
    void onPlayerDeath( );

	ControlPoint* GetControlPoint() const;

	bool onStop;
    unsigned int ID;
private:
    std::list<GameObject* > m_players;
    std::list<GameObject* > m_controlPoints;
    Node *m_baseBranch;
    Node *m_currentBranch;
    Blackboard m_blackboard;

    Transformation *m_transformation;
    Body *m_body;
    std::string onAttackMeleeSlot;
    std::string script;
};