#pragma once

struct Object {
	float x;
	float y;
	Math::Matrix mat;
};

struct Wipe {
	float x;
	float y;
	float scaleX;	//�g�嗦
	float scaleY;	//�g�嗦
	Math::Matrix mat;
	Math::Matrix transMat;	//�ړ��s��
	Math::Matrix scaleMat;	//�g�k�s��
};

class Scene
{
private:

	// �e�N�X�`�� �E�E�E �摜�f�[�^
	KdTexture playerTex;
	KdTexture backTex;
	KdTexture tmpTex;	//�`���(�����_�[�^�[�Q�b�g)�؂芷���p

	Object player;
	Object back;

	//���C�v�\���p
	Wipe wipe;	

	//��ʕ���
	Math::Matrix leftMat;	//�������p
	Math::Matrix rightMat;	//�E�����p
	float cutX;				//���ꂼ��̉�ʂ�X���W


public:

	// �����ݒ�
	void Init();

	// ���
	void Release();

	// �X�V����
	void Update();

	//�e�N�X�`���ւ̕`�揈��
	void DynamicDraw2D();

	// �o�b�N�o�b�t�@�ւ̕`�揈��
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
