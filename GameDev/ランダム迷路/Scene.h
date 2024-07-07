#pragma once

class Scene
{
private:

	// テクスチャ ・・・ 画像データ
	KdTexture blockTex;


	// 行列 ・・・ 座標などの情報
	Math::Matrix matrix;

	//迷路の高さ
	static const int MAZE_HEIGHT = 12;

	//迷路の幅
	static const int MAZE_WIDTH = 21;

	//迷路フラグ
	int mazeFlg[MAZE_HEIGHT][MAZE_WIDTH];

	//キー制御用フラグ(true;押されている false:離されている)
	bool KeyFlg;

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
