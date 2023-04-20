#pragma once
#include <vector>
#include "Cell.h"
class CCell;

struct Index;
class CGrid
{
private:
	std::vector<std::vector<CCell*>> cells; // Ma trận toàn bộ các cell trong map
	int columns;
	int rows;
	std::vector <CCell*> activeCells; // các cell được active trong viewport
public:
	CGrid();
	CGrid(Point mapSize);
	void Insert(CGameObject* gameObject);
	void Remove(CGameObject* gameObject);
	void Move(Point oldPosition, CGameObject* gameObject);
	std::vector <CCell*>  FindActiveCells(CCamera* camera);
	std::vector <CGameObject*> FindActiveGameObjects(CCamera* camera);
	RECT GetRectByPosition(Point pos, Point size);
	Index GetCellIndexByPosition(Point pos);
	CCell* GetCell(Index index);
	CCell* GetCell(Point position);
	~CGrid();
};

