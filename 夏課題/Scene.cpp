#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	//現在のシーンの描画処理を実行
	switch (nowScene)
	{
	case Title:
		DrawTitle();
		break;
	case Game:
		DrawGame();
		break;
	case Result:
		DrawResult();
		break;
	}
}

void Scene::Update()
{
	//現在のシーンの更新処理を実行
	switch (nowScene)
	{
	case Title:
		UpdateTitle();
		break;
	case Game:
		UpdateGame();
		break;
	case Result:
		UpdateResult();
		break;
	}
}

void Scene::Init()
{
	//乱数の初期化
	srand(timeGetTime());

	//ゲーム開始最初のシーン:タイトル
	nowScene = SceneType::Title;
	frame = 0;

	wcolor = { 1,1,1,1 };
	bcolor = { 0,0,0,1 };

	InitMap();
	LoadMap();
	LoadRoom();
	InitPlayer();
	InitBlock();
}

void Scene::Release()
{

}

void Scene::ImGuiUpdate()
{

}

void Scene::DrawTitle()
{

}

void Scene::UpdateTitle()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
		nowScene = Game;
		LoadMap();
		LoadRoom();
		InitPlayer();
		InitBlock();
	}
}

void Scene::DrawGame()
{
	DrawMap();
	DrawPlayer();
	//白の時だけ実行
	if (!changeStageFlg) {
		DrawBlock();
	}

	if (doFadeInOut) {
		FadeInOut(&changeStageFlg);
	}

	char str[80];
	sprintf_s(str, sizeof(str), "%d / %d", nowRoom + 1,roomNum);
	//sprintf_s(str, sizeof(str), "%d", clearFrame / 60);
	//sprintf_s(str, sizeof(str), "%f", player.y);
	if (!changeStageFlg) {
		SHADER.m_spriteShader.DrawString(100, 150, str, Math::Vector4(0, 0, 0, 1));
	}
	else {
		SHADER.m_spriteShader.DrawString(100, 150, str, Math::Vector4(1, 1, 1, 1));
	}
}

void Scene::UpdateGame()
{
	clearFrame++;
	
	//キーが押されたら色反転
	if (GetAsyncKeyState('M') & 0x8000) {
		if (!doFadeInOut) {
			doFadeInOut = true;
		}
	}
	

	UpdatePlayer();

	//白の時だけ実行
	if (!changeStageFlg) {
		UpdateBlock();
		NextRoom();
	}

	UpdateMap();

	//3つクリアしたらリザルトへ移動
	if (nowRoom >= roomNum) {
		nowScene = Result;
	}
}

void Scene::DrawResult()
{
	SHADER.m_spriteShader.DrawBox(0, 0, 320, 180, &wcolor, true);


	char str[80];
	sprintf_s(str, sizeof(str), "クリアタイム：%d秒", clearFrame / 60);
	SHADER.m_spriteShader.DrawString(0, 0, str, Math::Vector4(0, 0, 0, 1));
}

void Scene::UpdateResult()
{

}



void Scene::InitPlayer()
{
	player = { room[nowRoom].pX,room[nowRoom].pY,0,0,false};
	playerHorizontal = 1;
}

void Scene::UpdatePlayer()
{
	player.moveX = 0;
	player.moveY -= gravity;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		playerHorizontal = -1;
		player.moveX -= 9;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		playerHorizontal = 1;
		player.moveX += 9;
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		if (!player.jumpFlg) {
			player.moveY = jumpPower;
			player.jumpFlg = true;
			blockJumpFlg = true;
		}
	}
	else
	{
		player.jumpFlg = false;
	}
	//ブロックキャッチ
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
		if (!shiftKeyFlg) {
			shiftKeyFlg = true;
			cflg += 1;
		}
	}
	else {
		shiftKeyFlg = false;
	}
	//落下したら初期値に戻る
	if (player.y < mapChip[mapHeight - 1][0].y - 200) {
		player.y = room[nowRoom].pX;
		player.x = room[nowRoom].pY;
	}


	BlockHit();
	MapHit(&player.x,&player.y,&player.moveX,&player.moveY);


	player.x += player.moveX;
	player.y += player.moveY;
}

