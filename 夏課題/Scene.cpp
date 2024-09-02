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
	case HowtoPlay:
		DrawHtP();
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
	case HowtoPlay:
		UpdateHtP();
		break;
	}
}

void Scene::Init()
{
	//乱数の初期化
	srand(timeGetTime());

	//ゲーム開始最初のシーン:タイトル
	nowScene = SceneType::Title;
	htpScene = 0;
	keyflg.enter = false;
	titleGameCflg = false;
	arrow = { 80,72,0,0 };
	
	frame = 0;

	wcolor = { 1,1,1,1 };
	bcolor = { 0,0,0,1 };

	InitMap();
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
	DrawPlayer();
	DrawMap();

	if (!arrowKeyFlg) {
		color = { 1,0,0,1 };
		SHADER.m_spriteShader.DrawBox(arrow.x, arrow.y, 94, 24, &color, true);
	}
	else {
		if (frame % 30 > 15) {
			color = { 1,0,0,1 };
			SHADER.m_spriteShader.DrawBox(arrow.x, arrow.y, 94, 24, &color, true);
		}
		else {
			color = { 1,0,0,1 };
			SHADER.m_spriteShader.DrawBox(arrow.x, arrow.y, 94, 24, &wcolor, true);
		}
	}


	SHADER.m_spriteShader.DrawBox(80, 85 - 13, 90, 20, &bcolor, true);
	SHADER.m_spriteShader.DrawBox(80, 85 - 13, 88, 18, &wcolor, true);
	SHADER.m_spriteShader.DrawBox(80, 0 - 13, 90, 20, &bcolor, true);
	SHADER.m_spriteShader.DrawBox(80, 0 - 13, 88, 18, &wcolor, true);

	if (doFadeInOut) {
		if (arrow.y == 72) {
			FadeInOut(&titleGameCflg);
		}
		if (arrow.y == -13) {
			FadeInOut(&titleHtpCflg);
		}
	}


	SHADER.m_spriteShader.DrawString(-260, 50, "S", Math::Vector4{ 1,0,0,1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f });
	SHADER.m_spriteShader.DrawString(-260, 50, " e", Math::Vector4{ 0,0.6f,0,1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f });
	SHADER.m_spriteShader.DrawString(-260, 50, "  t", Math::Vector4{ 0,0,1,1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f });
	SHADER.m_spriteShader.DrawString(-260, 50, "    Blocks", Math::Vector4{ 0,0,0,1 });

	

	SHADER.m_spriteShader.DrawString(0, 85, "   START", Math::Vector4{ 0,0,0,1 });
	SHADER.m_spriteShader.DrawString(0, 0,  "How to Play", Math::Vector4{ 0,0,0,1 });

	if (!arrowKeyFlg) {
		SHADER.m_spriteShader.DrawString(0, -80, "↑↓キー ：選択\nEnterキー：決定", Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
	}
}

void Scene::UpdateTitle()
{
	UpdatePlayer();
	UpdateMap();

	if (!arrowKeyFlg) {
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			arrow.y = 72;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			arrow.y = -13;
		}
	}


	if (titleGameCflg) {
		titleGameCflg = false;
		arrowKeyFlg = false;
		nowScene = Game;
		nowRoom = 0;
		nextRoomFlg = false;
		goNextRoomFlg = false;
		clearFrame = 0;
		LoadMap();
		LoadRoom();
		InitPlayer();
		InitBlock();
		InitMoveFloor();
	}
	if (titleHtpCflg) {
		titleHtpCflg = false;
		arrowKeyFlg = false;
		nowScene = HowtoPlay;
		htpScene = 0;
		frame = 0;
		enterFrame = 0;
		InitMap();
		InitPlayer();
		InitBlock();
	}

	frame++;
}

void Scene::DrawGame()
{
	DrawMap();
	DrawMoveFloor();
	DrawPlayer();
	//白の時だけ実行
	if (!changeStageFlg) {
		DrawBlock();
	}

	if (doFadeInOut) {
		if (nextRoomFlg) {
			FadeInOut(&goNextRoomFlg);
		}
		else {
			FadeInOut(&changeStageFlg);
		}
	}

	char str[80];
	sprintf_s(str, sizeof(str), "block:%d/%d", score2, nowRoom + 1);
	if (!changeStageFlg) {
		SHADER.m_spriteShader.DrawString(100, 130, str, Math::Vector4(0, 0, 0, 1));
	}
	else {
		SHADER.m_spriteShader.DrawString(100, 130, str, Math::Vector4(1, 1, 1, 1));
	}
}

void Scene::UpdateGame()
{
	clearFrame++;
	score = 0;
	score2 = 0;
	//キーが押されたら色反転
	if (GetAsyncKeyState('M') & 0x8000) {
		if (!doFadeInOut) {
			doFadeInOut = true;
		}
	}
	

	UpdateMoveFloor();
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
		CheckScore();
		InitPlayer();
		InitMap();
	}
}

