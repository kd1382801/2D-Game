#pragma once

class Scene
{
private:

	//画面端用定数
	static const int  screenLeft   = -320;
	static const int  screenTop    =  180;
	static const int  screenRight  =  320;
	static const int  screenBottom = -180;

	//自機
	float playerX;
	float playerY;

	//敵
	float enemyX;
	float enemyY;
	float enemyMoveX;
	float enemyMoveY;

	//円型のエフェクト
	float circleX;
	float circleY;
	float circleRadius;		//半径
	Math::Color circleColor;//色
	bool circleFlg;			//フラグ

	//色
	Math::Color color;


	//フレーム数
	int frame;

	//ゲームの起伏用
	int gameStep;

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

	//自機用
	void InitPlayer();
	void UpdatePlayer();
	void DrawPlayer();

	//敵用
	void InitEnemy();
	void UpdateEnemy();
	void DrawEnemy();

	//エフェクト用
	void InitCircleEffect();
	void UpdateCircleEffect();
	void DrawCircleEffect();

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
