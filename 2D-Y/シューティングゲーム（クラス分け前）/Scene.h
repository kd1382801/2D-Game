#pragma once

struct Chara {
	float x;
	float y;
	float moveSpeed;
	Math::Matrix mat;
	KdTexture tex;
};

class Scene
{
private:

	struct Chara m_player;

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

	//�v���C���[�p
	void PlayerInit();
	void PlayerUpdate();
	void PlayerDraw();

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