void Scene::DrawResult()
{
	DrawMap();
	DrawPlayer();
	
	DrawREffect();

	if (doFadeInOut) {
		FadeInOut(&resultTitleCflg);
	}

	SHADER.m_spriteShader.DrawString(70 - scroll.X, 75, "  ステージクリア  ", Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
	
	char str[80];
	if (clearFrame >= 60 * 60) {
		sprintf_s(str, sizeof(str), "クリアタイム：%2d分%2d秒", clearFrame / 3600, clearFrame % 3600 / 60);
	}
	else {
		sprintf_s(str, sizeof(str), "クリアタイム：%2d秒", clearFrame / 60);
	}
	SHADER.m_spriteShader.DrawString(70 - scroll.X, 35, str, Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f ));
	
	if (maxScoreFlg) {
		sprintf_s(str, sizeof(str), "最速クリアタイム更新！");
		SHADER.m_spriteShader.DrawString(55 - scroll.X, -5, str, Math::Vector4(1, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
	}
	
}

void Scene::UpdateResult()
{
	UpdatePlayer();
	UpdateMap();

	if (resultTitleCflg) {
		resultTitleCflg = false;
		nowScene = Title;
		maxScoreFlg = false;
		InitMap();
		InitPlayer();
	}
}

void Scene::DrawHtP()
{
	DrawMap();
	DrawPlayer();

	//白の時だけ実行
	if (!changeStageFlg) {
		DrawBlock();
	}

	if (doFadeInOut) {
		if (htpScene == htpLastScene) {
			FadeInOut(&htpTitleCflg);
		}
		else{
			FadeInOut(&changeStageFlg);
		}
		
	}



	DrawHtpString();
}

void Scene::UpdateHtP() 
{
	UpdatePlayer();
	UpdateBlock();
	UpdateMap();


	if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
		if (!keyflg.enter) {
			if (frame - enterFrame >= 100) {
				keyflg.enter = true;
				enterFrame = frame;
				htpScene++;
			}
		}
	}
	else {
		keyflg.enter = false;
	}

	if (htpScene == htpLastScene) {
		doFadeInOut = true;
	}

	//タイトルに戻る
	if (htpTitleCflg) {
		htpTitleCflg = false;
		nowScene = Title;
		frame = 0;
		InitMap();
		InitPlayer();
	}

	frame++;
}



void Scene::InitPlayer()
{
	if (nowScene == Title) {
		player = {-200,-280,0,0,false};
		playerHorizontal = 1;
	}
	if (nowScene == HowtoPlay) {
		player = { -140,-280,0,0,false };
		playerHorizontal = 1;
	}
	if (nowScene == Game) {
		player = { room[nowRoom].pX,room[nowRoom].pY,0,0,false };
		playerHorizontal = 1;
	}
	if (nowScene == Result) {
		player = { -260,-280,0,0,false };
		playerHorizontal = 1;
	}
}

void Scene::UpdatePlayer()
{
	//nowSceneがタイトルの時
	if (nowScene == Title) {
		if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
			if (!keyflg.enter) {
				keyflg.enter = true;
				arrowKeyFlg = true;
				frame=0;
			}
		}
		else {
			keyflg.enter = false;
		}

		if (arrowKeyFlg == true) {
			if (frame >= 90) {
				doFadeInOut = true;//フェードインでシーンチェンジ
			}
		}

		player.x += player.moveX;
	}

	//nowSceneがHowtoPlayの時
	if (nowScene == HowtoPlay) {
		switch (htpScene)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			if (!htpChangeStage) {
				doFadeInOut = true;
				htpChangeStage = true;
			}
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			if (htpChangeStage) {
				doFadeInOut = true;
				htpChangeStage = false;
			}
			break;
		case 7:
			player.moveX = -2.5;
			playerHorizontal = -1;
			//ブロックの1つ前で止まる
			if (player.x <= mapChip[mapHeight - 1][2].x) {
				player.moveX = 0;
				cflg = 1;
			}
			break;
		case 8:
			player.moveX = 4.5;
			playerHorizontal = 1;
			//赤マスの1つ先でとまる
			if (player.x >= mapChip[mapHeight - 1][10].x) {
				player.moveX = 0;
				cflg = 0;
			}
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			player.moveX = 2.5;
			//ドアの場所に立った時
			if (player.x >= mapChip[mapHeight - 2][14].x) {
				player.moveX = 0;
				htpScene++;
			}

			break;
		default:
			break;
		}
		player.x += player.moveX;
	}

	//nowSceneがゲームの時
	if (nowScene == Game) {
		player.moveX = 0;
		player.moveY -= gravity;

		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
			playerHorizontal = -1;
			player.moveX -= 7;
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			playerHorizontal = 1;
			player.moveX += 7;
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
			if (!player.jumpFlg) {
				player.moveY = jumpPower;
				player.jumpFlg = true;
				for (int bn = 0; bn < blockNum; bn++) {
					block[bn].jumpFlg = true;
				}
			}
		}

		//ブロックキャッチ
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
			if (!keyflg.shift) {
				keyflg.shift = true;
				cflg += 1;
			}
		}
		else {
			keyflg.shift = false;
		}
		//落下したら初期値に戻る
		if (player.y < mapChip[mapHeight - 1][0].y - 300) {
			player.x = room[nowRoom].pX;
			player.y = room[nowRoom].pY;
		}

		//gcFlgの初期化
		player.gcFlg = 0;

		BlockHit(&player.x, &player.y, &player.moveX, &player.moveY);
		MoveFloorHit(&player.x, &player.y, &player.moveX, &player.moveY,&player.gcFlg);
		MapHit(&player.x, &player.y, &player.moveX, &player.moveY,&player.gcFlg);

		//挟まったら初期値に戻る
		if (player.gcFlg == GcHit) {
			player.x = room[nowRoom].pX;
			player.y = room[nowRoom].pY;
		}

		//ジャンプフラグ
		if (player.gcFlg & GcMoveFloor) {
			player.jumpFlg = false;
		}

		player.x += player.moveX;
		player.y += player.moveY;
	}

	//nowSceneがリザルトの時
	if (nowScene == Result) {
		player.moveX = 2;

		//ドアの場所に立った時
		if (player.x >= mapChip[mapHeight - 2][24].baseX) {
			player.moveX = 0;
			doFadeInOut = true;//フェードインでシーンチェンジ
		}

		player.x += player.moveX;
	}
}

