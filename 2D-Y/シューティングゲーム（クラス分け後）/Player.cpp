#include "Player.h"

C_Player::C_Player()
{
	
}

C_Player::~C_Player()
{
	
}

void C_Player::Init()
{
	m_pos.x = 0;
	m_pos.y = 0;
}

void C_Player::Update()
{
	if (GetAsyncKeyState('W') & 0x8000) {
		m_pos.y += m_moveSpeed;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		m_pos.y -= m_moveSpeed;
	}
	if (GetAsyncKeyState('A') & 0x8000) {
		m_pos.x -= m_moveSpeed;
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		m_pos.x += m_moveSpeed;
	}

	m_mat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
}

void C_Player::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle{ 0,0,64,64 }, 1.0f);
}

void C_Player::SetTex(KdTexture* pTex)
{
	m_pTex = pTex;
}
