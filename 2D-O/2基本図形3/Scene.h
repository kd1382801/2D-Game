#pragma once

class Scene
{
private:

	static const int screenTop    =  180;
	static const int screenBottom = -180;
	static const int screenLeft   = -320;
	static const int screenRight  =  320;

	//色
	Math::Color color;

	float size;//サイズ(半径)
	float degree;//角度
	
	//フレーム数
	int frame;

	
public:

	// 初期設定
	void Init();

	// 解放
	void Release();

	// 更新処理
	void Update();

	// 描画処理
	void Draw2D();

	//オリジナルの円描画関数
	void DrawCircleEx(float cx, float cy, float radiusX, float radiusY, Math::Color* pColor);

	//多角形描画関数
	void DrawPolygon(float cx, float cy, float radiusX, float radiusY, float vertexNum = 3, float startDeg = 90,
											Math::Color* pColor = &Math::Color(1,1,1,1), bool paintFlg = true);

	//花火
	void DrawFireworks(float cx, float cy);

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