void Scene::DrawPlayer()
{
	if (nowScene != Title) {
		color = { 1.0f,0.5f,0.0f,1.0f };
		SHADER.m_spriteShader.DrawCircle(player.x - scroll.X, player.y - scroll.Y, 18, &color, true);
		color = { 0,0,0,1 };
		SHADER.m_spriteShader.DrawBox(player.x - scroll.X + (8 * playerHorizontal), player.y - scroll.Y + 3, 2, 6, &color, true);
		SHADER.m_spriteShader.DrawBox(player.x - scroll.X, player.y - scroll.Y + 3, 2, 6, &color, true);
		
	}
	else {
		color = { 1.0f,0.5f,0.0f,1.0f };
		SHADER.m_spriteShader.DrawCircle(player.x - scroll.X, player.y - scroll.Y + 20, 100, &color, true);
		color = { 0,0,0,1 };
		SHADER.m_spriteShader.DrawBox(player.x - scroll.X + (45 * playerHorizontal), player.y - scroll.Y + 50, 11, 30, &color, true);
		SHADER.m_spriteShader.DrawBox(player.x - scroll.X, player.y - scroll.Y + 50, 11, 30, &color, true);
	}
}

void Scene::MapHit(float* x, float* y, float* moveX, float* moveY, int* gcFlg, bool block,int blockColor, bool cFlg)
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
			if (mapChip[h][w].flg < 8) {
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
						//ブロックならスコア計算
						if (block) {
							if (cFlg == false) {
								if (mapChip[h][w].flg == blockColor) {
									//ブロックとマップの距離を計算
									float a = mapChip[h][w].baseX - *x;
									float b = mapChip[h][w].baseY - *y;
									float c = sqrt(a * a + b * b);
									//真上あたりに置いたとき
									if (c <= 43) {
										switch (blockColor)
										{
										case 2:
											score |= Red;
											score2 += 1;
											break;
										case 3:
											score |= Green;
											score2 += 1;
											break;
										case 4:
											score |= Blue;
											score2 += 1;
										}
									}
								}
							}
						}
						else {
							player.jumpFlg = false;
						}
					}
					else if (chipBottom < nextTop && nextBottom < chipBottom)//未来座標が下からブロックに当たったとき
					{
						*moveY = 0;
						*y = chipBottom - playerRadius;
						*gcFlg |= GcTop;
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

void Scene::BlockHit(float* x, float* y, float* moveX, float* moveY, bool bl, int blockNum)
{
	if (!changeStageFlg) {
		const float playerLeft = *x - playerRadius;
		const float playerRight = *x + playerRadius;
		const float playerBottom = *y - playerRadius;
		const float playerTop = *y + playerRadius;

		//未来座標
		const float nextLeft = playerLeft + *moveX;
		const float nextRight = playerRight + *moveX;
		const float nextBottom = playerBottom + *moveY;
		const float nextTop = playerTop + *moveY;

		for (int bn = 0; bn < nowRoom + 1; bn++) {
			if (bl == true && bn == blockNum) {
				continue;
			}
			//ブロック座標
			const float blockLeft = block[bn].x - blockRadius;
			const float blockRight = block[bn].x + blockRadius;
			const float blockBottom = block[bn].y - blockRadius;
			const float blockTop = block[bn].y + blockRadius;

			if (playerRight > blockLeft && playerLeft < blockRight)//キャラとブロックが縦で重なっているか
			{
				if (nextBottom < blockTop && nextTop > blockTop)//未来座標が上からブロックに当たったとき(着地時など)
				{
					*moveY = 0;
					*y = blockTop + playerRadius;
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
					*moveX = 0;
					*x = blockRight + playerRadius;
				}
				else if (nextRight > blockLeft && nextLeft < blockLeft)//未来座標が左からブロックに当たったとき
				{
					*moveX = 0;
					*x = blockLeft - playerRadius;
				}
			}
		}

	}
}

void Scene::MoveFloorHit(float* x, float* y, float* moveX, float* moveY, int* gcFlg)
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

	for (int mfn = 0; mfn < mFNum; mfn++) {
		if (moveFloor[mfn].flg) {
			//動く床のそれぞれ
			const float mFLeft = moveFloor[mfn].baseX - mFWidth;
			const float mFRight = moveFloor[mfn].baseX + mFWidth;
			const float mFBottom = moveFloor[mfn].y - mFHeight;
			const float mFTop = moveFloor[mfn].y + mFHeight;

			if (playerRight > mFLeft && playerLeft < mFRight)//キャラとブロックが縦で重なっているか
			{
				if (nextBottom < mFTop && nextTop > mFTop)//未来座標が上からブロックに当たったとき(着地時など)
				{
					*moveY = 0;
					*y = mFTop + playerRadius;
					*gcFlg |= GcMoveFloor;
				}
				else if (mFBottom < nextTop && nextBottom < mFBottom)//未来座標が下からブロックに当たったとき
				{
					if (moveFloor[mfn].moveY > 0) {
						*moveY = 0;
						*y = mFBottom - playerRadius;
					}
					else {
						*moveY = 0;
						*y = mFBottom - playerRadius - moveFloor[mfn].moveY - 10;
					}
				}
			}
			if (playerTop > mFBottom && playerBottom < mFTop)//キャラとブロックが横で重なっているか
			{
				if (nextLeft < mFRight && nextRight > mFRight)//未来座標が右からブロックに当たったとき
				{
					//*moveX = 0;
					//*x = mFRight + playerRadius;
				}
				else if (nextRight > mFLeft && nextLeft < mFLeft)//未来座標が左からブロックに当たったとき
				{
					//*moveX = 0;
					//*x = mFLeft - playerRadius;
				}
			}
		}
	}
}

