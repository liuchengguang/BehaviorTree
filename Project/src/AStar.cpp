//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "AStar.h"
#include "GamePlay.h"
#include "Physics.h"

AStar::AStar( void ) : m_state( PATH_NONE ), m_heuristic( HEURISTIC_OCTILE ), m_heuristicWeight( 1.1 ), m_mapWidth( 0 ), m_mapHeight( 0 )
, map( NULL ), m_closedList( NULL ), m_openList( NULL )
{
    m_testBodyTrans = NULL;
    m_testBodyShape = NULL;
    m_testBody = NULL;
    m_divisionUnit = 1;
    map = NULL;
}

AStar::~AStar( void )
{
    delete m_testBodyTrans;
    delete m_testBodyShape;
    delete m_testBody;
    FreeMap( );
}

void AStar::UpdateMap( void )
{
    if ( map != NULL )
    {
        FreeMap( );
    }
    CreateMap( );
}

void AStar::ResetSearch( void )
{
    m_start.row = -1;
    m_start.column = -1;
    m_goal.row = -1;
    m_goal.column = -1;
}

void AStar::SetHeuristic( int type )
{
    m_heuristic = type;
}

void AStar::SetHeuristicWeight( float weight )
{
    m_heuristicWeight = weight;
}

void AStar::SetStraightLine( int straightline )
{
    m_straightline = straightline;
}

void AStar::SetRuberBand( int rubberband )
{
    m_rubberband = rubberband;
}

void AStar::SetSmooth( int smooth )
{
    m_smooth = smooth;
}

void AStar::SetAnalysis( int analysis )
{
    m_analysis = analysis;
}

glm::vec3 AStar::GetMapToWorldCoordinates( int row, int col )
{
    float rowWorld = ((row + 0.5) / m_divisionUnit) - m_offsetMapX;
    float colWorld = ((col + 0.5) / m_divisionUnit) - m_offsetMapY;
    glm::vec3 coordinates( rowWorld, 0, colWorld );
    return coordinates;
}

//STUB implementation
float AStar::GetInfluenceMapValue( int row, float column )
{
    return 1;
}

int AStar::GetMapWidth( )
{
    return GamePlay::GetGameplay( )->worldWidth * m_divisionUnit;
}

int AStar::GetMapHeight( )
{
    return GamePlay::GetGameplay( )->worldHeight * m_divisionUnit;
}

bool AStar::IsMapCollision( int row, int col )
{
    glm::vec3 worldPos = GetMapToWorldCoordinates( row, col );
    m_testBodyTrans->SetTranslation( worldPos.x, 0, worldPos.z );
    return Physics::GetPhysics( )->IsColliding( m_testBody );
}

glm::vec2 AStar::ConvertToMap( float positionMapX, float positionMapY )
{
    glm::vec2 result( 0, 0 );
    result.x = int( (positionMapX + m_offsetMapX) * (m_mapWidth / GetMapWidth( )) );
    result.y = int( (positionMapY + m_offsetMapY) * (m_mapHeight / GetMapHeight( )) );
    return result;
}

bool AStar::IsCollisionWorld(float worldX, float worldY)
{
	glm::vec2 mapCoordinate = ConvertToMap(worldX, worldY);
	if (map[(int)mapCoordinate.x][(int)mapCoordinate.y].row == -1)
	{
		return false;
	}
	return true;
}

