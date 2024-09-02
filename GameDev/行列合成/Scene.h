#pragma once

class Scene
{
private:

	float playerX;
	float playerY;
	float angle;
	float size;
	Math::Matrix playerMat;
	Math::Matrix playerTransMat;
	Math::Matrix playerScaleMat;
	Math::Matrix playerRotMat;
	KdTexture playerTex;

public:

	// ‰Šúİ’è
	void Init();

	// ‰ğ•ú
	void Release();

	// XVˆ—
	void Update();

	// •`‰æˆ—
	void Draw2D();

	// GUIˆ—
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