void Scene::InitBlock()
{
	if (nowScene == HowtoPlay) {
		block[0] = { -260,-275,0,0,2,false,false };
	}
	else {
		for (int bn = 0; bn < blockNum; bn++) {
			block[bn] = { room[nowRoom].bl[bn].bX,room[nowRoom].bl[bn].bY, 0, 0, 2 + bn,false,false };
		}
	}
}

void Scene::UpdateBlock()
{
	if (nowScene == HowtoPlay) {
		block[0].moveX = 0;
		block[0].moveY -= gravity;

		//キャッチされたとき
		if (cflg % 2 == 1) {
			block[0].cFlg = true;
			//ブロックのキャッチフラグがtrue
			if (block[0].cFlg == true) {
				if (playerHorizontal == -1) {
					block[0].x = player.x + blockRadius + playerRadius;
				}
				if (playerHorizontal == 1) {
					block[0].x = player.x - blockRadius - playerRadius;
				}
				if (block[0].jumpFlg) {
					block[0].jumpFlg = false;
					block[0].moveY = jumpPower;
				}
			}
		}
		else {	//キャッチされなくなったら
			block[0].cFlg = false;
		}


		MapHit(&block[0].x, &block[0].y, &block[0].moveX, &block[0].moveY,&block[0].gcFlg, true, block[0].flg, block[0].cFlg);
		block[0].jumpFlg = false;
		block[0].x += block[0].moveX;
		block[0].y += block[0].moveY;
	}
	else {
		for (int bn = 0; bn < nowRoom + 1; bn++) {
			block[bn].moveX = 0;
			block[bn].moveY -= gravity;

			//キャッチされたとき
			if (cflg % 2 == 1) {
				//ブロックのキャッチフラグがtrue
				if (block[bn].cFlg == true) {
					//一定の距離から離れたらブロックを離す
					float a = block[bn].x - player.x;
					float b = block[bn].y - player.y;
					float c = sqrt(a * a + b * b);
					if (c > 75) {
						block[bn].cFlg = false;
						cflg = 0;
					}

					if (playerHorizontal == -1) {
						block[bn].x = player.x + blockRadius + playerRadius;
					}
					if (playerHorizontal == 1) {
						block[bn].x = player.x - blockRadius - playerRadius;
					}
					if (block[bn].jumpFlg) {
						block[bn].jumpFlg = false;
						block[bn].moveY = jumpPower;
					}
				}
				else {	//ブロックのキャッチフラグがfalse
					int CNTcflg = 0;
					for (int i = 0; i < 3; i++) {
						if (block[i].cFlg == false) {
							CNTcflg++;
						}
					}
					if (CNTcflg == 3) {	//全てのフラグが0
						float a = block[bn].x - player.x;
						float b = block[bn].y - player.y;
						float c = sqrt(a * a + b * b);
						if (c < 60) {
							block[bn].cFlg = true;
						}
					}
				}
			}
			else {	//キャッチされなくなったら
				block[bn].cFlg = false;
			}

			//落下したら初期値に戻る
			if (block[bn].y < mapChip[mapHeight - 1][0].y - 200) {
				block[bn].x = room[nowRoom].bl[bn].bX;
				block[bn].y = room[nowRoom].bl[bn].bY;
			}

			//gcFlgのリセット
			block[bn].gcFlg = 0;

			BlockHit(&block[bn].x, &block[bn].y, &block[bn].moveX, &block[bn].moveY, true, bn);
			MoveFloorHit(&block[bn].x, &block[bn].y, &block[bn].moveX, &block[bn].moveY,&block[bn].gcFlg);
			MapHit(&block[bn].x, &block[bn].y, &block[bn].moveX, &block[bn].moveY, &block[bn].gcFlg, true, block[bn].flg, block[bn].cFlg);

			//挟まったら初期値に戻る
			if (block[bn].gcFlg == GcHit) {
				block[bn].x = room[nowRoom].bl[bn].bX;
				block[bn].y = room[nowRoom].bl[bn].bY;
			}

			block[bn].jumpFlg = false;
			block[bn].x += block[bn].moveX;
			block[bn].y += block[bn].moveY;
		}
	}
}

