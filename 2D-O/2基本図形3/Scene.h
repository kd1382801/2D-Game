#pragma once

class Scene
{
private:

	static const int screenTop    =  180;
	static const int screenBottom = -180;
	static const int screenLeft   = -320;
	static const int screenRight  =  320;

	//�F
	Math::Color color;

	//����p
	static const int starNum = 100;
	float starX[starNum];
	float starY[starNum];

	//�O�p�`�p
	float triX1;
	float triY1;
	float triX2;
	float triY2;
	float triX3;
	float triY3;

	float triMoveX1;
	float triMoveY1;
	float triMoveX2;
	float triMoveY2;
	float triMoveX3;
	float triMoveY3;

	//�G
	float enemyCx;		//���SX
	float enemyCy;		//���SY
	float enemyRadius;	//���a
	float enemyDeg;		//�p�x

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

	//����p
	void InitStars();
	void UpdateStars();
	void DrawStars();

	//�O�p�`�p
	void InitTri();
	void UpdateTri();
	void DrawTri();

	//�G
	void InitEnemy();
	void UpdateEnemy();
	void DrawEnemy();

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
