#pragma once

//��ʒ[�p�̒萔
const int SCREEN_LEFT = -640;
const int SCREEN_RIGHT = 640;
const int SCREEN_TOP = 360;
const int SCREEN_BOTTOM = -360;


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
	float m_moveX;
	float m_moveY;
	float m_speed;
	int m_count;				//����Ǘ��p�J�E���^
	bool m_bHoming;				//�z�[�~���O�t���O
	bool m_bActive;				//�t���O
	Math::Matrix m_matrix;
	KdTexture* m_pTexture;		//�e�N�X�`��(�|�C���^)
	Math::Rectangle m_srcRect;	//�e�N�X�`�����W
	Math::Color m_color;		//�FRGBA

};

struct tEnemy
{
	float m_posX;
	float m_posY;
	float m_moveX;
	float m_moveY;
	float m_radius;
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

	//�G
	struct tEnemy enemy;

	//�e
	static const int bulletNum = 200;
	struct tBullet bullet[bulletNum];
	KdTexture charaTex;//���[�h�����Ɏg��

	//�e�̔��ˊԊu����p
	static const int shotInterval = 5;
	int keyCount;

	bool debugFlg;//�f�o�b�O�p

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

	//�G�p
	void InitEnemy();
	void UpdateEnemy();
	void DrawEnemy();

	//�e�p
	void InitBullet();
	void UpdateBullet();
	void DrawBullet();
	void ShotBullet();
	void ShotBulletDS(float a_degree,float a_speed);

	//�p�x�����߂�֐�			(src:source���A��)	(dest:destination�ړI�n�A�s����)
	float GetAngleRad(float srcX, float srcY, float destX, float destY);
	float GetAngleDeg(float srcX, float srcY, float destX, float destY);

	//���������߂�֐�
	float GetDistance(float srcX, float srcY, float destX, float destY);

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
