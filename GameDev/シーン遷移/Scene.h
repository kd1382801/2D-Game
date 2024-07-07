#pragma once

//�V�[�����
enum SceneType
{
	Title,	//0:�^�C�g��
	Game,	//1:�Q�[��
	Result	//2:���U���g
};

class Scene
{
private:

	//�V�[���Ǘ��ϐ�
	SceneType nowScene;

	//�^�C�g���p�ϐ�
	float titleX;
	float titleY;

	//�Q�[���p�ϐ�
	float gameX;
	float gameY;

	//���U���g�p�ϐ�
	float ResultX;
	float ResultY;

	//�L�[�t���O
	bool KeyFlg;

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

	//�V�[���J��
	void ChangeScene();

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
