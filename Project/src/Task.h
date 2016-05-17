//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#pragma once

#include "Node.h"

class Task : public Node
{
public:
    Task( BehaviorTree *treeOwner );
    ~Task( );

    void Start( );
private:
};