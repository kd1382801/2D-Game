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
	int m_count;				//動作管理用カウンタ
	bool m_bHoming;				//ホーミングフラグ
	bool m_bActive;				//フラグ
	Math::Matrix m_matrix;
	KdTexture* m_pTexture;		//テクスチャ(ポインタ)
	Math::Rectangle m_srcRect;	//テクスチャ座標
	Math::Color m_color;		//色RGBA

};

struct tEnemy
{
	float m_posX;
	float m_posY;
	float m_moveX;
	float m_moveY;
	float m_radius;
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

	//敵
	struct tEnemy enemy;

	//弾
	static const int bulletNum = 200;
	struct tBullet bullet[bulletNum];
	KdTexture charaTex;//ロードや解放に使う

	//弾の発射間隔制御用
	static const int shotInterval = 5;
	int keyCount;

	bool debugFlg;//デバッグ用

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

	//敵用
	void InitEnemy();
	void UpdateEnemy();
	void DrawEnemy();

	//弾用
	void InitBullet();
	void UpdateBullet();
	void DrawBullet();
	void ShotBullet();
	void ShotBulletDS(float a_degree,float a_speed);

	//角度を求める関数			(src:source元、源)	(dest:destination目的地、行き先)
	float GetAngleRad(float srcX, float srcY, float destX, float destY);
	float GetAngleDeg(float srcX, float srcY, float destX, float destY);

	//距離を求める関数
	float GetDistance(float srcX, float srcY, float destX, float destY);

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
