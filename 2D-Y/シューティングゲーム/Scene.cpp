#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	PlayerDraw();
}

void Scene::Update()
{
	PlayerUpdate();
}

void Scene::Init()
{
	PlayerInit();
}

void Scene::Release()
{
	m_player.tex.Release();
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

void Scene::PlayerInit()
{
	m_player.tex.Load("Texture/player.png");
	m_player.x = 0.0f;
	m_player.y = 0.0f;
	m_player.moveSpeed = 5.0f;
}

void Scene::PlayerUpdate()
{
	m_player.mat = Math::Matrix::CreateTranslation(m_player.x, m_player.y, 0);

	if (GetAsyncKeyState('W') & 0x8000) {
		m_player.y += m_player.moveSpeed;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		m_player.y -= m_player.moveSpeed;
	}
	if (GetAsyncKeyState('A') & 0x8000) {
		m_player.x -= m_player.moveSpeed;
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		m_player.x += m_player.moveSpeed;
	}


}

void Scene::PlayerDraw()
{
	SHADER.m_spriteShader.SetMatrix(m_player.mat);
	SHADER.m_spriteShader.DrawTex(&m_player.tex, Math::Rectangle{ 0,0,64,64 }, 1.0f);
}
