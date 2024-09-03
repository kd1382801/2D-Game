#pragma once

struct Object
{
	float x;
	float y;
	float angle;
	Math::Matrix mat;

};

class Scene
{
private:
	KdTexture playerTex;	//‰æ‘œ‚Í\‘¢‘Ì‚ÉŠÜ‚ß‚È‚¢
	KdTexture enemyTex;

	Object player;
	Object enemy;

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