std::list<glm::vec3> AStar::FindPath( int startRow, int startColumn, int endRow, int endColumn )
{
    node *candidate = NULL, *nodeTmp, *mapNode;
    int i, j, binHeapPtr, binHeapSwapPtr, pathFound = 0, nodeIdx = 1;
    float gCost = 0;
    std::list<glm::vec3> path;
    glm::vec3 spot;

    if ( startRow >= m_mapWidth || endRow >= m_mapWidth || startColumn >= m_mapHeight || endColumn >= m_mapHeight
        || startRow < 0 || endRow < 0 || startColumn < 0 || endColumn < 0
        || (startRow == endRow && startColumn == endColumn) )
    {
        m_state = PATH_NOFOUND;
        spot = GetMapToWorldCoordinates( startRow, startColumn );
        path.push_front( spot );
        return path;
    }

    m_start = map[startRow][startColumn];
    m_goal = map[endRow][endColumn];
    InitializeMap( );

    if ( m_straightline == 1 )
    {
        if ( TestStraightLine( ) == 1 )
        {
            std::list<glm::vec3> path;
            glm::vec3 spot;
            spot = GetMapToWorldCoordinates( m_goal.row, m_goal.column );
            path.push_front( spot );
            spot = GetMapToWorldCoordinates( m_start.row, m_start.column );
            path.push_front( spot );
            return path;
        }
    }

    map[startRow][startColumn].isOpen = 1;
    *(m_openList + m_openListPtr) = &map[startRow][startColumn];
    m_openListPtr++;

    while ( pathFound == 0 )
    {
        if ( m_openListPtr > 1 )
        {
            candidate = *(m_openList + 1);
            (*(candidate)).isOpen = 0;
            m_openListPtr--;

            *(m_openList + 1) = *(m_openList + m_openListPtr);
            binHeapPtr = binHeapSwapPtr = 1;
            while ( 1 )
            {
                if ( (binHeapPtr << 1) + 1 < m_openListPtr )
                {
                    if ( (*(*(m_openList + binHeapPtr))).f > (*(*(m_openList + (binHeapPtr << 1)))).f )
                    {
                        binHeapSwapPtr = (binHeapPtr << 1);
                    }
                    if ( (*(*(m_openList + binHeapSwapPtr))).f > (*(*(m_openList + (binHeapPtr << 1) + 1))).f )
                    {
                        binHeapSwapPtr = (binHeapPtr << 1) + 1;
                    }
                }
                else if ( (binHeapPtr << 1) < m_openListPtr )
                {
                    if ( (*(*(m_openList + binHeapPtr))).f > (*(*(m_openList + (binHeapPtr << 1)))).f )
                    {
                        binHeapSwapPtr = (binHeapPtr << 1);
                    }
                }

                if ( binHeapPtr == binHeapSwapPtr )
                {
                    break;
                }
                else
                {
                    nodeTmp = *(m_openList + binHeapPtr);
                    *(m_openList + binHeapPtr) = *(m_openList + binHeapSwapPtr);
                    *(m_openList + binHeapSwapPtr) = nodeTmp;
                    binHeapPtr = binHeapSwapPtr;
                }
            }

            for ( i = 0; i < (*candidate).totalNeighbours; ++i )
            {
                mapNode = &map[(*candidate).neighbours[i].row][(*candidate).neighbours[i].col];
                if ( (*mapNode).isClosed == 1 )
                {
                    continue;
                }

                gCost = (*candidate).g + (*candidate).neighbours[i].cost;
                if ( m_analysis == 1 )
                {
                    gCost += GetInfluenceMapValue( mapNode->row, mapNode->column ) * 20;
                }
                if ( (*mapNode).isOpen == 0 || ((*mapNode).isOpen == 1 && gCost < (*mapNode).g) )
                {
                    (*mapNode).parent = candidate;
                    (*mapNode).g = gCost;
                    (*mapNode).f = gCost + (*mapNode).h;

                    (*mapNode).isOpen = 1;
                    *(m_openList + m_openListPtr) = mapNode;
                    binHeapPtr = m_openListPtr;
                    while ( binHeapPtr != 1 )
                    {
                        if ( (*(*(m_openList + binHeapPtr))).f < (*(*(m_openList + (binHeapPtr >> 1)))).f )
                        {
                            nodeTmp = *(m_openList + (binHeapPtr >> 1));
                            *(m_openList + (binHeapPtr >> 1)) = *(m_openList + binHeapPtr);
                            *(m_openList + binHeapPtr) = nodeTmp;
                            binHeapPtr = (binHeapPtr >> 1);
                        }
                        else
                        {
                            break;
                        }
                    }
                    m_openListPtr++;

                    //g_terrain.SetColor((*mapNode).row, (*mapNode).column, DEBUG_COLOR_GREEN);
                    if ( (*candidate).neighbours[i].row == m_goal.row && (*candidate).neighbours[i].col == m_goal.column )
                    {
                        m_state = PATH_FOUND;
                        pathFound = 1;
                        break;
                    }
                }
            }
            //g_terrain.SetColor((*candidate).row, (*candidate).column, DEBUG_COLOR_YELLOW);
            (*candidate).isClosed = 1;
        }
        else
        {
            m_state = PATH_NOFOUND;
            pathFound = 1;
        }
    }

    if ( m_state == PATH_FOUND )
    {

        if ( m_rubberband == 1 )
        {
            std::list<node> pathNodes;
            pathNodes.push_front( m_goal );
            node *next = map[m_goal.row][m_goal.column].parent;
            while ( next != NULL )
            {
                pathNodes.push_back( *next );
                next = (*next).parent;
            }

            pathNodes = GetRubberBandPath( pathNodes );

            std::list<node>::iterator iterator = pathNodes.begin( );
            for ( iterator = pathNodes.begin( ); iterator != pathNodes.end( ); ++iterator ) {
                spot = GetMapToWorldCoordinates( ( *iterator ).row, ( *iterator ).column );
                path.push_front( spot );
            }
        }
        else
        {
            path.push_front( GetMapToWorldCoordinates( m_goal.row, m_goal.column ) );
            node *next = map[m_goal.row][m_goal.column].parent;
            while ( next != NULL )
            {
                path.push_front( GetMapToWorldCoordinates( ( *next ).row, ( *next ).column ) );
                next = (*next).parent;
            }
        }
    }
    else
    {
        spot = GetMapToWorldCoordinates( m_start.row, m_start.column );
        path.push_front( spot );
    }
    return path;
}

