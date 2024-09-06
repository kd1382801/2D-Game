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

struct tBullet
{
	float m_posX;
	float m_posY;
	bool m_bActive;			//�t���O
	Math::Matrix m_matrix;
	KdTexture* m_pTexture;	//�e�N�X�`��(�|�C���^)
};

class Scene
{
private:
	//���@
	struct tPlayer player;

	//�Ə���
	struct tSight sight;

	//�e
	static const int bulletNum = 50;
	struct tBullet bullet[bulletNum];
	KdTexture charaTex;//���[�h�����Ɏg��

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

	//�e�p
	void InitBullet();
	void UpdateBullet();
	void DrawBullet();

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
