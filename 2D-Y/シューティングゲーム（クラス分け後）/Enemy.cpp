#include "Enemy.h"

C_Enemy::C_Enemy()
{

}

C_Enemy::~C_Enemy()
{

}

void C_Enemy::Init()
{
	m_pos.x = 0;
	m_pos.y = 200;
}

void C_Enemy::Update()
{
	m_pos.x += m_moveSpeed;

	//‰æ–Ê’[“–‚½‚è”»’è
	if (m_pos.x > 640) {
		m_moveSpeed *= -1;
	}
	if (m_pos.x < -640) {
		m_moveSpeed *= -1;
	}

	m_mat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
}

void C_Enemy::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(m_tex, Math::Rectangle{ 0,0,64,64 }, 1.0f);
}

void C_Enemy::SetTex(KdTexture* pTex)
{
	m_tex = pTex;
}
