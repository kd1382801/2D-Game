#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	//画面クリア
	color = { 0.0f,0.5f,0.5f,1.0f };//RGBA
	SHADER.m_spriteShader.DrawBox(0, 0, 320, 180, &color, true);
	
	//四角形
	color = { 0.9f,0.5f,0.0f,1.0f };//RGBA
	//SHADER.m_spriteShader.DrawBox(0, 0, 20, 10, &color, true);

	//円
	color = { 1.0f,1.0f,0.0f,1.0f };
	//SHADER.m_spriteShader.DrawCircle(playerX, playerY, 50, &color, true);

	//点
	color = { 0.0f,1.0f,1.0f,1.0f };
	//SHADER.m_spriteShader.DrawPoint(0, 0, &color);
	//SHADER.m_spriteShader.DrawPoint(1, 0, &color);
	//SHADER.m_spriteShader.DrawPoint(0, 1, &color);
	//SHADER.m_spriteShader.DrawPoint(1, 1, &color);

	//線を引く
	if (GetAsyncKeyState('L') & 0x8000)
	{
		for (int i = 180; i >= -180; i -= 10)
		{
			color = { (i + 180) / 360.0f ,1.0f,0.0f,1.0f };
			SHADER.m_spriteShader.DrawLine(-320, 180, 320, i, &color);
			color = { 0.0f,rand() / 32767.0f,1.0f,1.0f };
			SHADER.m_spriteShader.DrawLine(320, -180, -320, i, &color);
		}
	}

	//自機の描画
	DrawPlayer();

	//敵の描画
	DrawEnemy();

	//エフェクトの描画
	DrawCircleEffect();


	//文字列は図形などを描画した後に書くこと
	// 文字列表示
	SHADER.m_spriteShader.DrawString(100, 170, "基本図形１", Math::Vector4(0, 0, 0, 1));
	SHADER.m_spriteShader.DrawString(100 + 8, 170 - 5, "基本図形１", Math::Vector4(1, 1, 0, frame / 180.0f));
	
}

void Scene::Update()
{
	
	UpdatePlayer();
	
	UpdateEnemy();

	UpdateCircleEffect();

	//フレーム数を増やす
	frame++;

	//gameStepの切り替え
	if (frame == 60 * 5)
	{
		gameStep = 2;
	}
	else if (frame == 60 * 10)
	{
		gameStep = 3;
	}

}

void Scene::Init()
{
	//乱数の初期化
	srand(timeGetTime());

	InitPlayer();
	InitEnemy();
	InitCircleEffect();

	frame = 0;
	gameStep = 1;
}

void Scene::Release()
{
	
}


//自機初期化
void Scene::InitPlayer()
{
	//座標の初期化
	playerX = 0;
	playerY = -150;
}

//自機更新
void Scene::UpdatePlayer()
{
	//座標更新
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		playerX += 8;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		playerX -= 8;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		playerY += 8;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		playerY -= 8;
	}

}

//自機描画
void Scene::DrawPlayer()
{
	//自機処理
	color = { 1.0f,1.0f,0.0f,1.0f };
	SHADER.m_spriteShader.DrawCircle(playerX, playerY, 20, &color, true);
	color = { 0.0f,0.0f,0.0f,1.0f };
	SHADER.m_spriteShader.DrawBox(playerX - 6, playerY + 3, 2, 8, &color, true);
	SHADER.m_spriteShader.DrawBox(playerX + 6, playerY + 3, 2, 8, &color, true);
	SHADER.m_spriteShader.DrawBox(playerX , playerY - 12, 6, 1, &color, true);
}

//敵初期化
void Scene::InitEnemy()
{
	enemyX = 0;
	enemyY = 100;
	enemyMoveX = 3;
	enemyMoveY = 3;
}

//敵更新
void Scene::UpdateEnemy()
{
	if (gameStep == 1)
	{
		//横バウンド
		
		//座標更新
		enemyX += enemyMoveX;

		//右端判定
		if (enemyX > screenRight - 16)
		{
			//enemyMoveX = -3;
			enemyMoveX *= -1;	//符号反転
		}

		//左端判定
		if (enemyX < screenLeft + 16)
		{
			//enemyMoveX = 3;
			enemyMoveX *= -1;
		}
	}
	else if (gameStep == 2)
	{
		//縦バウンド
		
		enemyY += enemyMoveY;

		//判定
		//上端判定
		if (enemyY > screenTop - 16)
		{
			enemyMoveY *= -1;
		}

		//下端判定
		if (enemyY < screenBottom + 16)
		{
			enemyMoveY *= -1;
		}
	}
	else if (gameStep == 3)
	{
		//斜めバウンド

		enemyX += enemyMoveX;
		enemyY += enemyMoveY;

		//判定
		//右端判定
		if (enemyX > screenRight - 16)
		{
			enemyMoveX *= -1;	//符号反転
		}

		//左端判定
		if (enemyX < screenLeft + 16)
		{
			enemyMoveX *= -1;
		}
		//上端判定
		if (enemyY > screenTop - 16)
		{
			enemyMoveY *= -1;
		}

		//下端判定
		if (enemyY < screenBottom + 16)
		{
			enemyMoveY *= -1;
		}
	}
}

//敵描画
void Scene::DrawEnemy()
{
	if (gameStep == 1)
	{
		color = { 1,0,1,1 };
		SHADER.m_spriteShader.DrawBox(enemyX, enemyY, 16, 16, &color, true);
		color = { rand() / 32767.0f ,rand() / 32767.0f ,1,1 };
		SHADER.m_spriteShader.DrawBox(enemyX, enemyY, 10, 10, &color, true);
	}
	else if (gameStep == 2)
	{
		color = { 0.5f,0.7f,0.7f,1 };
		SHADER.m_spriteShader.DrawCircle(enemyX, enemyY, 25, &color, true);
	}
	else if (gameStep == 3)
	{
		color = { 0,1,1,1 };
		SHADER.m_spriteShader.DrawCircle(enemyX, enemyY, 30, &color, true);
		color = { 0.3f + rand() / 32767.0f ,0.3f + rand() / 32767.0f ,0.3f + rand() / 32767.0f,1 };
		SHADER.m_spriteShader.DrawCircle(enemyX, enemyY, 20, &color, true);
	}
}

//エフェクト初期化
void Scene::InitCircleEffect()
{
	circleX = 0;
	circleY = 0;
	circleRadius = 10;
	circleColor = { 1,1,1,1 };
	circleFlg = false;
}

//エフェクト更新
void Scene::UpdateCircleEffect()
{
	//スペースキーでエフェクト発動
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (!circleFlg)
		{
			circleX = playerX;
			circleY = playerY;
			circleRadius = 0;
			circleColor = { 1,1,1,1 };
			circleFlg = true;
		}
	}

	if (circleFlg)
	{
		circleRadius += 7;
		if (circleRadius > 500)
		{
			circleFlg = false;
		}
	}
}

//エフェクト描画
void Scene::DrawCircleEffect()
{
	if (circleFlg)
	{
		SHADER.m_spriteShader.DrawCircle(circleX, circleY, circleRadius, &circleColor, false);
		SHADER.m_spriteShader.DrawCircle(circleX, circleY, circleRadius - 10, &circleColor, false);
	}
}



//ctrl + mmでたためる
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