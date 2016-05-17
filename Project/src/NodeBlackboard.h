//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#pragma once

#include "Node.h"
#include <string>

enum blckValueType
{
    BLCK_INT = 0,
    BLCK_FLOAT,
    BLCK_OBJ,
    BLCK_SETPLAYERPOS,
};

class NodeBlackboard : public Node
{
public:
    NodeBlackboard( BehaviorTree *treeOwner, std::string key, int valueType );
    NodeBlackboard( BehaviorTree *treeOwner );
    ~NodeBlackboard( );

    void setPlayerPosition( std::string keyX, std::string keyZ );
    void setValueInt( int valueInt ) { m_valueInt = valueInt; };
    void setValueFloat( float valueFloat ) { m_valueFloat = valueFloat; };
    void setValueGameObject( GameObject *valueGameObj ) { m_valueGameObj = valueGameObj; };
    void processValue( );
private:
    int m_valueType;
    std::string m_key;
    std::string m_key2;
    GameObject *m_valueGameObj;
    int m_valueInt;
    float m_valueFloat;
};