#pragma once

//シーン種類
enum SceneType
{
	Title,	//0:タイトル
	Game,	//1:ゲーム
	Result	//2:リザルト
};

enum My {
	Red = 1 << 0 ,
	Blue = 1 << 1,
	Green = 1 << 2
};

struct Base {
	float x;
	float y;
	float moveX;
	float moveY;
};

struct Player{
	float x;
	float y;
	float moveX;
	float moveY;
	bool jumpFlg;
};

struct Block{
	float x;
	float y;
	float moveX;
	float moveY;
	int flg;//マップに対応する色
};

struct Scroll {
	float X;
	float Y;
	float XMin;//左端
	float XMax;//右端
	float YMin;//下端
	float YMax;//上端
};

struct Map {
	float x;
	float y;
	float baseX;
	float baseY;
	int flg;
};

struct Room
{
	int number;//部屋番号
	float pX;//初期値x
	float pY;//初期値y
	float bX;//ブロックの初期値x
	float bY;//ブロックの初期値y
	int clearScore;//クリアするために必要なスコア
};

class Scene
{
private:

	static const int ScreenTop = 180;//画面上端
	static const int ScreenBottom = -180;//画面下端
	static const int ScreenLeft = -320;//画面左端
	static const int ScreenRight = 320;//画面右端

	Math::Color color;
	Math::Color wcolor;//白
	Math::Color bcolor;//黒

	SceneType nowScene;//シーン管理変数
	int frame;//フレーム数
	int clearFrame;//クリアタイム用フレーム
	const float gravity = 1.0f;//重力

	//プレイヤー用
	const float playerRadius = 20.0f;
	static const int jumpPower = 18;
	Player player;
	int playerHorizontal;//キャラの向き 1:右 -1:左

	//ブロック用
	const float blockRadius = 20.0f;
	Block block;
	bool blockJumpFlg;//ブロック用ジャンプフラグ
	bool shiftKeyFlg;
	int cflg;

	//ステージ用
	static const int mapHeight = 13;
	static const int mapWidth = 64;
	const float mapRadius = 20.0f;
	Map mapChip[mapHeight][mapWidth];
	bool changeStageFlg;//0:白 1:黒
	//ステージスクロール用
	Scroll scroll;

	//フェードインアウト用
	int fadeFrame;
	bool doFadeInOut; // 0:停止 1:実行
	
	//部屋情報
	static const int roomNum = 3;
	Room room[roomNum];
	int nowRoom;//今の部屋
	int score;	//今のスコア


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

	//タイトル描画
	void DrawTitle();
	//タイトル更新
	void UpdateTitle();

	//ゲーム描画
	void DrawGame();
	//ゲーム更新
	void UpdateGame();

	//リザルト描画
	void DrawResult();
	//リザルト更新
	void UpdateResult();

	//プレイヤー初期化
	void InitPlayer();
	//プレイヤー更新
	void UpdatePlayer();
	//プレイヤー描画
	void DrawPlayer();

	//マップ当たり判定(blockならtrueと色を書く)
	void MapHit(float* x, float* y, float* moveX, float* moveY,bool block=false,int blockColor=0);

	//キャラとブロックの当たり判定(白のときだけ動作)
	void BlockHit();

	//ブロック初期化
	void InitBlock();
	//ブロック更新
	void UpdateBlock();
	//ブロック描画
	void DrawBlock();

	//部屋情報読み込み
	void LoadRoom();
	//次の部屋に移動
	void NextRoom();

	//マップ初期化
	void InitMap();
	//マップ読み込み
	void LoadMap();
	//マップ更新
	void UpdateMap();
	//マップ描画
	void DrawMap();


	//フェードインアウト(bool型のフラグを変える)
	void FadeInOut(bool* flg = false);


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
