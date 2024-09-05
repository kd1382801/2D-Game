#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	//�v���C���[
	SHADER.m_spriteShader.SetMatrix(player.mat);
	SHADER.m_spriteShader.DrawTex(&playerTex, Math::Rectangle{ 0,0,64,64 }, player.alpha);

	//�G
	SHADER.m_spriteShader.SetMatrix(enemy.mat);
	SHADER.m_spriteShader.DrawTex(&enemyTex, Math::Rectangle{ 0,0,64,64 }, enemy.alpha);
	
	//�L����
	SHADER.m_spriteShader.SetMatrix(chara.mat);
	SHADER.m_spriteShader.DrawTex(&charaTex, Math::Rectangle{ 0,0,64,64 }, chara.alpha);

}

void Scene::Update()
{
	//�v���C���[
	player.angle += 3;
	if (player.angle >= 360) {
		player.angle = 0;
	}
	//sin��-1�`1�����߂�
	player.y = sin(DirectX::XMConvertToRadians(player.angle)) * 100;
	//�A���t�@�l�ɔ��f -1�`1 �� 0�`1 �ɕ␳
	player.alpha = sin(DirectX::XMConvertToRadians(player.angle)) * 0.5f + 0.5f;

	//�G
	enemy.angle += 5;
	if (enemy.angle >= 360) {
		enemy.angle = 0;
	}
	//fabs:�����̐�Βl��float�^�ŕԂ�
	enemy.y = fabs(sin(DirectX::XMConvertToRadians(enemy.angle))) * 100;

	//�L����
	chara.angle += 5;
	if (chara.angle >= 360) {
		chara.angle = 0;
	}
	//�g�嗦�̒l�ɔ��f -1�`1 �� 0�`1 �ɕ␳
	chara.size = sin(DirectX::XMConvertToRadians(chara.angle)) * 0.5f + 0.5f;


	player.mat = Math::Matrix::CreateTranslation(player.x, player.y, 0);
	enemy.mat = Math::Matrix::CreateTranslation(enemy.x, enemy.y, 0);
	chara.mat = Math::Matrix::CreateScale(chara.size, 1, 1);
	
}

void Scene::Init()
{
	// �摜�̓ǂݍ��ݏ���
	playerTex.Load("Texture/player.png");
	enemyTex.Load("Texture/enemy.png");
	charaTex.Load("Texture/Chara.png");

	player.x = -300.0f;
	player.y = 0.0f;
	player.angle = 0.0f;
	player.alpha = 1.0f;

	enemy.x = 300.0f;
	enemy.y = 0.0f;
	enemy.angle = 0.0f;
	enemy.alpha = 1.0f;

	chara.x = 0.0f;
	chara.y = 0.0f;
	chara.angle = 0.0f;
	chara.alpha = 1.0f;
	chara.size = 1.0f;
}

void Scene::Release()
{
	// �摜�̉������
	playerTex.Release();
	enemyTex.Release();
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
