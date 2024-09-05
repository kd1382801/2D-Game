#pragma once

class C_Player 
{
public:

	C_Player();
	~C_Player();

	void Init();
	void Update();
	void Draw();

	const float m_moveSpeed = 5.0f;
	KdTexture m_tex;
	Math::Vector2 m_pos;
	Math::Matrix m_mat;

};