void Scene::DrawBlock()
{
	if (nowScene == HowtoPlay) {
		SHADER.m_spriteShader.DrawBox(block[0].x - scroll.X, block[0].y - scroll.Y, 20, 20, &bcolor, true);
		SHADER.m_spriteShader.DrawBox(block[0].x - scroll.X, block[0].y - scroll.Y, 18, 18, &wcolor, true);
		color = { 1,0,0,1 };
		SHADER.m_spriteShader.DrawBox(block[0].x - scroll.X, block[0].y - scroll.Y, 18, 2, &color, true);
	}
	else {
		for (int bn = 0; bn < nowRoom + 1; bn++) {
			SHADER.m_spriteShader.DrawBox(block[bn].x - scroll.X, block[bn].y - scroll.Y, 20, 20, &bcolor, true);
			SHADER.m_spriteShader.DrawBox(block[bn].x - scroll.X, block[bn].y - scroll.Y, 18, 18, &wcolor, true);
			switch (block[bn].flg)
			{
			case 2:
				color = { 1,0,0,1 };
				SHADER.m_spriteShader.DrawBox(block[bn].x - scroll.X, block[bn].y - scroll.Y, 18, 2, &color, true);
				break;
			case 3:
				color = { 0,1,0,1 };
				SHADER.m_spriteShader.DrawBox(block[bn].x - scroll.X, block[bn].y - scroll.Y, 18, 2, &color, true);
				break;
			case 4:
				color = { 0,0,1,1 };
				SHADER.m_spriteShader.DrawBox(block[bn].x - scroll.X, block[bn].y - scroll.Y, 18, 2, &color, true);
				break;
			}
		}
	}
}



void Scene::InitMoveFloor()
{
	for (int mfn = 0; mfn < mFNum; mfn++) {
		if (0 <= mfn && mfn < 9) {
			moveFloor[mfn] = { 0,mapChip[mapHeight - 1][0].baseY - 100,1740,0,false };
		}
		if (9 <= mfn && mfn < mFNum) {
			moveFloor[mfn] = { 0,mapChip[0][0].baseY + 100,2140,0,false };
		}
	}
}

void Scene::UpdateMoveFloor()
{
	//0-8は左(上に上がる)
	for (int mfn = 0; mfn < 9; mfn++) {
		if (!moveFloor[mfn].flg) {
			if (clearFrame % 150 == 0) {	//150=2.5s
				moveFloor[mfn].flg = true;
				break;
			}
		}
		if (moveFloor[mfn].flg) {
			moveFloor[mfn].moveY = 1.5;

			if (moveFloor[mfn].y > mapChip[0][0].y + 200) {
				moveFloor[mfn].flg = false;
				moveFloor[mfn].y = mapChip[mapHeight - 1][0].baseY - 100;
			}

			moveFloor[mfn].x = moveFloor[mfn].baseX - scroll.X;
			moveFloor[mfn].y = moveFloor[mfn].y + moveFloor[mfn].moveY;
		}
	}

	//9-16は右(下に下がる)
	for (int mfn = 9; mfn < mFNum; mfn++) {
		if (!moveFloor[mfn].flg) {
			if (clearFrame % 150 == 0) {	//150=2.5s
				moveFloor[mfn].flg = true;
				break;
			}
 	      	}
		if (moveFloor[mfn].flg) {
			moveFloor[mfn].moveY = -1.5;

			if (moveFloor[mfn].y < mapChip[mapHeight - 1][0].y - 200) {
				moveFloor[mfn].flg = false;
				moveFloor[mfn].y = mapChip[0][0].baseY + 100;
			}

			moveFloor[mfn].x = moveFloor[mfn].baseX - scroll.X;
			moveFloor[mfn].y = moveFloor[mfn].y + moveFloor[mfn].moveY;
		}
	}
}

