//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#pragma once

#include "Decorator.h"

class AlwaysSuccess : public Decorator
{
public:
    AlwaysSuccess( BehaviorTree *treeOwner, Node *nodeDecorate );
    ~AlwaysSuccess( );

    int Update( float deltaTime );
private:
    Node *m_task;
};