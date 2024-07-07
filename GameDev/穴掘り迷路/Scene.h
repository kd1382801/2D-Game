#pragma once

//棒を倒す方向
enum Direction
{
	Up,		//上
	Down,	//下
	Left,	//左
	Right,	//右
	Kind,	//種類数
	None=999//掘っていない
};

class Scene
{
private:

	// テクスチャ ・・・ 画像データ
	KdTexture blockTex;
	KdTexture charaTex;

	// 行列 ・・・ 座標などの情報
	Math::Matrix matrix;

	//迷路の高さ
	static const int MAZE_HEIGHT = 11;

	//迷路の幅
	static const int MAZE_WIDTH = 21;

	//迷路フラグ
	int mazeFlg[MAZE_HEIGHT][MAZE_WIDTH];

	//キー制御用フラグ(true;押されている false:離されている)
	bool KeyFlg;

	//倒す方向
	Direction dir;	//0:上 1:下 2:左 3:右
	//Direction型の変数

	//現在のキャラの位置
	int x;
	int y;

	//掘れる道があるか
	bool digFlg;	//掘れる:true 掘れない(行き止まり):false

	//掘った方向の最大セーブ数
	static const int SAVE_MAX = 200;

	//掘った履歴
	Direction save[SAVE_MAX];

	//掘った回数
	int digCnt;


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

	//迷路の初期化処理
	void InitMaze();

	//迷路生成処理
	void CreateMaze();

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
