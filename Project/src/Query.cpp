//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "Query.h"

Query::Query( BehaviorTree *treeOwner, bool reevaluate ) : Node( treeOwner ), m_reevaluate( reevaluate )
{
    type = NTYPE_QUERY;
}

Query::~Query( )
{

}