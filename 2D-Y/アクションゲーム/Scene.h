#pragma once

struct Chara 
{
	float x;
	float y;
	float moveX;
	float moveY;
	float rotation;
	float scaleX;
	Math::Matrix scaleMat;
	Math::Matrix rotationMat;
	Math::Matrix transMat;
	Math::Matrix mat;
	Math::Color color;
};

class Scene
{
private:
	
	struct Chara player;

	//�G
	static const int enemyNum = 3;
	struct Chara enemy[enemyNum];
	int moveCount[enemyNum];//�G�����]����J�E���g


	// �e�N�X�`��
	KdTexture charaTex;

	const float Gravity = 1.0f;		//�d��
	const float JumpPower = 20.0f;	//�W�����v��

	bool playerJumpFlg;//�v���C���[�̃W�����v�t���O

	FILE* fp_c;

	//�}�b�v
	static const int mapHeight = 12;	//�c
	static const int mapWidth = 32;		//��
	int mapData[mapHeight][mapWidth];
	KdTexture mapTex;					//�}�b�v�摜
	Math::Matrix mapMat[mapHeight][mapWidth];
	float mapX[mapHeight][mapWidth];	//X���W
	float mapY[mapHeight][mapWidth];	//Y���W

	//�����蔻��
	const float charaRadius = 32.0f;
	const float mapRadius = 32.0f;

	//�X�N���[������
	float scrollX;	//�X�N���[���̈ʒu�Ǘ�
	float scrollMin;//�X�N���[���̍ŏ�
	float scrollMax;//�X�N���[���̍ő�

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

	//alt + Enter�Œ�`���쐬�ł���
	//�R���\�[���쐬
	void CreateConsole();

	//�R���\�[���j��
	void DestroyConsole();

	//�}�b�v�̃��[�h
	void LoadMap();

	//�}�b�v�ƃL�����̓����蔻��(��`�����蔻��)
	void PlayerMapHit();

	//�G������
	void EnemyInit();

	//�G�X�V
	void EnemyUpdate();

	//�G�`��
	void EnemyDraw();

	//�v���C���[�ƓG�̓����蔻��
	void PlayerEnemyHit();


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
