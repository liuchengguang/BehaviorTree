//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#pragma once


#include <list>
#include <GLM\glm.hpp>
#include "Body.h"
#include "RectangleShape.h"

#define COST_STRAIGHT 1
#define COST_DIAGONAL_STRAIGHT 0.4
#define COST_DIAGONAL 1.4
#define NEIGHBOURS_SIZE 8

enum pathState
{
    PATH_NONE = 0,
    PATH_PROCESSING,
    PATH_FOUND,
    PATH_NOFOUND,
};

enum heuristicType
{
    HEURISTIC_EUCLIDEAN = 0,
    HEURISTIC_OCTILE,
    HEURISTIC_CHEBYSEV,
    HEURISTIC_MANHATTAN
};

struct neighbour
{
    int row;
    int col;
    float cost;
};

struct node
{
    node *parent;
    neighbour neighbours[8];
    int totalNeighbours;
    float f;
    float g;
    float h;
    short row;
    short column;
    unsigned char isOpen;
    unsigned char isClosed;

    bool operator==(const node& rhs)
    {
        return (row == rhs.row && column == rhs.column);
    }
};

class AStar
{
public:
    AStar( void );
    ~AStar( void );

    glm::vec2 ConvertToMap( float positionMapX, float positionMapY );
    int GetMapWidth( );
    int GetMapHeight( );
    bool IsMapCollision( int row, int col );
    glm::vec3 GetMapToWorldCoordinates( int row, int col );
    float GetInfluenceMapValue( int row, float column );
    void SetHeuristic( int type );
    void SetHeuristicWeight( float weigth );
    void SetStraightLine( int straightline );
    void SetRuberBand( int rubberband );
    void SetSmooth( int smooth );
    void SetAnalysis( int analysis );
    void SetFow( int fow );
    void UpdateMap( void );
    void ResetSearch( void );
    std::list<glm::vec3> FindPath( int startRow, int startColumn, int endRow, int endColumn );
    bool FindPathStep( int startRow, int startColumn, int endRow, int endColumn );
    std::list<glm::vec3> getPath( void );
	bool IsCollisionWorld(float worldX, float worldY);

protected:

private:
    void CreateMap( void );
    void InitializeMap( void );
    void FreeMap( void );
    int TestStraightLine( void );
    std::list<node> GetRubberBandPath( std::list<node> );
    std::list<node> GetSmoothPath( std::list<node> );
    void AddToListOpen( node *element );
    void AddToListClosed( node *element );
    void GetCandidate( node **element );
    int EvaluateNeighbours( node *candidate );

    int m_offsetMapX;
    int m_offsetMapY;
    int m_divisionUnit;
    Transformation *m_testBodyTrans;
    RectangleShape *m_testBodyShape;
    Body *m_testBody;
    int m_heuristic;
    int m_straightline;
    int m_rubberband;
    int m_smooth;
    int m_analysis;
    int m_state;
    node m_start;
    node m_goal;
    node **map;
    node **m_openList;
    int m_openListPtr;
    node **m_closedList;
    int m_closeListPtr;
    int m_mapWidth;
    int m_mapHeight;
    int m_listWidth;
    int m_straightLinePath;
    float m_heuristicWeight;
};