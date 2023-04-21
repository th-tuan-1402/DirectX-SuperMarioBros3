#pragma once
#include "Camera.h"

#include "XMLHelper.h"
#include <map>
#include <vector>
#include "TextureManager.h"
#include "Graph.h"
#include "Scene.h"
#include "Index.h"
struct TileSet;
struct Layer;
class CGraph;
class CObjectPool;
class CGrid;
class CScene;
struct Index;
class CTileMap
{
private:
	int width;
	int height;
	int tileWidth;
	int tileHeight;

	std::map<int, TileSet*> tileSets;
	std::vector<Layer*> layers;
	Layer* foreground;
	CGraph* graph;
	CGrid* grid;
	std::vector<CGameObject*>bricks, coins;
	CObjectPool* poolBricks;
	CObjectPool* poolCoins;

	CGameObject* card;
	CGameObject* player;
	CScene* scene;
	std::unordered_map<int, std::unordered_map<int, CGameObject*>> objectGroups;
	Index currentGOIndex;

public:
	CTileMap();
	CTileMap(int width, int height, int tileWidth, int tileHeight);

	TileSet* GetTileSetByTileID(int id);
	CTileMap* LoadMap(std::string filePath, std::string fileMap, std::vector<LPGameObject>& listGameObjects, CGameObject* player, CScene* scene);
	Layer* LoadLayer(XMLElement* layerElement);

	CGameObject* LoadSolidBox(Point position, Point size, std::string name, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadGhostBox(Point position, Point size, std::string name, std::vector<LPGameObject>& listGameObjects);

	CGameObject* LoadEnemy(Point position, std::string enemyName, std::string enemyType, XMLElement* object, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadKoopa(Point position, std::string enemyType, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadParakoopa(Point position, std::string enemyType, XMLElement* object, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadGoomba(Point position, std::string enemyType, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadParagoomba(Point position, std::string enemyType, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadPiranha(Point position, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadVenus(Point position, std::string enemyType, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadBoomerangBrother(Point position, std::vector<LPGameObject>& listGameObjects);

	CGameObject* LoadQuestionBlock(Point position, int type, std::string name, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadBrick(Point position, int type, std::string name, XMLElement* object, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadCoin(Point position, int type, XMLElement* object, std::vector<LPGameObject>& listGameObjects);

	CGameObject* LoadPipe(Point position, Point size, std::string direction, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadPortal(Point position, Point size, XMLElement* object, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadLabel(Point position, std::string labelName, Point size, XMLElement* object, std::vector<LPGameObject>& listGameObjects);

	CGameObject* LoadWorldItem(Point position, std::string itemName, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadPortalScene(Point position, Point size, XMLElement* object, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadSceneGate(Point position, Point size, XMLElement* object, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadNodeGate(Point position, Point size, XMLElement* object, std::vector<LPGameObject>& listGameObjects);

	CGameObject* LoadSwitchBlock(Point position, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadEmptyBlock(Point position, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadCard(Point position, std::vector<LPGameObject>& listGameObjects);
	CGameObject* LoadMovingPlatform(Point position, std::vector<LPGameObject>& listGameObjects);

	void Render(CCamera* camera, bool isRenderForeground);
	void RenderLayer(Layer* layer, int i, int j, int x, int y);
	CGraph* GetGraph();

	// Bricks và Coins đặc biệt là do nó chịu sự quản lý của Switch Button => Sau khi Switch Button chuyển trạng thái, Brick sẽ chuyển thành coin 
	// => Ta lấy coin từ poolCoins. Hết thời gian thì Coin chuyển thành Brick, ta dùng cách ngược lại
	std::vector<CGameObject*> GetBricks();
	std::vector<CGameObject*> GetCoins();
	CObjectPool* GetPoolBricks();
	CObjectPool* GetPoolCoins();
	CGameObject* GetCard();

	void AddObjectToList(CGameObject* gO);
	void AddObjectToList(CGameObject* gO, std::vector<LPGameObject>& listGameObjects);
	CGrid* GetGrid();
	~CTileMap();
};

struct TileSet
{
	int firstgid; // Có thể dùng để đại diện tileset (ID tileset)
	Point tileSize;
	int tileCount;
	int columns;
	Texture texture;
	std::string textureID;
	void Clear()
	{
		CTextureManager::GetInstance()->ClearTextureById(textureID);
	}
};

struct Layer
{
	int id;
	int width;
	int height;
	int** tiles; // ma trận
	bool isVisible;
	void Clear()
	{
		for (int i = 0; i < width; i++)
		{
			delete[] tiles[i];
		}
		delete[] tiles;
	}
};