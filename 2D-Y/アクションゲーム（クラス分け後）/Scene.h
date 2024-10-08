#pragma once
#include "Player.h"
#include "Map.h"

class Scene
{
private:

	C_Player m_player;
	KdTexture m_playerTex;

	C_Map m_map;
	KdTexture m_mapTex;

public:

	// 初期設定
	void Init();

	// 解放
	void Release();

	// 更新処理
	void Update();

	// 描画処理
	void Draw2D();

	// GUI処理
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
