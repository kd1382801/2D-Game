#pragma once

//シーン種類
enum SceneType
{
	Title,		//0:タイトル
	Game,		//1:ゲーム
	Result,		//2:リザルト
	HowtoPlay	//3:遊び方
};

enum My {
	Red = 1 << 0 ,
	Green = 1 << 1,
	Blue = 1 << 2,
	GcMoveFloor = 1<<0,	//動く床に当たった
	GcTop = 1<<1,		//天井に当たった
	GcHit = 3			//両方に当たった
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
	int gcFlg;//挟まったか用フラグ
};

struct Block{
	float x;
	float y;
	float moveX;
	float moveY;
	int flg;//マップに対応する色
	bool cFlg;//キャッチ用フラグ
	bool jumpFlg;//ジャンプ用フラグ
	int gcFlg;//挟まったか用フラグ
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

struct MoveFloor {
	float x;
	float y;
	float baseX;
	float moveY;
	bool flg;
};

struct BlockXY {
	float bX;
	float bY;
};

struct Room
{
	int number;//部屋番号
	float pX;//初期値x
	float pY;//初期値y
	BlockXY bl[3];
	int clearScore;//クリアするために必要なスコア
};

struct KeyFlg {
	bool enter;//エンターキーフラグ
	bool shift;//シフトキーフラグ
	bool p;//シフトキーフラグ
};

struct RE {
	float x;
	float y;
	bool flg;
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

	KeyFlg keyflg;


	//シーン関連
	SceneType nowScene;//シーン管理変数
	bool titleGameCflg;//titleGameChangeFlg
	bool titleHtpCflg;//titleHowtoPlayChangeFlg
	bool resultTitleCflg;
	bool htpTitleCflg;

	//タイトル用
	Base arrow;//矢印用
	bool arrowKeyFlg;//矢印用キーフラグ

	int frame;//フレーム数
	int clearFrame;//クリアタイム用フレーム
	const float gravity = 1.0f;//重力

	//プレイヤー用
	const float playerRadius = 20.0f;
	static const int jumpPower = 18;
	Player player;
	int playerHorizontal;//キャラの向き 1:右 -1:左
	
	//ブロック用
	static const int blockNum = 3;
	const float blockRadius = 20.0f;
	Block block[blockNum];
	int cflg;

	//動く床用
	static const int mFWidth = 70;	//動く床の幅
	static const int mFHeight = 10;	//動く床の高さ
	static const int mFNum = 17;
	MoveFloor moveFloor[mFNum];

	//ステージ用
	static const int mapHeight = 13;
	static const int mapWidth = 70;
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
	int score2; //表示用
	bool maxScoreFlg;//最高スコア更新用フラグ
	bool nextRoomFlg;
	bool goNextRoomFlg;

	//HowtoPlay用
	int htpScene;//HowtoPlayシーン管理用
	int enterFrame;//エンターキーが押されたフレーム
	static const int htpLastScene = 12;
	bool htpChangeStage;

	//Result用エフェクト
	static const int rENum = 500;
	RE rE[rENum];

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
	
	//遊び方描画
	void DrawHtP();
	//遊び方更新
	void UpdateHtP();

	//プレイヤー初期化
	void InitPlayer();
	//プレイヤー更新
	void UpdatePlayer();
	//プレイヤー描画
	void DrawPlayer();

	//マップ当たり判定(blockならtrueと色を書く)
	void MapHit(float* x, float* y, float* moveX, float* moveY, int* gcFlg = 0, bool block = false, int blockColor = 0, bool cFlg = false);

	//キャラとブロックの当たり判定(白のときだけ動作)
	void BlockHit(float* x, float* y, float* moveX, float* moveY, bool bl = false,int blockNum=0);

	//動く床との当たり判定
	void MoveFloorHit(float* x, float* y, float* moveX, float* moveY, int* gcFlg = 0);
	
	//ブロック初期化
	void InitBlock();
	//ブロック更新
	void UpdateBlock();
	//ブロック描画
	void DrawBlock();

	//動く床初期化
	void InitMoveFloor();
	//動く床更新
	void UpdateMoveFloor();
	//動く床描画
	void DrawMoveFloor();

	//最高スコアチェック
	void CheckScore();

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
	//htp用文字
	void DrawHtpString();

	//Result用エフェクト
	void DrawREffect();
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
