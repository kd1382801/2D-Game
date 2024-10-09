#include "main.h"
#include "Scene.h"

//テクスチャ(tmpTex)への描画処理 main.cpp 206 Draw2Dの前に処理
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

	//ノイズ処理
	//x軸の揺れ幅
	int x = 0;
	for (int i = 0; i < 720; i+=sliceNum) {
		//ノイズの振れ幅-50~50
		x = rand() % (noiseLv + 1) - ((float)noiseLv / 2);

		//切り取りした画像を画面の一番上から表示する
		back.mat = Math::Matrix::CreateTranslation(x, 360 - i - ((float)sliceNum / 2), 0);
		
		SHADER.m_spriteShader.SetMatrix(back.mat);
		SHADER.m_spriteShader.DrawTex(&tmpTex, Math::Rectangle(0, i, 1280, sliceNum), 1.0f);
	}

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

	//ノイズ
	//スライスの幅
	if (GetAsyncKeyState('S') & 0x8000) {
		sliceNum += 1;
		if (sliceNum > 50) {
			sliceNum = 50;
		}
	}
	if (GetAsyncKeyState('A') & 0x8000) {
		sliceNum -= 1;
		if (sliceNum < 1) {
			sliceNum = 1;
		}
	}
	//ノイズの強さ
	if (GetAsyncKeyState('X') & 0x8000) {
		noiseLv++;
		if (noiseLv > 200) {
			noiseLv = 200;
		}
	}
	if (GetAsyncKeyState('Z') & 0x8000) {
		noiseLv--;
		if (noiseLv < 0) {
			noiseLv = 0;
		}
	}

	player.mat = Math::Matrix::CreateTranslation(player.x, player.y, 0);
	back.mat = Math::Matrix::CreateTranslation(back.x, back.y, 0);

	//ワイプ用
	wipe.transMat = Math::Matrix::CreateTranslation(wipe.x, wipe.y, 0);
	wipe.scaleMat = Math::Matrix::CreateScale(wipe.scaleX, wipe.scaleY, 1);
	wipe.mat = wipe.scaleMat * wipe.transMat;
}

void Scene::Init()
{
	srand(time(0));

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

	//ノイズ
	sliceNum = 1;
	noiseLv = 0;
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
