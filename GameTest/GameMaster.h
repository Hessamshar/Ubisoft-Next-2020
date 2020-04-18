#ifndef _GAMEMASTER_H_
#define _GAMEMASTER_H_

#include <vector>
#include <memory>
#include "TriangleBase.h"

#define MAX_NUMBER_OF_TRIANGLES		(30)

class GameMaster
{
public:
	GameMaster(const GameMaster&) = delete;
	GameMaster(GameMaster&&) = delete;
	GameMaster& operator=(const GameMaster&) = delete;
	GameMaster& operator=(GameMaster&&) = delete;
	static GameMaster* getInstance();
	void CreateMap(int num_triangles);
	void Draw();
private:
	GameMaster();
	static GameMaster *instance;
	std::vector<std::unique_ptr<TriangleBase>> m_triangles;
	int m_current = 0;
};

#endif
