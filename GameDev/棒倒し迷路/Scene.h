#pragma once

//�_��|������
enum Direction
{
	Up,		//��
	Down,	//��
	Left,	//��
	Right,	//�E
	Kind	//��ސ�
};

class Scene
{
private:

	// �e�N�X�`�� �E�E�E �摜�f�[�^
	KdTexture blockTex;


	// �s�� �E�E�E ���W�Ȃǂ̏��
	Math::Matrix matrix;

	//���H�̍���
	static const int MAZE_HEIGHT = 11;

	//���H�̕�
	static const int MAZE_WIDTH = 21;

	//���H�t���O
	int mazeFlg[MAZE_HEIGHT][MAZE_WIDTH];

	//�L�[����p�t���O(true;������Ă��� false:������Ă���)
	bool KeyFlg;

	//�|������
	Direction dir;	//0:�� 1:�� 2:�� 3:�E
					//Direction�^�̕ϐ�

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

	//���H�̏���������
	void InitMaze();

	//���H��������
	void CreateMaze();

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
