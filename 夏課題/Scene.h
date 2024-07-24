#pragma once

//�V�[�����
enum SceneType
{
	Title,	//0:�^�C�g��
	Game,	//1:�Q�[��
	Result	//2:���U���g
};

enum My {
	Red = 1 << 0 ,
	Blue = 1 << 1,
	Green = 1 << 2
};

struct Base {
	float x;
	float y;
	float moveX;
	float moveY;
};

struct Player{
	float x;
	float y;
	float moveX;
	float moveY;
	bool jumpFlg;
};

struct Block{
	float x;
	float y;
	float moveX;
	float moveY;
	int flg;//�}�b�v�ɑΉ�����F
};

struct Scroll {
	float X;
	float Y;
	float XMin;//���[
	float XMax;//�E�[
	float YMin;//���[
	float YMax;//��[
};

struct Map {
	float x;
	float y;
	float baseX;
	float baseY;
	int flg;
};

struct Room
{
	int number;//�����ԍ�
	float pX;//�����lx
	float pY;//�����ly
	float bX;//�u���b�N�̏����lx
	float bY;//�u���b�N�̏����ly
	int clearScore;//�N���A���邽�߂ɕK�v�ȃX�R�A
};

class Scene
{
private:

	static const int ScreenTop = 180;//��ʏ�[
	static const int ScreenBottom = -180;//��ʉ��[
	static const int ScreenLeft = -320;//��ʍ��[
	static const int ScreenRight = 320;//��ʉE�[

	Math::Color color;
	Math::Color wcolor;//��
	Math::Color bcolor;//��

	SceneType nowScene;//�V�[���Ǘ��ϐ�
	int frame;//�t���[����
	int clearFrame;//�N���A�^�C���p�t���[��
	const float gravity = 1.0f;//�d��

	//�v���C���[�p
	const float playerRadius = 20.0f;
	static const int jumpPower = 18;
	Player player;
	int playerHorizontal;//�L�����̌��� 1:�E -1:��

	//�u���b�N�p
	const float blockRadius = 20.0f;
	Block block;
	bool blockJumpFlg;//�u���b�N�p�W�����v�t���O
	bool shiftKeyFlg;
	int cflg;

	//�X�e�[�W�p
	static const int mapHeight = 13;
	static const int mapWidth = 64;
	const float mapRadius = 20.0f;
	Map mapChip[mapHeight][mapWidth];
	bool changeStageFlg;//0:�� 1:��
	//�X�e�[�W�X�N���[���p
	Scroll scroll;

	//�t�F�[�h�C���A�E�g�p
	int fadeFrame;
	bool doFadeInOut; // 0:��~ 1:���s
	
	//�������
	static const int roomNum = 3;
	Room room[roomNum];
	int nowRoom;//���̕���
	int score;	//���̃X�R�A


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

	//�^�C�g���`��
	void DrawTitle();
	//�^�C�g���X�V
	void UpdateTitle();

	//�Q�[���`��
	void DrawGame();
	//�Q�[���X�V
	void UpdateGame();

	//���U���g�`��
	void DrawResult();
	//���U���g�X�V
	void UpdateResult();

	//�v���C���[������
	void InitPlayer();
	//�v���C���[�X�V
	void UpdatePlayer();
	//�v���C���[�`��
	void DrawPlayer();

	//�}�b�v�����蔻��(block�Ȃ�true�ƐF������)
	void MapHit(float* x, float* y, float* moveX, float* moveY,bool block=false,int blockColor=0);

	//�L�����ƃu���b�N�̓����蔻��(���̂Ƃ���������)
	void BlockHit();

	//�u���b�N������
	void InitBlock();
	//�u���b�N�X�V
	void UpdateBlock();
	//�u���b�N�`��
	void DrawBlock();

	//�������ǂݍ���
	void LoadRoom();
	//���̕����Ɉړ�
	void NextRoom();

	//�}�b�v������
	void InitMap();
	//�}�b�v�ǂݍ���
	void LoadMap();
	//�}�b�v�X�V
	void UpdateMap();
	//�}�b�v�`��
	void DrawMap();


	//�t�F�[�h�C���A�E�g(bool�^�̃t���O��ς���)
	void FadeInOut(bool* flg = false);


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