void Scene::DrawPlayer()
{
	color = { 1.0f,0.5f,0.0f,1.0f };
	SHADER.m_spriteShader.DrawCircle(player.x - scroll.X, player.y - scroll.Y, 18, &color, true);
	color = { 0,0,0,1 };
	SHADER.m_spriteShader.DrawBox(player.x - scroll.X + (8 * playerHorizontal), player.y - scroll.Y + 3, 2, 6, &color, true);
	SHADER.m_spriteShader.DrawBox(player.x - scroll.X, player.y - scroll.Y + 3, 2, 6, &color, true);
}

void Scene::MapHit(float* x, float* y, float* moveX, float* moveY,bool block,int blockColor)
{
	const float playerLeft = *x - playerRadius;
	const float playerRight = *x + playerRadius;
	const float playerBottom = *y - playerRadius;
	const float playerTop = *y + playerRadius;

	//未来座標
	const float nextLeft = playerLeft + *moveX;
	const float nextRight = playerRight + *moveX;
	const float nextBottom = playerBottom + *moveY;
	const float nextTop = playerTop + *moveY;

	for (int h = 0; h < mapHeight; h++)
	{
		for (int w = 0; w < mapWidth; w++)
		{
			if (mapChip[h][w].flg == 0)
			{
				continue;
			}
			if (mapChip[h][w].flg != 9) {
				const float chipLeft = mapChip[h][w].baseX - mapRadius;
				const float chipRight = mapChip[h][w].baseX + mapRadius;
				const float chipBottom = mapChip[h][w].baseY - mapRadius;
				const float chipTop = mapChip[h][w].baseY + mapRadius;

				if (playerRight > chipLeft && playerLeft < chipRight)//キャラとブロックが縦で重なっているか
				{
					if (nextBottom < chipTop && nextTop > chipTop)//未来座標が上からブロックに当たったとき(着地時など)
					{
						*moveY = 0;
						*y = chipTop + playerRadius;
						if (block) {
							if (mapChip[h][w].flg == blockColor) {
								score |= Red;
							}
							else {
								score = 0;
							}
						}
					}
					else if (chipBottom < nextTop && nextBottom < chipBottom)//未来座標が下からブロックに当たったとき
					{
						*moveY = 0;
						*y = chipBottom - playerRadius;

					}
				}

				if (playerTop > chipBottom && playerBottom < chipTop)//キャラとブロックが横で重なっているか
				{
					if (nextLeft < chipRight && nextRight > chipRight)//未来座標が右からブロックに当たったとき
					{
						*moveX = 0;
						*x = chipRight + playerRadius;
					}
					else if (nextRight > chipLeft && nextLeft < chipLeft)//未来座標が左からブロックに当たったとき
					{
						*moveX = 0;
						*x = chipLeft - playerRadius;
					}
				}
			}
		}
	}
}

void Scene::BlockHit()
{
	if (!changeStageFlg) {
		const float playerLeft = player.x - playerRadius;
		const float playerRight = player.x + playerRadius;
		const float playerBottom = player.y - playerRadius;
		const float playerTop = player.y + playerRadius;

		//未来座標
		const float nextLeft = playerLeft + player.moveX;
		const float nextRight = playerRight + player.moveX;
		const float nextBottom = playerBottom + player.moveY;
		const float nextTop = playerTop + player.moveY;

		//ブロック座標
		const float blockLeft = block.x - blockRadius;
		const float blockRight = block.x + blockRadius;
		const float blockBottom = block.y - blockRadius;
		const float blockTop = block.y + blockRadius;

		if (playerRight > blockLeft && playerLeft < blockRight)//キャラとブロックが縦で重なっているか
		{
			if (nextBottom < blockTop && nextTop > blockTop)//未来座標が上からブロックに当たったとき(着地時など)
			{
				player.moveY = 0;
				player.y = blockTop + playerRadius;
			}
			//else if (blockBottom < nextTop && nextBottom < blockBottom)//未来座標が下からブロックに当たったとき
			//{
			//	player.moveY = 0;
			//	player.y = blockBottom - playerRadius;
			//}
		}

		if (playerTop > blockBottom && playerBottom < blockTop)//キャラとブロックが横で重なっているか
		{
			if (nextLeft < blockRight && nextRight > blockRight)//未来座標が右からブロックに当たったとき
			{
				player.moveX = 0;
				player.x = blockRight + playerRadius;
			}
			else if (nextRight > blockLeft && nextLeft < blockLeft)//未来座標が左からブロックに当たったとき
			{
				player.moveX = 0;
				player.x = blockLeft - playerRadius;
			}
		}


	}
}

