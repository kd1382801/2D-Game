#pragma once
#include "Player.h"
#include "Enemy.h"

class Scene
{
private:

	C_Player m_player;
	C_Enemy m_enemy;

	KdTexture m_playerTex;
	KdTexture m_enemyTex;

public:

	// ‰Šúİ’è
	void Init();

	// ‰ğ•ú
	void Release();

	// XVˆ—
	void Update();

	// •`‰æˆ—
	void Draw2D();

	// GUIˆ—
	void ImGuiUpdate();

private:

	Scene() {}

public:
	static Scene& GetInstance()
	{
		static Scene instance;
		return instance;
	}
};

#define SCENE Scene::GetInstance()
