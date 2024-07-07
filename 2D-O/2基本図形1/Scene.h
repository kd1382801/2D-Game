#pragma once

class Scene
{
private:

	//��ʒ[�p�萔
	static const int  screenLeft   = -320;
	static const int  screenTop    =  180;
	static const int  screenRight  =  320;
	static const int  screenBottom = -180;

	//���@
	float playerX;
	float playerY;

	//�G
	float enemyX;
	float enemyY;
	float enemyMoveX;
	float enemyMoveY;

	//�~�^�̃G�t�F�N�g
	float circleX;
	float circleY;
	float circleRadius;		//���a
	Math::Color circleColor;//�F
	bool circleFlg;			//�t���O

	//�F
	Math::Color color;


	//�t���[����
	int frame;

	//�Q�[���̋N���p
	int gameStep;

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

	//�G�p
	void InitEnemy();
	void UpdateEnemy();
	void DrawEnemy();

	//�G�t�F�N�g�p
	void InitCircleEffect();
	void UpdateCircleEffect();
	void DrawCircleEffect();

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
