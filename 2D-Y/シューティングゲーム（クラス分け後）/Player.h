#pragma once

class C_Player 
{
public:

	C_Player();
	~C_Player();

	void Init();
	void Update();
	void Draw();

	void SetTex(KdTexture* pTex);

private:

	const float m_moveSpeed = 5.0f;
	KdTexture* m_pTex;
	Math::Vector2 m_pos;
	Math::Matrix m_mat;

};