bool AStar::FindPathStep( int startRow, int startColumn, int endRow, int endColumn )
{
    if ( m_start.row != startRow || m_start.column != startColumn || m_goal.row != endRow || m_goal.column != endColumn )
    {
        m_state = PATH_NONE;
    }

    switch ( m_state )
    {
    case PATH_NONE:
    case PATH_FOUND:
        m_start = map[startRow][startColumn];
        m_goal = map[endRow][endColumn];
        InitializeMap( );
        AddToListOpen( &map[startRow][startColumn] );
        m_straightLinePath = 0;
        if ( m_straightline == 1 )
        {
            if ( TestStraightLine( ) == 1 )
            {
                m_state = PATH_FOUND;
                m_straightLinePath = 1;
                return true;
            }
        }
        m_state = PATH_PROCESSING;
        if ( startRow == endRow && startColumn == endColumn )
        {
            m_state = PATH_NOFOUND;
        }
        break;
    case PATH_PROCESSING:
        node *candidate = NULL;
        int fCost = 0, listSize;
        GetCandidate( &candidate );
        if ( candidate == NULL )
        {
            m_state = PATH_NOFOUND;
            return true;
        }
        if ( EvaluateNeighbours( candidate ) == 1 )
        {
            m_state = PATH_FOUND;
            return true;
        }
        AddToListClosed( candidate );
        break;
    }
    return false;
}

std::list<glm::vec3> AStar::getPath( void )
{
    std::list<glm::vec3> path;
    glm::vec3 spot;
    if ( m_state == PATH_FOUND )
    {
        if ( m_straightLinePath == 1 )
        {
            spot = GetMapToWorldCoordinates( m_goal.row, m_goal.column );
            path.push_front( spot );
            spot = GetMapToWorldCoordinates( m_start.row, m_start.column );
            path.push_front( spot );
            return path;
        }
        else
        {
            std::list<node> pathNodes;
            pathNodes.push_front( m_goal );
            node *next = map[m_goal.row][m_goal.column].parent;
            while ( next != NULL )
            {
                pathNodes.push_back( *next );
                next = (*next).parent;
            }

            if ( m_rubberband == 1 )
            {
                pathNodes = GetRubberBandPath( pathNodes );
            }
            //if (m_smooth == 1)
            //{
            //	pathNodes = getSmoothPath(pathNodes);
            //}
            std::list<node>::iterator iterator = pathNodes.begin( );
            for ( iterator = pathNodes.begin( ); iterator != pathNodes.end( ); ++iterator ) {
                spot = GetMapToWorldCoordinates( ( *iterator ).row, ( *iterator ).column );
                path.push_front( spot );
            }
        }
    }
    else
    {
        glm::vec3 spot = GetMapToWorldCoordinates( m_start.row, m_start.column );
        path.push_back( spot );
    }
    return path;
}

