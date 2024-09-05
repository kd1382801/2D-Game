#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	//プレイヤー
	SHADER.m_spriteShader.SetMatrix(player.mat);
	SHADER.m_spriteShader.DrawTex(&playerTex, Math::Rectangle{ 0,0,64,64 }, player.alpha);

	//敵
	SHADER.m_spriteShader.SetMatrix(enemy.mat);
	SHADER.m_spriteShader.DrawTex(&enemyTex, Math::Rectangle{ 0,0,64,64 }, enemy.alpha);
	
	//キャラ
	SHADER.m_spriteShader.SetMatrix(chara.mat);
	SHADER.m_spriteShader.DrawTex(&charaTex, Math::Rectangle{ 0,0,64,64 }, chara.alpha);

}

void Scene::Update()
{
	//プレイヤー
	player.angle += 3;
	if (player.angle >= 360) {
		player.angle = 0;
	}
	//sinで-1～1を求める
	player.y = sin(DirectX::XMConvertToRadians(player.angle)) * 100;
	//アルファ値に反映 -1～1 を 0～1 に補正
	player.alpha = sin(DirectX::XMConvertToRadians(player.angle)) * 0.5f + 0.5f;

	//敵
	enemy.angle += 5;
	if (enemy.angle >= 360) {
		enemy.angle = 0;
	}
	//fabs:引数の絶対値をfloat型で返す
	enemy.y = fabs(sin(DirectX::XMConvertToRadians(enemy.angle))) * 100;

	//キャラ
	chara.angle += 5;
	if (chara.angle >= 360) {
		chara.angle = 0;
	}
	//拡大率の値に反映 -1～1 を 0～1 に補正
	chara.size = sin(DirectX::XMConvertToRadians(chara.angle)) * 0.5f + 0.5f;


	player.mat = Math::Matrix::CreateTranslation(player.x, player.y, 0);
	enemy.mat = Math::Matrix::CreateTranslation(enemy.x, enemy.y, 0);
	chara.mat = Math::Matrix::CreateScale(chara.size, 1, 1);
	
}

void Scene::Init()
{
	// 画像の読み込み処理
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
	// 画像の解放処理
	playerTex.Release();
	enemyTex.Release();
	charaTex.Release();
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