void Scene::InitBlock()
{
		block = { room[nowRoom].bX,room[nowRoom].bY,0,0,2};
		blockJumpFlg = false;
	
}

void Scene::UpdateBlock()
{
	block.moveX = 0;
	block.moveY -= gravity;

	//キャッチされたとき
	if (cflg % 2 == 1) {
		float a = block.x - player.x;
		float b = block.y - player.y;
		float c = sqrt(a * a + b * b);
		if (c < 60) {
			if (playerHorizontal == -1) {
				block.x = player.x + blockRadius + playerRadius;
			}
			if (playerHorizontal == 1) {
				block.x = player.x - blockRadius - playerRadius;
			}
			if (blockJumpFlg) {
				blockJumpFlg = false;
				block.moveY = jumpPower;
			}
		}
		else { 
			cflg = 0; 
		}
	}
	
	//落下したら初期値に戻る
	if (block.y < mapChip[mapHeight - 1][0].y - 200) {
		block.y = room[nowRoom].bX;
		block.x = room[nowRoom].bY;
	}

	//BlockHit(&block.x, &block.y, &block.moveX, &block.moveY, bn);

	MapHit(&block.x, &block.y, &block.moveX, &block.moveY, true, block.flg);
	blockJumpFlg = false;
	block.x += block.moveX;
	block.y += block.moveY;

}

void Scene::DrawBlock()
{
	SHADER.m_spriteShader.DrawBox(block.x - scroll.X, block.y - scroll.Y, 20, 20, &bcolor, true);
	SHADER.m_spriteShader.DrawBox(block.x - scroll.X, block.y - scroll.Y, 18, 18, &wcolor, true);
	SHADER.m_spriteShader.DrawBox(block.x - scroll.X, block.y - scroll.Y, 18, 2, &bcolor, true);
}


void Scene::LoadRoom()
{
	FILE* fp;
	fp = fopen("Room.txt", "r");
	//文字読み込み処理
	
	for (int rn = 0; rn < roomNum; rn++) {
						//部屋番号,初期値x,y,ブロックの初期値x,y,脱出に必要なスコア
		int b = fscanf(fp, "%d%f%f%f%f%d", &room[rn].number, &room[rn].pX, &room[rn].pY,
												&room[rn].bX, &room[rn].bY, &room[rn].clearScore);
	}
	fclose(fp);
}

void Scene::NextRoom()
{
	if (GetAsyncKeyState('E') & 0x8000) {
		for (int h = 0; h < mapHeight; h++) {
			for (int w = 0; w < mapWidth; w++) {
				//mapChipがドアの時の処理
				if (mapChip[h][w].flg == 9) {
					//白の時だけ実行
					if (!changeStageFlg) {
						const float playerLeft = player.x - playerRadius;
						const float playerRight = player.x + playerRadius;
						const float playerBottom = player.y - playerRadius;
						const float playerTop = player.y + playerRadius;

						const float chipLeft = mapChip[h][w].baseX - mapRadius;
						const float chipRight = mapChip[h][w].baseX + mapRadius;
						const float chipBottom = mapChip[h][w].baseY - mapRadius;
						const float chipTop = mapChip[h][w].baseY + mapRadius;

						if (playerRight > chipLeft && playerLeft < chipRight && playerTop > chipBottom && playerBottom < chipTop) {	//縦と横で重なっているか
							if (score == room[nowRoom].clearScore) {
								nowRoom++;
								player.x = room[nowRoom].pX;
								player.y = room[nowRoom].pY;
								block.x = room[nowRoom].bX;
								block.y = room[nowRoom].bY;
								score = 0;
							}
						}
					}
				}
			}
		}
	}
}

