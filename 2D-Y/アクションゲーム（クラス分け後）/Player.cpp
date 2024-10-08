#include "Player.h"

void C_Player::Init()
{
	m_pos = { 0,0 };
	m_move = { 0,0 };
	m_alive = true;
	m_scaleX = 1.0f;
	m_jump = false;
}

void C_Player::Update()
{
	if (!m_alive)return;

	m_move.x = 0;
	m_move.y -= Gravity;

	
	//���E�L�[�ňړ�
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		m_move.x -= MovePower;
		m_scaleX = -1.0f;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		m_move.x += MovePower;
		m_scaleX = 1.0f;
	}

	//�W�����v
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		if (!m_jump) {
			m_jump = true;
			m_move.y = JumpPower;
		}
	}

	m_pos += m_move;

	//���n�ʔ���
	if (m_pos.y < -200) {
		m_pos.y = -200;
		m_jump = false;
	}

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
