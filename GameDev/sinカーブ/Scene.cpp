#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	SHADER.m_spriteShader.SetMatrix(player.mat);
	SHADER.m_spriteShader.DrawTex(&playerTex, Math::Rectangle{ 0,0,64,64 }, 1.0f);
}

void Scene::Update()
{
	player.y = sinf(player.angle) * 200;
	player.angle+=0.1f;
	if (player.angle >= 360) {
		player.angle = 0;
	}


	player.mat = Math::Matrix::CreateTranslation(player.x, player.y, 0);
}

void Scene::Init()
{
	// 画像の読み込み処理
	playerTex.Load("Texture/player.png");
	enemyTex.Load("Texture/enemy.png");

	player = { 0,0,0 };
}

void Scene::Release()
{
	// 画像の解放処理
	playerTex.Release();
	enemyTex.Release();
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
