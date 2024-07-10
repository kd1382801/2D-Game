#pragma once

class Scene
{
private:

	//コンソール用
	FILE* fp_c;

	//画面のサイズ
	static const int WIDTH = 390;	//幅
	static const int HEIGHT = 780;	//高さ

	//マウス用(POINT:メンバにXとYを持つ構造体)
	POINT mouse;		//毎フレームマウス座標を取得
	POINT clickPos;		//クリックした瞬間のマウス座標
	POINT releasePos;	//クリックを離した瞬間のマウス座標
	bool clickFlg;		//クリック制御
	
	//キャラ
	KdTexture charaTex;
	Math::Matrix charaMat;
	float charaX;
	float charaY;
	float moveX;
	float moveY;
	float angle;
	float speed;

	//敵
	KdTexture enemyTex;
	Math::Matrix enemyMat;
	float enemyX;
	float enemyY;

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
