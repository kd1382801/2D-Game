#include "Player.h"
#include "Scene.h"

C_Player::C_Player()
{
	
}

C_Player::~C_Player()
{
	m_bulletTex.Release();
}

void C_Player::Init()
{
	m_bulletTex.Load("Texture/bullet.png");

	for (int b = 0; b < m_bulletNum; b++) {
		m_bullet[b].SetTex(&m_bulletTex);
		m_bullet[b].Init();
	}

	m_pos.x = 0;
	m_pos.y = 0;
}

void C_Player::Update(POINT mousePos)
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

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
		for (int b = 0; b < m_bulletNum; b++) {
			if (!m_bullet[b].GetAlive()) {
				const float x = mousePos.x - m_pos.x;
				const float y = mousePos.y - m_pos.y;
				const float radian = atan2(y, x);

				m_bullet[b].Shot(m_pos, radian);
				break;
			}
		}
	}

	m_mat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);

	for (int b = 0; b < m_bulletNum; b++) {
		m_bullet[b].Update();
	}

}

void C_Player::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle{ 0,0,64,64 }, 1.0f);

	for (int b = 0; b < m_bulletNum; b++) {
		m_bullet[b].Draw();
	}
}

void C_Player::SetTex(KdTexture* pTex)
{
	m_pTex = pTex;
}

void C_Player::SetOwner(Scene* pOwner)
{
	m_pOwner = pOwner;
}

void C_Player::hitBulletEnemy()
{
	C_Enemy* enemy = m_pOwner->GetEnemy();

	if (!enemy->GetAlive()) {
		return;
	}

	const Math::Vector2 enemyPos = enemy->GetPos();
	
	for (int i = 0; i < m_bulletNum; i++) {
		if (!m_bullet[i].GetAlive()) {
			continue;
		}

		const Math::Vector2 bulletPos = m_bullet[i].GetPos();

		const float x = bulletPos.x - enemyPos.x;
		const float y = bulletPos.y - enemyPos.y;
		const float z = sqrt(x * x + y * y);

		const float sumRadius = enemy->GetRadius() + m_bullet[i].GetRadius();

		if (z < sumRadius) {
			enemy->Hit();
			m_bullet[i].Hit();
		}
	}
}
