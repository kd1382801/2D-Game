#pragma once

class C_Enemy 
{
public:

	C_Enemy();
	~C_Enemy();

	void Init();
	void Update();
	void Draw();

	void SetTex(KdTexture *pTex);



private:
	
	float m_moveSpeed = 5.0f;
	KdTexture* m_tex;
	Math::Vector2 m_pos;
	Math::Matrix m_mat;
};