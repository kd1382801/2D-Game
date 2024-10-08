#pragma once

//画面端用の定数
const int SCREEN_LEFT = -640;
const int SCREEN_RIGHT = 640;
const int SCREEN_TOP = 360;
const int SCREEN_BOTTOM = -360;


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

struct tBullet
{
	float m_posX;
	float m_posY;
	float m_moveX;
	float m_moveY;
	float m_speed;
	bool m_bActive;				//フラグ
	Math::Matrix m_matrix;
	KdTexture* m_pTexture;		//テクスチャ(ポインタ)
	Math::Rectangle m_srcRect;	//テクスチャ座標
	Math::Color m_color;		//色RGBA

};

class Scene
{
private:
	//自機
	struct tPlayer player;

	//照準器
	struct tSight sight;

	//弾
	static const int bulletNum = 200;
	struct tBullet bullet[bulletNum];
	KdTexture charaTex;//ロードや解放に使う

	//弾の発射間隔制御用
	static const int shotInterval = 5;
	int keyCount;

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

	//弾用
	void InitBullet();
	void UpdateBullet();
	void DrawBullet();
	void ShotBullet();
	void ShotBulletDS(float a_degree,float a_speed);

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
