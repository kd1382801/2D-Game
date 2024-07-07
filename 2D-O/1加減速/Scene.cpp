#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	//行列をセット
	SHADER.m_spriteShader.SetMatrix(matrix);

	//描画
	//SHADER.m_spriteShader.DrawTex(&charaTex, Math::Rectangle{ 0,0,64,64 }, 1.0f);

	Math::Rectangle srcRect{ 0,0,64,64 };			//テクスチャ座標
	Math::Color color = { 1.0f,0.5f,1.0f,1.0f };	//色RGBA
	SHADER.m_spriteShader.DrawTex(&charaTex, 0, 0, &srcRect, &color);

	//文字列はテクスチャなどを描画した後に書くこと
	// 文字列表示
	SHADER.m_spriteShader.DrawString(230, 350, "加減速", Math::Vector4(0, 0, 0, 1));
	SHADER.m_spriteShader.DrawString(230+8, 350-5, "加減速", Math::Vector4(1, 1, 0, frame / 180.0f));

	char str[80];
	sprintf_s(str, sizeof(str), "X移動量:%.2f", playerMoveX);
	SHADER.m_spriteShader.DrawString(230, 300, str, Math::Vector4(0, 1, 0, 1));
	sprintf_s(str, sizeof(str), "Y移動量:%.2f", playerMoveY);
	SHADER.m_spriteShader.DrawString(230, 260, str, Math::Vector4(0, 1, 0, 1));

}

void Scene::Update()
{
	//座標更新
	
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		//右加速
		playerMoveX += 0.5f;
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		//左加速
		playerMoveX -= 0.5f;
	}
	else 
	{
		//減速
		//playerMoveX = 0;	←ピタッと止める場合
		playerMoveX *= 0.92f;

		//0の近似値を0に補正する
		if (playerMoveX > -0.5f && playerMoveX < 0.5f)
		{
			playerMoveX = 0;
		}

	}

	playerX += playerMoveX;

	//画面端判定
	if (playerX > 640 + 32)
	{
		playerX = -640 - 32;
	}
	else if (playerX < -640 - 32)
	{
		playerX = 640 + 32;
	}

	//ジャンプ
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		playerMoveY = 23;
	}

	playerY += playerMoveY;

	//重力をかける
	playerMoveY -= 1.0f;

	//地面判定
	if (playerY < -300)
	{
		playerY = -300;
		playerMoveY = 5;
	}

	//行列の作成
	matrix = Math::Matrix::CreateTranslation(playerX, playerY, 0);

	//フレーム数を増やす
	frame++;

}

void Scene::Init()
{
	// 画像の読み込み処理
	charaTex.Load("Texture/smile_transparent.png");

	//座標の初期化
	playerX = 0;
	playerY = 360;
	playerMoveX = 0;
	playerMoveY = -1;

	frame = 0;
}

void Scene::Release()
{
	// 画像の解放処理
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
