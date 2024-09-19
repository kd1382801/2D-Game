#pragma once

class C_Bullet 
{
public:

	C_Bullet();
	~C_Bullet();

	void Init();
	void Update();
	void Draw();

	void SetTex(KdTexture *pTex);
	bool GetAlive();
	Math::Vector2 GetPos();
	float GetRadius();
	void Shot(Math::Vector2 pos, float targetAngle);
	void Hit();
private:

	const float m_moveSpeed = 10.0f;
	const float m_radius = 8.0f;
	KdTexture* m_tex;
	Math::Vector2 m_pos;
	Math::Matrix m_mat;
	float m_angle;
	bool m_alive;
};