void AStar::CreateMap( void )
{
    if ( m_testBodyTrans == NULL )
    {
        m_testBodyTrans = new Transformation( );
        m_testBodyTrans->SetTranslation( 0, 0, 0 );
        m_testBodyTrans->SetScale( 1, 1, 1 );
        m_testBodyShape = new RectangleShape( "recshape", glm::vec3( 0, 0, 0 ), 1, 1 );
        m_testBody = new Body( m_testBodyShape, m_testBodyTrans );

        m_testBodyTrans->SetScale( 1.0f, 1.0f, 1.0f );
        m_testBodyShape->SetWidth( 1.0f/m_divisionUnit*1.5 );
        m_testBodyShape->SetHeight( 1.0f/m_divisionUnit*1.5 );
        Physics::GetPhysics( )->RemoveBody( m_testBody );
    }

    node *mapNode;
    m_mapWidth = GetMapWidth( );
    m_mapHeight = GetMapHeight( );

    m_offsetMapX = (GamePlay::GetGameplay( )->worldWidth*m_divisionUnit / 2);
    m_offsetMapY = (GamePlay::GetGameplay( )->worldHeight*m_divisionUnit / 2);

    m_listWidth = m_mapWidth * m_mapHeight + 1;
    m_openList = (node **)malloc( m_listWidth * sizeof( struct node* ) );
    *m_openList = NULL;
    m_openListPtr = 1;
    m_closedList = (node **)malloc( m_listWidth * sizeof( struct node* ) );
    m_closeListPtr = 1;
    int neighbourRow[8] = { 1, 1, 0, -1, -1, 1, 0, -1 };
    int neighbourCol[8] = { 0, 1, 1, -1, 0, -1, -1, 1 };
    unsigned int searchRow = 0, searchCol = 0;

    int i = 0, j = 0, k = 0;
    map = (node**)malloc( m_mapWidth*sizeof( struct node* ) );
    for ( i = 0; i < m_mapWidth; ++i )
    {
        *(map + i) = (node*)malloc( m_mapHeight*sizeof( struct node ) );
    }
    for ( i = 0; i < m_mapWidth; ++i )
    {
        for ( j = 0; j < m_mapHeight; ++j )
        {
            map[i][j].row = -1;
            if ( IsMapCollision( i, j ) == false )
            {
                (*(*(map + i) + j)).row = i;
                (*(*(map + i) + j)).column = j;
                (*(*(map + i) + j)).parent = NULL;
            }
        }
    }

    for ( i = 0; i < m_mapWidth; ++i )
    {
        for ( j = 0; j < m_mapHeight; ++j )
        {
            if ( map[i][j].row != -1 )
            {
                map[i][j].totalNeighbours = 0;
                for ( k = 0; k < 8; ++k )
                {
                    searchRow = map[i][j].row + neighbourRow[k];
                    searchCol = map[i][j].column + neighbourCol[k];
                    if ( searchCol >= m_mapHeight || searchRow >= m_mapWidth )
                    {
                        continue;
                    }

                    if ( map[searchRow][searchCol].row == -1 )
                    {
                        continue;
                    }
                    if ( k % 2 == 1 )
                    {
                        if ( map[searchRow - neighbourRow[k]][searchCol].row == -1 || map[searchRow][searchCol - neighbourCol[k]].row == -1 )
                        {
                            continue;
                        }
                        map[i][j].neighbours[map[i][j].totalNeighbours].cost = COST_DIAGONAL;
                    }
                    else
                    {
                        map[i][j].neighbours[map[i][j].totalNeighbours].cost = COST_STRAIGHT;
                    }

                    map[i][j].neighbours[map[i][j].totalNeighbours].col = searchCol;
                    map[i][j].neighbours[map[i][j].totalNeighbours].row = searchRow;
                    map[i][j].totalNeighbours++;
                }
            }
        }
    }
}

