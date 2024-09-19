#pragma once
#include "Bullet.h"

class Scene;	//‘O•ûéŒ¾(cpp‚Åinclude‚·‚é)

class C_Player 
{
public:

	C_Player();
	~C_Player();

	void Init();
	void Update(POINT mousePos);
	void Draw();

	void SetTex(KdTexture* pTex);
	void SetOwner(Scene* pOwner);

	void hitBulletEnemy();

private:
	
	Scene* m_pOwner;

	static const int m_bulletNum = 100;
	C_Bullet m_bullet[m_bulletNum];
	KdTexture m_bulletTex;


	const float m_moveSpeed = 5.0f;
	KdTexture* m_pTex;
	Math::Vector2 m_pos;
	Math::Matrix m_mat;

};
