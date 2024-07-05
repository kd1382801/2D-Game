#pragma once

struct Chara 
{
	float x;
	float y;
	float moveX;
	float moveY;
	float rotation;
	float scaleX;
	Math::Matrix scaleMat;
	Math::Matrix rotationMat;
	Math::Matrix transMat;
	Math::Matrix mat;
};

class Scene
{
private:
	
	struct Chara player;

	struct Chara enemy;
	int moveCount;//敵が反転するカウント


	// テクスチャ ・・・ 画像データ
	KdTexture charaTex;

	const float Gravity = 1.0f;		//重力
	const float JumpPower = 20.0f;	//ジャンプ力

	bool playerJumpFlg;//プレイヤーのジャンプフラグ

	FILE* fp_c;

	//マップ
	static const int mapHeight = 12;	//縦
	static const int mapWidth = 32;		//横
	int mapData[mapHeight][mapWidth];
	KdTexture mapTex;					//マップ画像
	Math::Matrix mapMat[mapHeight][mapWidth];
	float mapX[mapHeight][mapWidth];	//X座標
	float mapY[mapHeight][mapWidth];	//Y座標

	//当たり判定
	const float charaRadius = 32.0f;
	const float mapRadius = 32.0f;

	//スクロール処理
	float scrollX;	//スクロールの位置管理
	float scrollMin;//スクロールの最小
	float scrollMax;//スクロールの最大

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

	//alt + Enterで定義を作成できる
	//コンソール作成
	void CreateConsole();

	//コンソール破棄
	void DestroyConsole();

	//マップのロード
	void LoadMap();

	//マップとキャラの当たり判定(矩形当たり判定)
	void PlayerMapHit();

	//敵初期化
	void EnemyInit();

	//敵更新
	void EnemyUpdate();

	//敵描画
	void EnemyDraw();

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
