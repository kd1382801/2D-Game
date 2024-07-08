#pragma once

class Scene
{
private:

	//�}�E�X�p(POINT:�����o��X��Y�����\����)
	POINT mouse;
	
	//�L����
	KdTexture charaTex;
	Math::Matrix charaMat;
	float charaX;
	float charaY;


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
