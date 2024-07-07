#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	//�s����Z�b�g
	SHADER.m_spriteShader.SetMatrix(matrix);

	//�`��
	//SHADER.m_spriteShader.DrawTex(&charaTex, Math::Rectangle{ 0,0,64,64 }, 1.0f);

	Math::Rectangle srcRect{ 0,0,64,64 };			//�e�N�X�`�����W
	Math::Color color = { 1.0f,0.5f,1.0f,1.0f };	//�FRGBA
	SHADER.m_spriteShader.DrawTex(&charaTex, 0, 0, &srcRect, &color);

	//������̓e�N�X�`���Ȃǂ�`�悵����ɏ�������
	// ������\��
	SHADER.m_spriteShader.DrawString(230, 350, "������", Math::Vector4(0, 0, 0, 1));
	SHADER.m_spriteShader.DrawString(230+8, 350-5, "������", Math::Vector4(1, 1, 0, frame / 180.0f));

	char str[80];
	sprintf_s(str, sizeof(str), "X�ړ���:%.2f", playerMoveX);
	SHADER.m_spriteShader.DrawString(230, 300, str, Math::Vector4(0, 1, 0, 1));
	sprintf_s(str, sizeof(str), "Y�ړ���:%.2f", playerMoveY);
	SHADER.m_spriteShader.DrawString(230, 260, str, Math::Vector4(0, 1, 0, 1));

}

void Scene::Update()
{
	//���W�X�V
	
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		//�E����
		playerMoveX += 0.5f;
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		//������
		playerMoveX -= 0.5f;
	}
	else 
	{
		//����
		//playerMoveX = 0;	���s�^�b�Ǝ~�߂�ꍇ
		playerMoveX *= 0.92f;

		//0�̋ߎ��l��0�ɕ␳����
		if (playerMoveX > -0.5f && playerMoveX < 0.5f)
		{
			playerMoveX = 0;
		}

	}

	playerX += playerMoveX;

	//��ʒ[����
	if (playerX > 640 + 32)
	{
		playerX = -640 - 32;
	}
	else if (playerX < -640 - 32)
	{
		playerX = 640 + 32;
	}

	//�W�����v
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		playerMoveY = 23;
	}

	playerY += playerMoveY;

	//�d�͂�������
	playerMoveY -= 1.0f;

	//�n�ʔ���
	if (playerY < -300)
	{
		playerY = -300;
		playerMoveY = 5;
	}

	//�s��̍쐬
	matrix = Math::Matrix::CreateTranslation(playerX, playerY, 0);

	//�t���[�����𑝂₷
	frame++;

}

void Scene::Init()
{
	// �摜�̓ǂݍ��ݏ���
	charaTex.Load("Texture/smile_transparent.png");

	//���W�̏�����
	playerX = 0;
	playerY = 360;
	playerMoveX = 0;
	playerMoveY = -1;

	frame = 0;
}

void Scene::Release()
{
	// �摜�̉������
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
