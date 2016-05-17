//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#pragma once

#include "Task.h"
#include "Transformation.h"
#include "Body.h"
#include <list>

class EscapeTarget : public Task
{
public:
    EscapeTarget( BehaviorTree *treeOwner );
    ~EscapeTarget( );

    int Update( float deltaTime );
    void Start( );
private:
    float m_repulseTime;
    float m_repulseTimeTotal;
    bool m_onRepulse;
    float m_repulsion;
    Transformation *m_transformation;
    Body* m_body;
    float m_velocity;
    float m_angle;
    int m_change;
    GameObject *m_object;
    float m_distance;
};