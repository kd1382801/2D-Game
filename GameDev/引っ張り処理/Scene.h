#pragma once

class Scene
{
private:

	//�R���\�[���p
	FILE* fp_c;

	//��ʂ̃T�C�Y
	static const int WIDTH = 390;	//��
	static const int HEIGHT = 780;	//����

	//�}�E�X�p(POINT:�����o��X��Y�����\����)
	POINT mouse;		//���t���[���}�E�X���W���擾
	POINT clickPos;		//�N���b�N�����u�Ԃ̃}�E�X���W
	POINT releasePos;	//�N���b�N�𗣂����u�Ԃ̃}�E�X���W
	bool clickFlg;		//�N���b�N����
	
	//�L����
	KdTexture charaTex;
	Math::Matrix charaMat;
	float charaX;
	float charaY;
	float moveX;
	float moveY;
	float angle;
	float speed;

	//�G
	KdTexture enemyTex;
	Math::Matrix enemyMat;
	float enemyX;
	float enemyY;

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

	//�}�E�X���W�擾
	POINT GetMousePos();
	void GetMousePos2(POINT *mousePos);

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
