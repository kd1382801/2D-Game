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
