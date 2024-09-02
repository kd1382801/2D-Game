#pragma once

class Scene
{
private:

	float playerX;
	float playerY;
	float angle;
	float size;
	Math::Matrix playerMat;
	Math::Matrix playerTransMat;
	Math::Matrix playerScaleMat;
	Math::Matrix playerRotMat;
	KdTexture playerTex;

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
