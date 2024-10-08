#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	m_player.Draw();
}

void Scene::Update()
{
	m_player.Update();
}

void Scene::Init()
{
	// �摜�̓ǂݍ��ݏ���
	m_playerTex.Load("Texture/Player/Chara.png");

	m_player.SetTex(&m_playerTex);

	m_player.Init();
}

void Scene::Release()
{
	// �摜�̉������
	m_playerTex.Release();
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
