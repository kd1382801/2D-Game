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
	m_move.x = m_moveSpeed;
	m_move.y = 0;
	m_alive = true;
}

void C_Enemy::Update()
{
	if (!m_alive) {
		return;
	}

	m_pos += m_move;

	//‰æ–Ê’[“–‚½‚è”»’è
	if (m_pos.x > 640 || m_pos.x < -640) {
		m_move.x *= -1;
	}
	
	m_mat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
}

void C_Enemy::Draw()
{
	if (!m_alive) {
		return;
	}

	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(m_tex, Math::Rectangle{ 0,0,64,64 }, 1.0f);
}

void C_Enemy::SetTex(KdTexture* pTex)
{
	m_tex = pTex;
}

bool C_Enemy::GetAlive()
{
	return m_alive;
}

Math::Vector2 C_Enemy::GetPos()
{
	return m_pos;
}

float C_Enemy::GetRadius()
{
	return m_radius;
}

void C_Enemy::Hit()
{
	m_alive = false;
}
