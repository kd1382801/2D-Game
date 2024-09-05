#pragma once

struct tPlayer {
	float m_posX;
	float m_posY;
	Math::Matrix m_matrix;
	KdTexture m_texture;
};

struct tSight {
	float m_posX;
	float m_posY;
	Math::Matrix m_matrix;
	KdTexture m_texture;
};

class Scene
{
private:
	//自機
	struct tPlayer player;

	//照準器
	struct tSight sight;

	//直角三角形
	float a;	//底辺
	float b;	//高さ
	float c;	//斜辺
	float deg;	//角度


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

	// GUI処理
	void ImGuiUpdate();

	//自機用
	void InitPlayer();
	void UpdatePlayer();
	void DrawPlayer();

	//照準器用
	void InitSight();
	void UpdateSight();
	void DrawSight();

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
