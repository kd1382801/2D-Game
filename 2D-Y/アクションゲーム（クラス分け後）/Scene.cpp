#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	m_map.Draw();
	m_player.Draw();
}

void Scene::Update()
{
	m_player.Update();
	m_map.Update();
}

void Scene::Init()
{
	m_map.LoadMap();

	// �摜�̓ǂݍ��ݏ���
	m_playerTex.Load("Texture/Player/Chara.png");
	m_mapTex.Load("Texture/Map/MapChip.png");

	m_player.SetTex(&m_playerTex);
	m_map.SetTex(&m_mapTex);

	m_player.Init();
	m_map.Init();

}

void Scene::Release()
{
	// �摜�̉������
	m_playerTex.Release();
	m_mapTex.Release();
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
