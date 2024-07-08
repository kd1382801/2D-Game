#pragma once

class Scene
{
private:

	//マウス用(POINT:メンバにXとYを持つ構造体)
	POINT mouse;
	
	//キャラ
	KdTexture charaTex;
	Math::Matrix charaMat;
	float charaX;
	float charaY;


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

	//マウス座標取得
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
