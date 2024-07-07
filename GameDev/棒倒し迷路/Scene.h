#pragma once

//棒を倒す方向
enum Direction
{
	Up,		//上
	Down,	//下
	Left,	//左
	Right,	//右
	Kind	//種類数
};

class Scene
{
private:

	// テクスチャ ・・・ 画像データ
	KdTexture blockTex;


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
