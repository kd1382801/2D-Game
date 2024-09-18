#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	m_player.Draw();
	m_enemy.Draw();
}

void Scene::Update()
{
	m_player.Update();
	m_enemy.Update();
}

void Scene::Init()
{
	m_playerTex.Load("Texture/player.png");
	m_player.SetTex(&m_playerTex);
	m_player.Init();

	//敵
	m_enemyTex.Load("Texture/enemy.png");
	m_enemy.SetTex(&m_enemyTex);
	m_enemy.Init();
}

void Scene::Release()
{
	m_playerTex.Release();
	m_enemyTex.Release();
}

void Scene::ImGuiUpdate()
{
	return;

	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Once);

	// デバッグウィンドウ
	if (ImGui::Begin("Debug Window"))
	{
		ImGui::Text("FPS : %d", APP.m_fps);
	}
	ImGui::End();
}

