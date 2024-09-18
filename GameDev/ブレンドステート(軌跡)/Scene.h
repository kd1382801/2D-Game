#pragma once

struct Object {
	float x;
	float y;
	float moveX;
	float moveY;
	float speed;
	bool  flg;
	int	  timer;	//��������
	Math::Matrix mat;
};


class Scene
{
private:

	KdTexture backTex;
	KdTexture sunTex;

	//���z
	static const int SUN_MAX = 700;
	Object sun[SUN_MAX];
	int sunNum;	//���݁@�X�V�E�`�悳��Ă��鑾�z�̐�

	//�w�i
	Math::Matrix backMat;

	//�u�����h���[�h�ؑ�
	bool addFlg;

	//�}�E�X�p
	POINT mousePos;

	//���[�h�؂�ւ�
	int mode;

public:

	//���z�������֐�
	void SunInit();

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
