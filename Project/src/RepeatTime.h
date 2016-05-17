//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#pragma once

#include "Decorator.h"

class RepeatTime : public Decorator
{
public:
    RepeatTime( BehaviorTree *treeOwner, Node *nodeDecorate );
    ~RepeatTime( );

    void Start( );
    int Update( float deltaTime );
private:
    Node *m_task;
    float m_timeAcumulator;
    float m_timeValue;
};