void Scene::DrawMoveFloor()
{
	if (nowRoom == 2) {
		for (int mfn = 0; mfn < mFNum; mfn++) {
			if (moveFloor[mfn].flg) {
				if (!changeStageFlg) {
					SHADER.m_spriteShader.DrawBox(moveFloor[mfn].x, moveFloor[mfn].y - scroll.Y, mFWidth, mFHeight, &bcolor, true);
					SHADER.m_spriteShader.DrawBox(moveFloor[mfn].x, moveFloor[mfn].y - scroll.Y, mFWidth - 2, mFHeight - 2, &wcolor, true);
				}
				else {
					SHADER.m_spriteShader.DrawBox(moveFloor[mfn].x, moveFloor[mfn].y - scroll.Y, mFWidth, mFHeight, &wcolor, true);
					SHADER.m_spriteShader.DrawBox(moveFloor[mfn].x, moveFloor[mfn].y - scroll.Y, mFWidth - 2, mFHeight - 2, &bcolor, true);
				}
				
			}
		}
	}
}

void Scene::CheckScore()
{
	FILE* fp;
	int maxScore;
	
	//maxScoreの読み込み
	fp = fopen("Score.txt", "r");
	int b = fscanf(fp, "%d", &maxScore);
	fclose(fp);

	//scoreの比較

	if (clearFrame < maxScore) {
		maxScoreFlg = true;
	}

	fp = fopen("Score.txt", "w");
	fprintf(fp, "%d", clearFrame);
	fclose(fp);
}

void Scene::LoadRoom()
{
	int bn = 0;
	FILE* fp;
	fp = fopen("Room.txt", "r");
	//文字読み込み処理
	
	for (int rn = 0; rn < roomNum; rn++) {
		bn = 0;
						//部屋番号,初期値x,y,ブロックの初期値x,y,脱出に必要なスコア
		int b = fscanf(fp, "%d%f%f%f%f%f%f%f%f%d", &room[rn].number, &room[rn].pX, &room[rn].pY,
												&room[rn].bl[0].bX, &room[rn].bl[0].bY, 
												&room[rn].bl[1].bX, &room[rn].bl[1].bY, 
												&room[rn].bl[2].bX, &room[rn].bl[2].bY, 
																			&room[rn].clearScore);
	}
	fclose(fp);
}

void Scene::NextRoom()
{
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
							doFadeInOut = true;
							nextRoomFlg = true;
						}
					}
				}
			}
		}
	}

	if (goNextRoomFlg) {
		nextRoomFlg = false;
		goNextRoomFlg = false;
		nowRoom++;
		player.x = room[nowRoom].pX;
		player.y = room[nowRoom].pY;
		score = 0;
		for (int bn = 0; bn < blockNum; bn++) {
			block[bn].x = room[nowRoom].bl[bn].bX;
			block[bn].y = room[nowRoom].bl[bn].bY;
		}
	}
}

