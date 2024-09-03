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
	KdTexture playerTex;	//画像は構造体に含めない
	KdTexture enemyTex;

	Object player;
	Object enemy;

public:

	// 初期設定
	void Init();

	// 解放
	void Release();

	// 更新処理
	void Update();

	// 描画処理
	void Draw2D();

	// GUI処理
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
