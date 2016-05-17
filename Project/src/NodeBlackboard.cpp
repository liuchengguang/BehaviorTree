//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "NodeBlackboard.h"
#include "GamePlay.h"
#include "Transformation.h"
#include "ArtificialIntelligence.h"

NodeBlackboard::NodeBlackboard( BehaviorTree *treeOwner, std::string key, int valueType ) : Node( treeOwner ), m_valueType( valueType ), m_key( key )
{
    type = NTYPE_BLACKBOARD;
}

NodeBlackboard::NodeBlackboard( BehaviorTree *treeOwner ) : Node( treeOwner )
{
    type = NTYPE_BLACKBOARD;
}

NodeBlackboard::~NodeBlackboard( )
{

}

void NodeBlackboard::processValue( )
{
    switch ( m_valueType )
    {
    case BLCK_INT:
        m_blackboard->SetParameterInt( m_key, m_valueInt );
        break;
    case BLCK_FLOAT:
        m_blackboard->SetParameterFloat( m_key, m_valueFloat );
        break;
    case BLCK_OBJ:
        m_blackboard->SetParameterGameObj( m_key, m_valueGameObj );
        break;
    case BLCK_SETPLAYERPOS:
        setPlayerPosition( m_key, m_key2 );
        break;
    }
}

void NodeBlackboard::setPlayerPosition( std::string keyX, std::string keyZ )
{
    m_valueType = BLCK_SETPLAYERPOS;
    m_key = keyX;
    m_key2 = keyZ;

	Transformation *playerTransform;

	std::list<GameObject* > players = ArtificialIntelligence::GetAI()->getPlayers();
	GameObject *player = NULL;
	if (players.size() > 0)
	{
		player = players.front();
	}

	if (player != NULL)
	{
		playerTransform = dynamic_cast<Transformation*>(player->GetComponent(TYPE_Transformation));
		m_blackboard->SetParameterFloat(m_key, playerTransform->GetTranslation().x);
		m_blackboard->SetParameterFloat(m_key2, playerTransform->GetTranslation().z);
	}
	else
	{
		m_blackboard->SetParameterFloat(m_key, -9999);
		m_blackboard->SetParameterFloat(m_key2, -9999);
	}
}