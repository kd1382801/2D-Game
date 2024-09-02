#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{

	SHADER.m_spriteShader.SetMatrix(playerMat);
	SHADER.m_spriteShader.DrawTex(&playerTex,Math::Rectangle{0,0,64,64},1.0f);


}

void Scene::Update()
{
	//���W����
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		playerX -= 5;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		playerX += 5;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		playerY += 5;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		playerY -= 5;
	}

	//��]
	angle++;
	if (angle >= 360) {
		angle = 0;
	}

	//�g�嗦����
	if (GetAsyncKeyState('Z') & 0x8000) {
		size += 0.1f;
	}
	if (GetAsyncKeyState('X') & 0x8000) {
		size -= 0.1f;
	}

	//�s��쐬
	playerTransMat = Math::Matrix::CreateTranslation(playerX, playerY, 0);
	playerRotMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(angle));
	playerScaleMat = Math::Matrix::CreateScale(size, size, 1);
	//�s��̍���
	playerMat = playerScaleMat * playerRotMat * playerTransMat;
	//               �g�k      *     ��]     *      �ړ�
}

void Scene::Init()
{
	// �摜�̓ǂݍ��ݏ���
	playerTex.Load("Texture/player.png");

	playerX = 0;
	playerY = 0;
	angle = 0;
	size = 1;
}

void Scene::Release()
{
	// �摜�̉������
	playerTex.Release();
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
