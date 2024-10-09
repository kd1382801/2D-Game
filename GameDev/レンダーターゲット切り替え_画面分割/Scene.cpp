#include "main.h"
#include "Scene.h"

//テクスチャ(tmptex)への描画処理 main.cpp 206 Draw2Dの前に処理
void Scene::DynamicDraw2D()
{
	//描画先のテクスチャのクリア(透明色)
	tmpTex.ClearRenerTarget(Math::Color(0.0f, 0.0f, 0.0f, 0.0f));

	//描画先をテクスチャへ切り替え
	tmpTex.SetRenderTarget();

	//背景
	SHADER.m_spriteShader.SetMatrix(back.mat);
	SHADER.m_spriteShader.DrawTex(&backTex, Math::Rectangle(0, 0, 1280, 720), 1.0f);

	//自機
	SHADER.m_spriteShader.SetMatrix(player.mat);
	SHADER.m_spriteShader.DrawTex(&playerTex, Math::Rectangle(0, 0, 64, 64), 1.0f);

}

//バックバッファ(実行画面)への描画処理
void Scene::Draw2D()
{
	//描画先をバックバッファへ切り替え(以降の描画処理はバックバッファへ)
	D3D.SetBackBuffer();

	//DynamicDraw2D()で作成したテクスチャを描画

	//左半分
	SHADER.m_spriteShader.SetMatrix(leftMat);
	SHADER.m_spriteShader.DrawTex(&tmpTex, Math::Rectangle(0, 0, 640, 720), 1.0f);
	
	//右半分
	SHADER.m_spriteShader.SetMatrix(rightMat);
	SHADER.m_spriteShader.DrawTex(&tmpTex, Math::Rectangle(640, 0, 640, 720), 1.0f);

	//ワイプ表示(もう一度temTexを右上に小さく表示)
	SHADER.m_spriteShader.SetMatrix(wipe.mat);
	SHADER.m_spriteShader.DrawTex(&tmpTex, Math::Rectangle(0, 0, 1280, 720), 1.0f);



}

void Scene::Update()
{
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		player.x -= 5;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		player.x += 5;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		player.y += 5;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		player.y -= 5;
	}

	//画面分割
	if (GetAsyncKeyState('Z') & 0x8000) {
		cutX += 10;
	}
	if (GetAsyncKeyState('X') & 0x8000) {
		cutX -= 10;
	}

	player.mat = Math::Matrix::CreateTranslation(player.x, player.y, 0);
	back.mat = Math::Matrix::CreateTranslation(back.x, back.y, 0);

	//ワイプ用
	wipe.transMat = Math::Matrix::CreateTranslation(wipe.x, wipe.y, 0);
	wipe.scaleMat = Math::Matrix::CreateScale(wipe.scaleX, wipe.scaleY, 1);
	wipe.mat = wipe.scaleMat * wipe.transMat;

	//画面分割
	leftMat = Math::Matrix::CreateTranslation(-cutX, 0, 0);
	rightMat = Math::Matrix::CreateTranslation(cutX, 0, 0);
}

void Scene::Init()
{
	playerTex.Load("Texture/player.png");
	backTex.Load("Texture/Back_02.png");

	//レンダーターゲット用テクスチャ作成
	tmpTex.CreateRenderTarget(1280, 720);


	back.x = 0.0f;
	back.y = 0.0f;

	player.x = 0.0f;
	player.y = 0.0f;

	wipe.x = 400.0f;
	wipe.y = 200.0f;
	wipe.scaleX = 0.25f;
	wipe.scaleY = 0.25f;

	cutX = 320;
}

void Scene::Release()
{
	// 画像の解放処理
	playerTex.Release();
	backTex.Release();
	tmpTex.Release();
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
