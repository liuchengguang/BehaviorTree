//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#pragma once
#include "Debug.h"
#include "Task.h"
#include "Transformation.h"
#include "Body.h"

class Wander : public Task
{
public:
    Wander( BehaviorTree *treeOwner );
    ~Wander( );

    void Start( );
    int Update( float deltaTime );
    void onCollision( GameObject *other );
private:
    Transformation *m_transformation;
    Body* m_body;
    float m_velocity;
    float m_angle;
    int m_change;
    float m_timeChangeDirectionMax;
    float m_timeChangeDirection;
    int m_direction;
};