void Scene::InitMap()
{
	for (int h = 0; h < mapHeight; h++) {
		for (int w = 0; w < mapWidth; w++) {
			mapChip[h][w] = { 0,0,(float)ScreenLeft + (40 * (w + 1)) - 20,(float)ScreenTop - (40 * h) - 20,0 };
		}
	}

	scroll.X = 0;
	scroll.XMin = mapChip[0][0].baseX + 300;
	scroll.XMax = mapChip[0][mapWidth - 1].baseX - 300;
	scroll.Y = 0;
	scroll.YMax = mapChip[0][0].baseY - 160;
	scroll.YMin = mapChip[mapHeight - 1][0].baseY + 160;

	//タイトルの時
	if (nowScene == Title) {
		for (int h = 0; h < mapHeight; h++) {
			for (int w = 0; w < mapWidth; w++) {
				if (h == 4 || h == mapHeight - 1) {
					mapChip[h][w].flg = 1;
				}
				if (w == 0 || w == 15) {
					mapChip[h][w].flg = 1;
				}
				if (h == mapHeight - 2 && w == 14) {
					mapChip[h][w].flg = 9;
				}
			}
		}
	}
	
	//HowtoPlayの時
	if (nowScene == HowtoPlay) {
		for (int h = 0; h < mapHeight; h++) {
			for (int w = 0; w < mapWidth; w++) {
				if (h == 4 || h == mapHeight - 1) {
					mapChip[h][w].flg = 1;
				}
				if (w == 0 || w == 15) {
					mapChip[h][w].flg = 1;
				}
				if (h == mapHeight - 1 && w == 9) {
					mapChip[h][w].flg = 2;
				}
				if (h == mapHeight - 2 && w == 14) {
					mapChip[h][w].flg = 9;
				}
			}
		}
	}

	//リザルトの時
	if (nowScene == Result) {
		for (int h = 0; h < mapHeight; h++) {
			for (int w = 0; w < mapWidth; w++) {
				if (h == 4 || h == mapHeight - 1) {
					mapChip[h][w].flg = 1;
				}
				if (w == 0 || w == 25) {
					mapChip[h][w].flg = 1;
				}
				if (h == mapHeight - 2 && (w == 24 || w==1)) {
					mapChip[h][w].flg = 9;
				}
			}
		}
		scroll.X = 0;
		scroll.XMin = mapChip[0][0].baseX + 300;
		scroll.XMax = mapChip[0][25].baseX - 300;
		scroll.Y = 0;
		scroll.YMax = mapChip[0][0].baseY - 160;
		scroll.YMin = mapChip[mapHeight - 1][0].baseY + 160;
	}
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

	//ブロック設置位置のランダム化
	int r;
	//room1
	r = rand() % 4 + 9;
	mapChip[6][r].flg = 2;//Red

	//room2
	r = rand() % 2;
	switch (r) 
	{
	case 0:
		r = rand() % 7 + 21;
		mapChip[4][r].flg = 2;//Red
		r = rand() % 12 + 31;
		mapChip[mapHeight - 1][r].flg = 3;//Green
		break;
	case 1:
		r = rand() % 7 + 21;
		mapChip[4][r].flg = 3;
		r = rand() % 12 + 31;
		mapChip[mapHeight - 1][r].flg = 2;
		break;
	}
	
	//room3
	r = rand() % 3;
	switch (r)
	{
	case 0:
		r = rand() % 5 + 54;
		mapChip[8][r].flg = 2;//Red
		r = rand() % 5 + 44;
		mapChip[4][r].flg = 3;//Green
		r = rand() % 5 + 64;
		mapChip[4][r].flg = 4;//Blue
		break;
	case 1:
		r = rand() % 5 + 54;
		mapChip[8][r].flg = 3;
		r = rand() % 5 + 44;
		mapChip[4][r].flg = 4;
		r = rand() % 5 + 64;
		mapChip[4][r].flg = 2;
		break;
	case 2:
		r = rand() % 5 + 54;
		mapChip[8][r].flg = 4;
		r = rand() % 5 + 44;
		mapChip[4][r].flg = 2;
		r = rand() % 5 + 64;
		mapChip[4][r].flg = 3;
		break;
	default:
		break;
	}

}

void Scene::UpdateMap()
{
	//タイトルの時
	if (nowScene == Title) {
		scroll.Y = player.y;

		if (scroll.Y > scroll.YMax) {
			scroll.Y = scroll.YMax;
		}
		if (scroll.Y < scroll.YMin) {
			scroll.Y = scroll.YMin;
		}

		for (int h = 0; h < mapHeight; h++) {
			for (int w = 0; w < mapWidth; w++) {
				mapChip[h][w].x = mapChip[h][w].baseX;
				mapChip[h][w].y = mapChip[h][w].baseY - scroll.Y;
			}
		}
	}

	//HowtoPlayの時
	if (nowScene == HowtoPlay) {
		scroll.Y = player.y;

		if (scroll.Y > scroll.YMax) {
			scroll.Y = scroll.YMax;
		}
		if (scroll.Y < scroll.YMin) {
			scroll.Y = scroll.YMin;
		}

		for (int h = 0; h < mapHeight; h++) {
			for (int w = 0; w < mapWidth; w++) {
				mapChip[h][w].x = mapChip[h][w].baseX;
				mapChip[h][w].y = mapChip[h][w].baseY - scroll.Y;
			}
		}
	}

	//ゲームの時
	if (nowScene == Game) {
		scroll.X = player.x;
		scroll.Y = player.y;

		switch (nowRoom)
		{
		case 0:
			scroll.XMin = mapChip[0][0].baseX + 300;
			scroll.XMax = mapChip[0][16].baseX - 300;
			scroll.YMax = mapChip[2][0].baseY - 160;
			break;
		case 1:
			scroll.XMin = mapChip[0][16].baseX + 300;
			scroll.XMax = mapChip[0][43].baseX - 300;
			scroll.YMax = mapChip[0][0].baseY - 160;
			break;
		case 2:
			scroll.XMin = mapChip[0][43].baseX + 300;
			scroll.XMax = mapChip[0][mapWidth - 1].baseX - 300;
			scroll.YMax = mapChip[0][0].baseY - 160;
			break;
		}

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
	
	//リザルトの時
	if (nowScene == Result) {
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

		
	

		
}

void Scene::DrawMap()
{
	if (nowScene != Title) {
		if (!changeStageFlg) {
			SHADER.m_spriteShader.DrawBox(0, 0, 1000, 1000, &wcolor, true);
		}
		else {
			SHADER.m_spriteShader.DrawBox(0, 0, 1000, 1000, &bcolor, true);
		}
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
			if (mapChip[h][w].flg == 3) {
				if (!changeStageFlg) {
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y, 20, 20, &bcolor, true);
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y, 18, 18, &wcolor, true);
				}
				else {
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y, 20, 20, &wcolor, true);
					color = { 0,1,0,1 };
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y, 18, 18, &color, true);
				}
			}
			if (mapChip[h][w].flg == 4) {
				if (!changeStageFlg) {
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y, 20, 20, &bcolor, true);
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y, 18, 18, &wcolor, true);
				}
				else {
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y, 20, 20, &wcolor, true);
					color = { 0,0,1,1 };
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y, 18, 18, &color, true);
				}
			}
			if (mapChip[h][w].flg == 9 || mapChip[h][w].flg == 8) {
				if (!changeStageFlg) {
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y + 10, 20, 30, &bcolor, true);
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y + 10, 18, 28, &wcolor, true);
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y + 19, 11, 13, &bcolor, false);
				}
				else {
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y + 10, 20, 30, &wcolor, true);
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y + 10, 18, 28, &bcolor, true);
					SHADER.m_spriteShader.DrawBox(mapChip[h][w].x, mapChip[h][w].y + 19, 11, 13, &wcolor, false);
				}
			}

			//SHADER.m_spriteShader.DrawCircle(mapChip[h][w].baseX, mapChip[h][w].baseY, 3, &bcolor, true);
		}
	}
	
}

