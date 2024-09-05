#pragma once

struct Chara {
	float x;
	float y;
	float moveSpeed;
	Math::Matrix mat;
	KdTexture tex;
};

class Scene
{
private:

	struct Chara m_player;

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

	//プレイヤー用
	void PlayerInit();
	void PlayerUpdate();
	void PlayerDraw();

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
