#pragma once

//�V�[�����
enum SceneType
{
	Title,		//0:�^�C�g��
	Game,		//1:�Q�[��
	Result,		//2:���U���g
	HowtoPlay	//3:�V�ѕ�
};

enum My {
	Red = 1 << 0 ,
	Green = 1 << 1,
	Blue = 1 << 2,
	GcMoveFloor = 1<<0,	//�������ɓ�������
	GcTop = 1<<1,		//�V��ɓ�������
	GcHit = 3			//�����ɓ�������
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
	int gcFlg;//���܂������p�t���O
};

struct Block{
	float x;
	float y;
	float moveX;
	float moveY;
	int flg;//�}�b�v�ɑΉ�����F
	bool cFlg;//�L���b�`�p�t���O
	bool jumpFlg;//�W�����v�p�t���O
	int gcFlg;//���܂������p�t���O
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

struct MoveFloor {
	float x;
	float y;
	float baseX;
	float moveY;
	bool flg;
};

struct BlockXY {
	float bX;
	float bY;
};

struct Room
{
	int number;//�����ԍ�
	float pX;//�����lx
	float pY;//�����ly
	BlockXY bl[3];
	int clearScore;//�N���A���邽�߂ɕK�v�ȃX�R�A
};

struct KeyFlg {
	bool enter;//�G���^�[�L�[�t���O
	bool shift;//�V�t�g�L�[�t���O
	bool p;//�V�t�g�L�[�t���O
};

struct RE {
	float x;
	float y;
	bool flg;
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

	KeyFlg keyflg;


	//�V�[���֘A
	SceneType nowScene;//�V�[���Ǘ��ϐ�
	bool titleGameCflg;//titleGameChangeFlg
	bool titleHtpCflg;//titleHowtoPlayChangeFlg
	bool resultTitleCflg;
	bool htpTitleCflg;

	//�^�C�g���p
	Base arrow;//���p
	bool arrowKeyFlg;//���p�L�[�t���O

	int frame;//�t���[����
	int clearFrame;//�N���A�^�C���p�t���[��
	const float gravity = 1.0f;//�d��

	//�v���C���[�p
	const float playerRadius = 20.0f;
	static const int jumpPower = 18;
	Player player;
	int playerHorizontal;//�L�����̌��� 1:�E -1:��
	
	//�u���b�N�p
	static const int blockNum = 3;
	const float blockRadius = 20.0f;
	Block block[blockNum];
	int cflg;

	//�������p
	static const int mFWidth = 70;	//�������̕�
	static const int mFHeight = 10;	//�������̍���
	static const int mFNum = 17;
	MoveFloor moveFloor[mFNum];

	//�X�e�[�W�p
	static const int mapHeight = 13;
	static const int mapWidth = 70;
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
	int score2; //�\���p
	bool maxScoreFlg;//�ō��X�R�A�X�V�p�t���O
	bool nextRoomFlg;
	bool goNextRoomFlg;

	//HowtoPlay�p
	int htpScene;//HowtoPlay�V�[���Ǘ��p
	int enterFrame;//�G���^�[�L�[�������ꂽ�t���[��
	static const int htpLastScene = 12;
	bool htpChangeStage;

	//Result�p�G�t�F�N�g
	static const int rENum = 500;
	RE rE[rENum];

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
	
	//�V�ѕ��`��
	void DrawHtP();
	//�V�ѕ��X�V
	void UpdateHtP();

	//�v���C���[������
	void InitPlayer();
	//�v���C���[�X�V
	void UpdatePlayer();
	//�v���C���[�`��
	void DrawPlayer();

	//�}�b�v�����蔻��(block�Ȃ�true�ƐF������)
	void MapHit(float* x, float* y, float* moveX, float* moveY, int* gcFlg = 0, bool block = false, int blockColor = 0, bool cFlg = false);

	//�L�����ƃu���b�N�̓����蔻��(���̂Ƃ���������)
	void BlockHit(float* x, float* y, float* moveX, float* moveY, bool bl = false,int blockNum=0);

	//�������Ƃ̓����蔻��
	void MoveFloorHit(float* x, float* y, float* moveX, float* moveY, int* gcFlg = 0);
	
	//�u���b�N������
	void InitBlock();
	//�u���b�N�X�V
	void UpdateBlock();
	//�u���b�N�`��
	void DrawBlock();

	//������������
	void InitMoveFloor();
	//�������X�V
	void UpdateMoveFloor();
	//�������`��
	void DrawMoveFloor();

	//�ō��X�R�A�`�F�b�N
	void CheckScore();

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
	//htp�p����
	void DrawHtpString();

	//Result�p�G�t�F�N�g
	void DrawREffect();
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
