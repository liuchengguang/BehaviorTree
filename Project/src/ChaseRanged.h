//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#pragma once

#include "Task.h"
#include "Transformation.h"
#include "Body.h"
#include <list>

#define WAIT_TRY_REPATH 0.2

class ChaseRanged : public Task
{
public:
    enum states
    {
        STATE_CHASE = 0,
        STATE_RETREAT,
		STATE_RECOVER,
	};

    ChaseRanged( BehaviorTree *treeOwner );
    ~ChaseRanged( );

    int Update( float deltaTime );
    void Start( );
    void onCollision( GameObject *other );
    void onHit(  );
    void onPlayerDeath( );
private:
    int m_state;
    int m_stateLast;
    bool m_onPlayerDeath;

    Transformation *m_transformation;
    Body* m_body;
    float m_velocity;
    float m_angle;
    int m_change;
    GameObject *m_object;
    float m_distance;
    float m_time;
    float m_timeMax;
	float m_timeRecover;
	float m_timeRecoverMax;
    float m_waitRePathTime;
    bool m_onWaitRepath;
    float m_waitTryRepath;

    void setTargetPath( );
    std::list<glm::vec3> m_path;
    glm::vec3 currentTarget;
    glm::vec2 m_targetPosition;
};