void AStar::InitializeMap( void )
{
    int octileMin, octileMax, tmpOctile;
    float minOctileMult[40] = { 0, 0.41, 0.82, 1.23, 1.64, 2.05, 2.46, 2.87, 3.28, 3.69, 4.1, 4.51, 4.92, 5.33, 5.74, 6.15, 6.56, 6.97, 7.38, 7.79, 8.2, 8.61, 9.02, 9.43, 9.84, 10.25, 10.66, 11.07, 11.48, 11.89, 12.3, 12.71, 13.12, 13.53, 13.94, 14.35, 14.76, 15.17, 15.58, 15.99 };

    for ( int i = 0; i < m_mapWidth; ++i )
    {
        for ( int j = 0; j < m_mapHeight; ++j )
        {
            if ( map[i][j].row != -1 )
            {
                map[i][j].parent = NULL;
                map[i][j].g = 0;
                map[i][j].isClosed = 0;
                map[i][j].isOpen = 0;

                switch ( m_heuristic )
                {
                case HEURISTIC_EUCLIDEAN:
                    map[i][j].h = (sqrt( pow( (m_goal.row - map[i][j].row) * COST_STRAIGHT, 2 ) + pow( (m_goal.column - map[i][j].column) * COST_STRAIGHT, 2 ) )) * m_heuristicWeight;
                    break;
                case HEURISTIC_OCTILE:
                    octileMin = abs( m_goal.row - map[i][j].row );
                    octileMax = abs( m_goal.column - map[i][j].column );
                    if ( octileMax < octileMin )
                    {
                        tmpOctile = octileMin;
                        octileMin = octileMax;
                        octileMax = tmpOctile;
                    }

                    map[i][j].h = (octileMax + minOctileMult[octileMin]) * m_heuristicWeight;
                    break;
                case HEURISTIC_CHEBYSEV:
                    octileMin = abs( m_goal.row - map[i][j].row );
                    octileMax = abs( m_goal.column - map[i][j].column );
                    if ( octileMax < octileMin )
                    {
                        tmpOctile = octileMin;
                        octileMin = octileMax;
                        octileMax = tmpOctile;
                    }
                    map[i][j].h = (octileMax * COST_STRAIGHT) * m_heuristicWeight;
                    break;
                case HEURISTIC_MANHATTAN:
                    map[i][j].h = ((abs( m_goal.row - map[i][j].row ) + abs( m_goal.column - map[i][j].column )) * COST_STRAIGHT) * m_heuristicWeight;
                    break;
                }

            }
        }
    }
    m_openListPtr = 1;
    m_closeListPtr = 1;
}

void AStar::FreeMap( void )
{
    if ( m_closedList != NULL )
    {
        free( m_closedList );
    }
    if ( m_openList != NULL )
    {
        free( m_openList );
    }

    if ( map != NULL )
    {
        int i = 0, j = 0;
        for ( i = 0; i < m_mapWidth; ++i )
        {
            free( *( map + i ) );
        }
        free( map );
    }
}

int AStar::TestStraightLine( void )
{
    int searchStartRow, searchStartCol, searchEndRow, searchEndCol, i, j;
    searchStartRow = m_start.row;
    searchStartCol = m_start.column;
    searchEndRow = m_goal.row;
    searchEndCol = m_goal.column;
    if ( searchEndRow < searchStartRow )
    {
        searchStartRow = m_goal.row;
        searchEndRow = m_start.row;
    }
    if ( searchEndCol < searchStartCol )
    {
        searchStartCol = m_goal.column;
        searchEndCol = m_start.column;
    }
    int foundWall = 0;
    for ( i = searchStartRow; i <= searchEndRow; ++i )
    {
        for ( j = searchStartCol; j <= searchEndCol; ++j )
        {
            if ( map[i][j].row == -1 )
            {
                foundWall = 1;
            }
        }
    }
    if ( foundWall == 0 )
    {
        return 1;
    }
    return 0;
}

