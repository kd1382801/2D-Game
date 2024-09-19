#pragma once
#include "Player.h"
#include "Enemy.h"

class Scene
{
private:

	POINT m_mousePos;

	C_Player m_player;
	C_Enemy m_enemy;

	KdTexture m_playerTex;
	KdTexture m_enemyTex;

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

	//�}�E�X���W�v�Z
	void CalcMousePos();
	
	C_Enemy* GetEnemy();

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