void Scene::InitMap()
{
	for (int h = 0; h < mapHeight; h++) {
		for (int w = 0; w < mapWidth; w++) {
			mapChip[h][w] = { 0,0, (float)ScreenLeft + (40 * (w + 1)) - 20,(float)ScreenTop - (40 * h) - 20,0 };
		}
	}

	scroll.X = 0;
	scroll.XMin = mapChip[0][0].baseX + 300;
	scroll.XMax = mapChip[0][mapWidth - 1].baseX - 300;
	scroll.Y = 0;
	scroll.YMax = mapChip[0][0].baseY - 160;
	scroll.YMin = mapChip[mapHeight - 1][0].baseY + 160;
}

void Scene::LoadMap()
{
	FILE* fp;
	fp = fopen("Stage.txt", "r");
	//文字読み込み処理
	char ch;
	int h = 0;	//縦のカウンタ
	int w = 0;	//横のカウンタ
	while ((ch = fgetc(fp)) != EOF){
		if (ch == '\n') {
			h++;
			w = 0;
		}
		else{
			mapChip[h][w].flg = atoi(&ch);
			w++;
		}
	}
	fclose(fp);
}

void Scene::UpdateMap()
{
	scroll.X = player.x;
	scroll.Y = player.y;

	if (scroll.X < scroll.XMin) {
		scroll.X = scroll.XMin;
	}
	if (scroll.X > scroll.XMax) {
		scroll.X = scroll.XMax;
	}
	if (scroll.Y > scroll.YMax) {
		scroll.Y = scroll.YMax;
	}
	if (scroll.Y < scroll.YMin) {
		scroll.Y = scroll.YMin;
	}
	

	for (int h = 0; h < mapHeight; h++) {
		for (int w = 0; w < mapWidth; w++) {
			mapChip[h][w].x = mapChip[h][w].baseX - scroll.X;
			mapChip[h][w].y = mapChip[h][w].baseY - scroll.Y;
		}
	}
}

void Scene::DrawMap()
{
	if (!changeStageFlg) {
		SHADER.m_spriteShader.DrawBox(0, 0, 1000, 1000, &wcolor, true);
	}
	else {
		SHADER.m_spriteShader.DrawBox(0, 0, 1000, 1000, &bcolor, true);
	}
	

	for (int h = 0; h < mapHeight; h++) {
		for (int w = 0; w < mapWidth; w++) {
			if (mapChip[h][w].flg == 1) {
				if (!changeStageFlg) {
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y, 20, 20, &bcolor, true);
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y, 18, 18, &wcolor, true);
				}
				else {
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y, 20, 20, &wcolor, true);
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y, 18, 18, &bcolor, true);
				}

			}
			if (mapChip[h][w].flg == 2) {
				if (!changeStageFlg) {
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y, 20, 20, &bcolor, true);
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y, 18, 18, &wcolor, true);
				}
				else {
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y, 20, 20, &wcolor, true);
					color = { 1,0,0,1 };
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y, 18, 18, &color, true);
				}
			}
			if (mapChip[h][w].flg == 9) {
				if (!changeStageFlg) {
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y + 10, 20, 30, &bcolor, true);
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y + 10, 18, 28, &wcolor, true);
				}
			}

			//SHADER.m_spriteShader.DrawCircle(mapChip[h][w].baseX, mapChip[h][w].baseY, 3, &bcolor, true);
		}
	}
}

void Scene::FadeInOut(bool* flg)
{
	if (fadeFrame < 30) {
		color = { 0,0,0,fadeFrame / 30.0f };
		SHADER.m_spriteShader.DrawBox(0, 0, 320, 180, &color, true);

		fadeFrame++;
	}
	if(fadeFrame == 30){
		*flg = !*flg;
		fadeFrame++;
	}
	if (fadeFrame > 30) {
		color = { 0,0,0,1.0f - ((fadeFrame - 30) / 30.0f) };
		SHADER.m_spriteShader.DrawBox(0, 0, 320, 180, &color, true);

		fadeFrame++;
	}
	if (fadeFrame == 60) {
		fadeFrame = 0;
		doFadeInOut = false;
	}
}



