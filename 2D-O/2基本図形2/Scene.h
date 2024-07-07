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

	//星空用
	static const int starNum = 100;
	float starX[starNum];
	float starY[starNum];

	//三角形用
	float triX1;
	float triY1;
	float triX2;
	float triY2;
	float triX3;
	float triY3;

	float triMoveX1;
	float triMoveY1;
	float triMoveX2;
	float triMoveY2;
	float triMoveX3;
	float triMoveY3;

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

	//星空用
	void InitStars();
	void UpdateStars();
	void DrawStars();

	//三角形用
	void InitTri();
	void UpdateTri();
	void DrawTri();

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
