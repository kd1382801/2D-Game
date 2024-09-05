#pragma once

struct tPlayer {
	float m_posX;
	float m_posY;
	Math::Matrix m_matrix;
	KdTexture m_texture;
};

struct tSight {
	float m_posX;
	float m_posY;
	Math::Matrix m_matrix;
	KdTexture m_texture;
};

class Scene
{
private:
	//���@
	struct tPlayer player;

	//�Ə���
	struct tSight sight;

	//���p�O�p�`
	float a;	//���
	float b;	//����
	float c;	//�Ε�
	float deg;	//�p�x


	//�t���[����
	int frame;

	
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

	//���@�p
	void InitPlayer();
	void UpdatePlayer();
	void DrawPlayer();

	//�Ə���p
	void InitSight();
	void UpdateSight();
	void DrawSight();

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