void Scene::DrawHtpString()
{
	char str[500];
	char str2[500];
	switch (htpScene)
	{
	case 0:
		sprintf_s(str, sizeof(str), " このゲームは指定された場所に\nブロックを置いていくゲームだよ");
		SHADER.m_spriteShader.DrawString(-210, 110, str, Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		break;
	case 1:
		sprintf_s(str, sizeof(str), "     左右移動は←→キー、\nジャンプはSpaceキーでできるよ");
		SHADER.m_spriteShader.DrawString(-200, 110, str, Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		break;
	case 2:
		sprintf_s(str, sizeof(str), "    ブロックをどこに置くか\n分からないときはMキーを押すと、");
		SHADER.m_spriteShader.DrawString(-210, 110, str, Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		break;
	case 3:
		if (fadeFrame == 0) {
			sprintf_s(str, sizeof(str), "ブロックの置く場所が分かるようになるよ");
			SHADER.m_spriteShader.DrawString(-270, 110, str, Math::Vector4(1, 1, 1, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		}
		break;
	case 4:
		sprintf_s(str, sizeof(str), " この状態ではキャラは動かせるけど、\nブロックは移動させることはできないよ");
		SHADER.m_spriteShader.DrawString(-250, 110, str, Math::Vector4(1, 1, 1, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		break;
	case 5:
		sprintf_s(str, sizeof(str), "もう一度Mキーを押すと元の状態に戻るよ");
		SHADER.m_spriteShader.DrawString(-260, 110, str, Math::Vector4(1, 1, 1, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		break;
	case 6:
		break;
	case 7:
		sprintf_s(str, sizeof(str), "ブロックはshiftキーで掴めるよ");
		SHADER.m_spriteShader.DrawString(-210, 110, str, Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		break;
	case 8:
		sprintf_s(str, sizeof(str), "ブロックを指定された場所に置いたら");
		SHADER.m_spriteShader.DrawString(-240, 110, str, Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		break;
	case 9:
		sprintf_s(str, sizeof(str), "ドアに行くと次の部屋に行けるよ");
		SHADER.m_spriteShader.DrawString(-220, 110, str, Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		break;
	case 10:
		sprintf_s(str, sizeof(str), "では、最速クリアタイムを\n  目指して頑張ってね！");
		SHADER.m_spriteShader.DrawString(-175, 110, str, Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		break;

	default:
		break;
	}

	sprintf_s(str, sizeof(str2), "Enterキーで次へ");
	if (frame - enterFrame >= 100) {
		if (htpScene < 8) {
			if (!changeStageFlg) {
				SHADER.m_spriteShader.DrawString(10, -110, str, Math::Vector4(0, 0, 0, 1));
			}
			else {
				SHADER.m_spriteShader.DrawString(10, -110, str, Math::Vector4(1, 1, 1, 1));
			}
		}
		else {
			SHADER.m_spriteShader.DrawString(10, -70, str, Math::Vector4(0, 0, 0, 1));
		}
	}
}

void Scene::DrawREffect()
{
	float a, b, c = 100, deg;

	for (c = 0; c < 100; c += 15) {
		color = { rand() / 32767.0f,rand() / 32767.0f,rand() / 32767.0f,1 };
		for (deg = 0; deg < 360; deg += 10)//円1周分のループ
		{
			a = cos(deg * 3.14f / 180) * c;
			b = sin(deg * 3.14f / 180) * c;
			SHADER.m_spriteShader.DrawCircle(-100 + a - scroll.X, 0 + b, 3, &color);
		}
	}
	for (c = 0; c < 100; c += 15) {
		color = { rand() / 32767.0f,rand() / 32767.0f,rand() / 32767.0f,1 };
		for (deg = 0; deg < 360; deg += 10)//円1周分のループ
		{
			a = cos(deg * 3.14f / 180) * c;
			b = sin(deg * 3.14f / 180) * c;
			SHADER.m_spriteShader.DrawCircle(500 + a - scroll.X, 0 + b, 3, &color);
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