std::list<node> AStar::GetRubberBandPath( std::list<node> pathNodes )
{
    int searchStartRow, searchStartCol, searchEndRow, searchEndCol, i, j, foundWall = 0;

    std::list<node>::iterator iterator = pathNodes.begin( );
    std::list<node>::iterator iteratorCurr;
    std::list<node>::iterator iteratorCurr1;
    std::list<node>::iterator iteratorCurr2;
    int listNodePos = 0, listNodeSize = pathNodes.size( );
    while ( iterator != pathNodes.end( ) && (listNodePos < listNodeSize - 2) )
    {
        foundWall = 0;
        iteratorCurr = iterator;
        iteratorCurr1 = ++iterator;
        iteratorCurr2 = ++iterator;


        searchStartRow = (*iteratorCurr).row;
        searchStartCol = (*iteratorCurr).column;
        searchEndRow = (*iteratorCurr2).row;
        searchEndCol = (*iteratorCurr2).column;
        if ( searchEndRow < searchStartRow )
        {
            searchStartRow = (*iteratorCurr2).row;
            searchEndRow = (*iteratorCurr).row;
        }
        if ( searchEndCol < searchStartCol )
        {
            searchStartCol = (*iteratorCurr2).column;
            searchEndCol = (*iteratorCurr).column;
        }
        for ( i = searchStartRow; i <= searchEndRow; ++i )
        {
            for ( j = searchStartCol; j <= searchEndCol; ++j )
            {
                if ( map[i][j].row == -1 )
                {
                    foundWall = 1;
                }
            }
        }
        if ( foundWall == 0 )
        {
            pathNodes.remove( *iteratorCurr1 );
            iterator = iteratorCurr;
            listNodeSize--;
        }
        else
        {
            iterator = iteratorCurr1;
            listNodePos++;
        }
    }

    return pathNodes;
}

std::list<node> AStar::GetSmoothPath( std::list<node> pathNodes )
{
    node tmpNode;
    float distance = 0;
    std::list<node>::iterator iterator = pathNodes.begin( );
    std::list<node>::iterator iteratorCurr;
    std::list<node>::iterator iteratorCurr1;
    int listNodePos = 0, listNodeSize = pathNodes.size( );
    while ( iterator != pathNodes.end( ) && (listNodePos < listNodeSize - 1) )
    {
        iteratorCurr = iterator;
        iteratorCurr1 = ++iterator;
        distance = sqrt( pow( iteratorCurr->row - iteratorCurr1->row, 2 ) + pow( iteratorCurr->column - iteratorCurr1->column, 2 ) );
        if ( distance > 2 )
        {
            tmpNode.row = iteratorCurr->row + (iteratorCurr1->row - iteratorCurr->row) / distance;
            tmpNode.column = iteratorCurr->column + (iteratorCurr1->column - iteratorCurr->column) / distance;
            pathNodes.insert( iteratorCurr1, tmpNode );
            listNodeSize++;
        }
        else
        {
            listNodePos++;
        }
    }
    return pathNodes;
}

void AStar::AddToListOpen( node *element )
{
    (*element).isOpen = 1;
    *(m_openList + m_openListPtr) = element;
    //g_terrain.SetColor((*element).row, (*element).column, DEBUG_COLOR_GREEN);
    m_openListPtr++;
}

void AStar::AddToListClosed( node *element )
{
    (*element).isClosed = 1;
    *(m_closedList + m_closeListPtr) = element;
    //g_terrain.SetColor((*element).row, (*element).column, DEBUG_COLOR_YELLOW);
    m_closeListPtr++;
}

void AStar::GetCandidate( node **element )
{
    int nodeIdx = 1;
    node *candidate;
    candidate = *(m_openList + 1);
    for ( int i = 2; i < m_openListPtr; ++i )
    {
        if ( (*candidate).f >( *(*(m_openList + i)) ).f )
        {
            candidate = *(m_openList + i);
            nodeIdx = i;
        }
    }
    m_openListPtr--;
    if ( m_openListPtr > 0 )
    {
        *element = candidate;
        (*(*element)).isOpen = 0;
        *(m_openList + nodeIdx) = *(m_openList + m_openListPtr);
    }
    else
    {
        *element = NULL;
    }
}

