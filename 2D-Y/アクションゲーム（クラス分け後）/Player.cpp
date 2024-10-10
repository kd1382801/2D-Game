#include "Player.h"

void C_Player::Init()
{
	m_pos = { 0,0 };
	m_move = { 0,0 };
	m_alive = true;
	m_scaleX = 1.0f;
	m_jump = false;
}

void C_Player::Action()
{
	if (!m_alive)return;
	
	m_move.x = 0;
	m_move.y -= Gravity;

	//左右キーで移動
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		m_move.x -= MovePower;
		m_scaleX = -1.0f;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		m_move.x += MovePower;
		m_scaleX = 1.0f;
	}

	//ジャンプ
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		if (!m_jump) {
			m_jump = true;
			m_move.y = JumpPower;
		}
	}

}

void C_Player::Update()
{
	
	m_pos += m_move;

	m_transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_scaleMat = Math::Matrix::CreateScale(m_scaleX, 1.0f, 1.0f);
	m_mat = m_scaleMat * m_transMat;
}

void C_Player::Draw()
{
	if (!m_alive)return;

	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle{ 0,0,64,64 },1.0f);
}

void C_Player::MapHitX(float posX, float moveX)
{
	m_pos.x = posX;
	m_move.x = moveX;
}

void C_Player::MapHitY(float posY, float moveY, bool jump)
{
	m_pos.y = posY;
	m_move.y = moveY;
	m_jump = jump;
}
