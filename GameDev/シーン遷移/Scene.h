#pragma once

//シーン種類
enum SceneType
{
	Title,	//0:タイトル
	Game,	//1:ゲーム
	Result	//2:リザルト
};

class Scene
{
private:

	//シーン管理変数
	SceneType nowScene;

	//タイトル用変数
	float titleX;
	float titleY;

	//ゲーム用変数
	float gameX;
	float gameY;

	//リザルト用変数
	float ResultX;
	float ResultY;

	//キーフラグ
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

	//シーン遷移
	void ChangeScene();

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