int AStar::EvaluateNeighbours( node * candidate )
{
    int i, j;
    short searchRow = 0, searchCol = 0;
    float gCost = 0, fCost = 0;
    float octileMin, octileMax, tmpOctile;
    for ( i = -1; i <= 1; ++i )
    {
        for ( j = -1; j <= 1; ++j )
        {
            if ( i == 0 && j == 0 )
            {
                continue;
            }
            searchRow = (*candidate).row + i;
            searchCol = (*candidate).column + j;
            if ( searchRow < 0 || searchRow >= m_mapWidth || searchCol < 0 || searchCol >= m_mapHeight )
            {
                continue;
            }
            if ( map[searchRow][searchCol].row == -1 )
            {
                continue;
            }
            if ( map[searchRow][searchCol].isClosed == 1 )
            {
                continue;
            }
            if ( i != 0 && j != 0 )
            {
                if ( map[searchRow - i][searchCol].row == -1 || map[searchRow][searchCol - j].row == -1 )
                {
                    continue;
                }
                gCost = (*candidate).g + COST_DIAGONAL;
            }
            else
            {
                gCost = (*candidate).g + COST_STRAIGHT;
            }

            if ( m_analysis == 1 )
            {
                gCost += GetInfluenceMapValue( searchRow, searchCol ) * 20;
            }

            if ( map[searchRow][searchCol].h == 0 )
            {
                switch ( m_heuristic )
                {
                case HEURISTIC_EUCLIDEAN:
                    map[searchRow][searchCol].h = (sqrt( pow( (m_goal.row - map[searchRow][searchCol].row) * COST_STRAIGHT, 2 ) + pow( (m_goal.column - map[searchRow][searchCol].column) * COST_STRAIGHT, 2 ) )) * m_heuristicWeight;
                    break;
                case HEURISTIC_OCTILE:
                    octileMin = abs( m_goal.row - map[searchRow][searchCol].row );
                    octileMax = abs( m_goal.column - map[searchRow][searchCol].column );
                    if ( octileMax < octileMin )
                    {
                        tmpOctile = octileMin;
                        octileMin = octileMax;
                        octileMax = tmpOctile;
                    }
                    map[searchRow][searchCol].h = ((octileMin * COST_DIAGONAL) + (octileMax - octileMin) * COST_STRAIGHT) * m_heuristicWeight;
                    break;
                case HEURISTIC_CHEBYSEV:
                    octileMin = abs( m_goal.row - map[searchRow][searchCol].row );
                    octileMax = abs( m_goal.column - map[searchRow][searchCol].column );
                    if ( octileMax < octileMin )
                    {
                        tmpOctile = octileMin;
                        octileMin = octileMax;
                        octileMax = tmpOctile;
                    }
                    map[searchRow][searchCol].h = (octileMax * COST_STRAIGHT) * m_heuristicWeight;
                    break;
                case HEURISTIC_MANHATTAN:
                    map[searchRow][searchCol].h = ((abs( m_goal.row - map[searchRow][searchCol].row ) + abs( m_goal.column - map[searchRow][searchCol].column )) * COST_STRAIGHT) * m_heuristicWeight;
                    break;
                }
            }

            fCost = gCost + map[searchRow][searchCol].h;
            if ( map[searchRow][searchCol].isOpen == 0 || (map[searchRow][searchCol].isOpen == 1 && fCost < map[searchRow][searchCol].f) )
            {
                map[searchRow][searchCol].parent = candidate;
                map[searchRow][searchCol].g = gCost;
                map[searchRow][searchCol].f = fCost;

                map[searchRow][searchCol].isOpen = 1;
                *(m_openList + m_openListPtr) = &map[searchRow][searchCol];
                //g_terrain.SetColor(map[searchRow][searchCol].row, map[searchRow][searchCol].column, DEBUG_COLOR_GREEN);
                m_openListPtr++;

                if ( searchRow == m_goal.row && searchCol == m_goal.column )
                {
                    m_state = PATH_FOUND;
                    return 1;
                }
            }
        }
    }
    return 0;
}