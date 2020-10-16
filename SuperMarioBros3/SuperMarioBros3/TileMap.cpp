#include "TileMap.h"
#include "Ultis.h"

CTileMap::CTileMap()
{
	tileWidth = 1;
	tileHeight = 1;
	width = 1;
	height = 1;
}

CTileMap::CTileMap(int width, int height, int tileWidth, int tileHeight)
{
    this->width = width;
    this->height = height;
    this->tileHeight = tileHeight;
    this->tileWidth = tileWidth;
}

LPTileset CTileMap::GetTileSetByTileID(int id)
{
    return floor_entry(listTilesets, id).second;
}

void CTileMap::AddTileSet(int firstgid, LPTileset tileSet)
{
    this->listTilesets[firstgid] = tileSet;
}

void CTileMap::AddLayer(LPLayer layer)
{
    this->listLayers.push_back(layer);
}

void CTileMap::Render(CCamera* camera)
{
	int col = camera->GetPositionCam().x / tileWidth;
	int row = camera->GetPositionCam().y / tileHeight;

	if (col > 0) col--;
	if (row > 0) row--;

	D3DXVECTOR2 camSize = D3DXVECTOR2(camera->GetWidthCam() / tileWidth, camera->GetHeightCam() / tileHeight);

	for (int i = col; i < camSize.x + col + 2; i++) {
		for (int j = row; j < camSize.y + row + 2; j++) {

			int x = i * tileWidth - camera->GetPositionCam().x;
			int y = j * tileHeight - camera->GetPositionCam().y;

			for (LPLayer layer : listLayers) {
				int id = layer->GetTileID(i % width, j % height);
				this->GetTileSetByTileID(id)->Draw(id, D3DXVECTOR2(x,y));
			}
		}
	}
}

CTileMap* CTileMap::FromTMX(std::string filePath, std::string fileName)
{
	string fullPath = filePath + fileName;
	TiXmlDocument doc(fullPath.c_str());

	if (doc.LoadFile()) {
		OutputDebugString(L"Loading TMX \n");
		TiXmlElement* root = doc.RootElement();
		CTileMap* gameMap = new CTileMap();

		root->QueryIntAttribute("width", &gameMap->width);
		root->QueryIntAttribute("height", &gameMap->height);
		root->QueryIntAttribute("tilewidth", &gameMap->tileWidth);
		root->QueryIntAttribute("tileheight", &gameMap->tileHeight);

		//Load tileset
		for (TiXmlElement* node = root->FirstChildElement("tileset"); node != nullptr; node = node->NextSiblingElement("tileset")) {
			CTileset* tileSet = new CTileset(node, filePath);
			gameMap->listTilesets[tileSet->GetFirstgid()] = tileSet;
		}
		//Load layer
		for (TiXmlElement* node = root->FirstChildElement("layer"); node != nullptr; node = node->NextSiblingElement("layer")) {
			LPLayer layer = new CLayer(node);
			gameMap->AddLayer(layer);
		}

		return gameMap;
	}

	return nullptr;
}

CTileMap::~CTileMap()
{
}
