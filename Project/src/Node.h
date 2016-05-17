//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#pragma once

#include "Blackboard.h"

enum NTYPE
{
	NTYPE_BRANCH = 0,
	NTYPE_QUERY,
	NTYPE_TASK,
	NTYPE_DECORATOR,
	NTYPE_BLACKBOARD,
};

enum NSTATUS
{
	NSTATUS_WAIT = 0,
	NSTATUS_RUNNING,
	NSTATUS_FAIL,
	NSTATUS_SUCCESS,
};

class BehaviorTree;
class Node
{
public:
	Node(BehaviorTree *treeOwner);
	virtual ~Node();

	virtual int Update(float deltaTime) { return NSTATUS_RUNNING; }
	virtual void Start() {}
	int GetStatus();
	void SetParent(Node *parent);
	void SetOverParent(Node *overParent);
	Node *GetParent() { return m_parent; }
	Node *GetOverParent() { return m_overParent; }
    virtual void onCollision( GameObject *other );
    virtual void onHit(  );
    virtual void onPlayerDeath( );

	int type;
protected:
	int m_status;
	BehaviorTree *m_behaviorTree;
	Blackboard *m_blackboard;
	Node *m_overParent;
	Node *m_parent;

private:
};