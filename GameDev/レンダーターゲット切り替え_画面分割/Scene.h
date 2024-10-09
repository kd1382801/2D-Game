#pragma once

struct Object {
	float x;
	float y;
	Math::Matrix mat;
};

struct Wipe {
	float x;
	float y;
	float scaleX;	//拡大率
	float scaleY;	//拡大率
	Math::Matrix mat;
	Math::Matrix transMat;	//移動行列
	Math::Matrix scaleMat;	//拡縮行列
};

class Scene
{
private:

	// テクスチャ ・・・ 画像データ
	KdTexture playerTex;
	KdTexture backTex;
	KdTexture tmpTex;	//描画先(レンダーターゲット)切り換え用

	Object player;
	Object back;

	//ワイプ表示用
	Wipe wipe;	

	//画面分割
	Math::Matrix leftMat;	//左半分用
	Math::Matrix rightMat;	//右半分用
	float cutX;				//それぞれの画面のX座標


public:

	// 初期設定
	void Init();

	// 解放
	void Release();

	// 更新処理
	void Update();

	//テクスチャへの描画処理
	void DynamicDraw2D();

	// バックバッファへの描画処理
	void Draw2D();

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
