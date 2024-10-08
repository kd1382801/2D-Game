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
	// 画像の読み込み処理
	m_playerTex.Load("Texture/Player/Chara.png");

	m_player.SetTex(&m_playerTex);

	m_player.Init();
}

void Scene::Release()
{
	// 画像の解放処理
	m_playerTex.Release();
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
