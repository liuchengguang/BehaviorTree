//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#pragma once
#include "Debug.h"
#include "Task.h"
#include "Transformation.h"
#include "Body.h"
#include <list>

class ShootRandom : public Task
{
public:
    ShootRandom( BehaviorTree *treeOwner );
    ~ShootRandom( );

    int Update( float deltaTime );
    void Start( );
private:
    Transformation *m_transformation;
    Body* m_body;
    GameObject * m_target;
    std::list<glm::vec3> m_path;
};