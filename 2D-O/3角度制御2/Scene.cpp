#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	DrawPlayer();
	DrawSight();
	DrawBullet();

	//������̓e�N�X�`���Ȃǂ�`�悵����ɏ�������
	// ������\��
	SHADER.m_spriteShader.DrawString(230, 350, "�p�x����Q", Math::Vector4(0, 0, 0, 1));
	SHADER.m_spriteShader.DrawString(230+8, 350-5, "�p�x����Q", Math::Vector4(1, 1, 0, frame / 180.0f));

	char str[50];
	sprintf_s(str, sizeof(str), "deg:%.2f", deg);
	SHADER.m_spriteShader.DrawString(240, 300, str, Math::Vector4(0, 1, 0, 1));
	sprintf_s(str, sizeof(str), "c:%.2f", c);
	SHADER.m_spriteShader.DrawString(240, 260, str, Math::Vector4(0, 1, 0, 1));

}

void Scene::Update()
{
	UpdatePlayer();
	UpdateSight();
	UpdateBullet();
	
	//�t���[�����𑝂₷
	frame++;
}

void Scene::Init()
{
	// �摜�̓ǂݍ��ݏ���
	player.m_texture.Load("Texture/smile_transparent.png");
	sight.m_texture.Load("Texture/sight_transparent.png");
	charaTex.Load("Texture/bullet_transparent.png");

	//�ǂݍ��񂾃e�N�X�`��(charaTex)�̃A�h���X��m_pTexture�֓����
	for (int b = 0; b < bulletNum; b++) {
		bullet[b].m_pTexture = &charaTex;
	}

	InitPlayer();
	InitSight();
	InitBullet();

	frame = 0;
}

void Scene::Release()
{
	// �摜�̉������
	player.m_texture.Release();
	sight.m_texture.Release();
	charaTex.Release();
}

void Scene::ImGuiUpdate()
{
	return;

	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Once);

	// �f�o�b�O�E�B���h�E
	if (ImGui::Begin("Debug Window"))
	{
		ImGui::Text("FPS : %d", APP.m_fps);
	}
	ImGui::End();
}

void Scene::InitPlayer()
{
	//���W�̏�����
	player.m_posX = 0;
	player.m_posY = 0;

}

void Scene::UpdatePlayer()
{
	//���W�X�V
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		player.m_posX += 8;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		player.m_posX -= 8;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		player.m_posY += 8;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		player.m_posY -= 8;
	}

	//Z��X��deg�𑝌�����
	if (GetAsyncKeyState('Z') & 0x8000) 
	{
		deg += 2;
	}
	if (GetAsyncKeyState('X') & 0x8000) 
	{
		deg -= 2;
	}

	//A��S��c�𑝌�����
	if (GetAsyncKeyState('A') & 0x8000) {
		c -= 2;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		c += 2;
	}

	//�s��̍쐬
	player.m_matrix = Math::Matrix::CreateTranslation(player.m_posX, player.m_posY, 0);

}

void Scene::DrawPlayer()
{
	//�s����Z�b�g
	SHADER.m_spriteShader.SetMatrix(player.m_matrix);

	//�`��
	Math::Rectangle srcRect{ 0,0,64,64 };			//�e�N�X�`�����W
	Math::Color color = { 1.0f,0.5f,1.0f,1.0f };	//�FRGBA
	SHADER.m_spriteShader.DrawTex(&player.m_texture, 0, 0, &srcRect, &color);
}

void Scene::InitSight()
{
	sight.m_posX = 0;
	sight.m_posY = 0;

	c = 200;
	deg = 30;
}

void Scene::UpdateSight()
{

	//deg += 1;

	a = cos(DirectX::XMConvertToRadians(deg)) * c;
	b = sin(DirectX::XMConvertToRadians(deg)) * c;

	sight.m_posX = player.m_posX + a;
	sight.m_posY = player.m_posY + b;

	sight.m_matrix = Math::Matrix::CreateTranslation(sight.m_posX, sight.m_posY, 0);
}

void Scene::DrawSight()
{
	SHADER.m_spriteShader.SetMatrix(sight.m_matrix);

	Math::Rectangle srcRect{ 0,0,32,32 };			//�e�N�X�`�����W
	Math::Color color = { 1.0f,1.0f,1.0f,1.0f };	//�FRGBA
	SHADER.m_spriteShader.DrawTex(&sight.m_texture, 0, 0, &srcRect, &color);
}

void Scene::InitBullet()
{
	for (int b = 0; b < bulletNum; b++) {
		bullet[b].m_posX = 0;
		bullet[b].m_posY = 0;
		bullet[b].m_bActive = false;
	}
}

void Scene::UpdateBullet()
{
	for (int b = 0; b < bulletNum; b++) {


		bullet[b].m_matrix = Math::Matrix::CreateTranslation(bullet[b].m_posX, bullet[b].m_posY, 0);
	}
}

void Scene::DrawBullet()
{
	for (int b = 0; b < bulletNum; b++) {
		SHADER.m_spriteShader.SetMatrix(bullet[b].m_matrix);

		Math::Rectangle srcRect{ 0,0,32,32 };			//�e�N�X�`�����W
		Math::Color color = { 1.0f,1.0f,1.0f,1.0f };	//�FRGBA
		SHADER.m_spriteShader.DrawTex(bullet[b].m_pTexture, 0, 0, &srcRect, &color);
	}
}
