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

	// �����ݒ�
	void Init();

	// ���
	void Release();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw2D();

	// GUI����
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
