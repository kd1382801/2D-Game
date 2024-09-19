#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	m_player.Draw();
	m_enemy.Draw();
}

void Scene::Update()
{
	CalcMousePos();


	m_player.Update(m_mousePos);
	m_enemy.Update();

	m_player.hitBulletEnemy();
}

void Scene::Init()
{
	m_playerTex.Load("Texture/player.png");
	m_enemyTex.Load("Texture/enemy.png");
	
	m_player.SetTex(&m_playerTex);
	m_enemy.SetTex(&m_enemyTex);
	
	m_player.Init();
	m_enemy.Init();

	m_player.SetOwner(this);
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

void Scene::CalcMousePos()
{
	GetCursorPos(&m_mousePos);
	ScreenToClient(APP.m_window.GetWndHandle(), &m_mousePos);
	m_mousePos.x -= 640;
	m_mousePos.y -= 360;
	m_mousePos.y *= -1;
}

C_Enemy* Scene::GetEnemy()
{
	return &m_enemy;
}

