#pragma once

struct Object {
	float x;
	float y;
	float moveX;
	float moveY;
	float speed;
	bool  flg;
	int	  timer;	//生存時間
	Math::Matrix mat;
};


class Scene
{
private:

	KdTexture backTex;
	KdTexture sunTex;

	//太陽
	static const int SUN_MAX = 700;
	Object sun[SUN_MAX];
	int sunNum;	//現在　更新・描画されている太陽の数

	//背景
	Math::Matrix backMat;

	//ブレンドモード切替
	bool addFlg;

	//マウス用
	POINT mousePos;

	//モード切り替え
	int mode;

public:

	//太陽初期化関数
	void SunInit();

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
