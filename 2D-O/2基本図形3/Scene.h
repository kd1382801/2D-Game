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

	//�I���W�i���̉~�`��֐�
	void DrawCircleEx(float cx, float cy, float radiusX, float radiusY, Math::Color* pColor);

	//���p�`�`��֐�
	void DrawPolygon(float cx, float cy, float radiusX, float radiusY, float vertexNum, Math::Color* pColor,bool paintFlg);

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
