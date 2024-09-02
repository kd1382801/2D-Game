#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	//���݂̃V�[���̕`�揈�������s
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
	//���݂̃V�[���̍X�V���������s
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
	//�����̏�����
	srand(timeGetTime());

	//�Q�[���J�n�ŏ��̃V�[��:�^�C�g��
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
		SHADER.m_spriteShader.DrawString(0, -80, "�����L�[ �F�I��\nEnter�L�[�F����", Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
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
	//���̎��������s
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
	//�L�[�������ꂽ��F���]
	if (GetAsyncKeyState('M') & 0x8000) {
		if (!doFadeInOut) {
			doFadeInOut = true;
		}
	}
	

	UpdateMoveFloor();
	UpdatePlayer();

	//���̎��������s
	if (!changeStageFlg) {
		UpdateBlock();
		NextRoom();
	}

	UpdateMap();

	//3�N���A�����烊�U���g�ֈړ�
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

	SHADER.m_spriteShader.DrawString(70 - scroll.X, 75, "  �X�e�[�W�N���A  ", Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
	
	char str[80];
	if (clearFrame >= 60 * 60) {
		sprintf_s(str, sizeof(str), "�N���A�^�C���F%2d��%2d�b", clearFrame / 3600, clearFrame % 3600 / 60);
	}
	else {
		sprintf_s(str, sizeof(str), "�N���A�^�C���F%2d�b", clearFrame / 60);
	}
	SHADER.m_spriteShader.DrawString(70 - scroll.X, 35, str, Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f ));
	
	if (maxScoreFlg) {
		sprintf_s(str, sizeof(str), "�ő��N���A�^�C���X�V�I");
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

	//���̎��������s
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

	//�^�C�g���ɖ߂�
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
	//nowScene���^�C�g���̎�
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
				doFadeInOut = true;//�t�F�[�h�C���ŃV�[���`�F���W
			}
		}

		player.x += player.moveX;
	}

	//nowScene��HowtoPlay�̎�
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
			//�u���b�N��1�O�Ŏ~�܂�
			if (player.x <= mapChip[mapHeight - 1][2].x) {
				player.moveX = 0;
				cflg = 1;
			}
			break;
		case 8:
			player.moveX = 4.5;
			playerHorizontal = 1;
			//�ԃ}�X��1��łƂ܂�
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
			//�h�A�̏ꏊ�ɗ�������
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

	//nowScene���Q�[���̎�
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

		//�u���b�N�L���b�`
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
			if (!keyflg.shift) {
				keyflg.shift = true;
				cflg += 1;
			}
		}
		else {
			keyflg.shift = false;
		}
		//���������珉���l�ɖ߂�
		if (player.y < mapChip[mapHeight - 1][0].y - 300) {
			player.x = room[nowRoom].pX;
			player.y = room[nowRoom].pY;
		}

		//gcFlg�̏�����
		player.gcFlg = 0;

		BlockHit(&player.x, &player.y, &player.moveX, &player.moveY);
		MoveFloorHit(&player.x, &player.y, &player.moveX, &player.moveY,&player.gcFlg);
		MapHit(&player.x, &player.y, &player.moveX, &player.moveY,&player.gcFlg);

		//���܂����珉���l�ɖ߂�
		if (player.gcFlg == GcHit) {
			player.x = room[nowRoom].pX;
			player.y = room[nowRoom].pY;
		}

		//�W�����v�t���O
		if (player.gcFlg & GcMoveFloor) {
			player.jumpFlg = false;
		}

		player.x += player.moveX;
		player.y += player.moveY;
	}

	//nowScene�����U���g�̎�
	if (nowScene == Result) {
		player.moveX = 2;

		//�h�A�̏ꏊ�ɗ�������
		if (player.x >= mapChip[mapHeight - 2][24].baseX) {
			player.moveX = 0;
			doFadeInOut = true;//�t�F�[�h�C���ŃV�[���`�F���W
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

	//�������W
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

				if (playerRight > chipLeft && playerLeft < chipRight)//�L�����ƃu���b�N���c�ŏd�Ȃ��Ă��邩
				{
					if (nextBottom < chipTop && nextTop > chipTop)//�������W���ォ��u���b�N�ɓ��������Ƃ�(���n���Ȃ�)
					{
						*moveY = 0;
						*y = chipTop + playerRadius;
						//�u���b�N�Ȃ�X�R�A�v�Z
						if (block) {
							if (cFlg == false) {
								if (mapChip[h][w].flg == blockColor) {
									//�u���b�N�ƃ}�b�v�̋������v�Z
									float a = mapChip[h][w].baseX - *x;
									float b = mapChip[h][w].baseY - *y;
									float c = sqrt(a * a + b * b);
									//�^�゠����ɒu�����Ƃ�
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
					else if (chipBottom < nextTop && nextBottom < chipBottom)//�������W��������u���b�N�ɓ��������Ƃ�
					{
						*moveY = 0;
						*y = chipBottom - playerRadius;
						*gcFlg |= GcTop;
					}
				}

				if (playerTop > chipBottom && playerBottom < chipTop)//�L�����ƃu���b�N�����ŏd�Ȃ��Ă��邩
				{
					if (nextLeft < chipRight && nextRight > chipRight)//�������W���E����u���b�N�ɓ��������Ƃ�
					{
						*moveX = 0;
						*x = chipRight + playerRadius;
					}
					else if (nextRight > chipLeft && nextLeft < chipLeft)//�������W��������u���b�N�ɓ��������Ƃ�
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

		//�������W
		const float nextLeft = playerLeft + *moveX;
		const float nextRight = playerRight + *moveX;
		const float nextBottom = playerBottom + *moveY;
		const float nextTop = playerTop + *moveY;

		for (int bn = 0; bn < nowRoom + 1; bn++) {
			if (bl == true && bn == blockNum) {
				continue;
			}
			//�u���b�N���W
			const float blockLeft = block[bn].x - blockRadius;
			const float blockRight = block[bn].x + blockRadius;
			const float blockBottom = block[bn].y - blockRadius;
			const float blockTop = block[bn].y + blockRadius;

			if (playerRight > blockLeft && playerLeft < blockRight)//�L�����ƃu���b�N���c�ŏd�Ȃ��Ă��邩
			{
				if (nextBottom < blockTop && nextTop > blockTop)//�������W���ォ��u���b�N�ɓ��������Ƃ�(���n���Ȃ�)
				{
					*moveY = 0;
					*y = blockTop + playerRadius;
				}
				//else if (blockBottom < nextTop && nextBottom < blockBottom)//�������W��������u���b�N�ɓ��������Ƃ�
				//{
				//	player.moveY = 0;
				//	player.y = blockBottom - playerRadius;
				//}
			}

			if (playerTop > blockBottom && playerBottom < blockTop)//�L�����ƃu���b�N�����ŏd�Ȃ��Ă��邩
			{
				if (nextLeft < blockRight && nextRight > blockRight)//�������W���E����u���b�N�ɓ��������Ƃ�
				{
					*moveX = 0;
					*x = blockRight + playerRadius;
				}
				else if (nextRight > blockLeft && nextLeft < blockLeft)//�������W��������u���b�N�ɓ��������Ƃ�
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

	//�������W
	const float nextLeft = playerLeft + *moveX;
	const float nextRight = playerRight + *moveX;
	const float nextBottom = playerBottom + *moveY;
	const float nextTop = playerTop + *moveY;

	for (int mfn = 0; mfn < mFNum; mfn++) {
		if (moveFloor[mfn].flg) {
			//�������̂��ꂼ��
			const float mFLeft = moveFloor[mfn].baseX - mFWidth;
			const float mFRight = moveFloor[mfn].baseX + mFWidth;
			const float mFBottom = moveFloor[mfn].y - mFHeight;
			const float mFTop = moveFloor[mfn].y + mFHeight;

			if (playerRight > mFLeft && playerLeft < mFRight)//�L�����ƃu���b�N���c�ŏd�Ȃ��Ă��邩
			{
				if (nextBottom < mFTop && nextTop > mFTop)//�������W���ォ��u���b�N�ɓ��������Ƃ�(���n���Ȃ�)
				{
					*moveY = 0;
					*y = mFTop + playerRadius;
					*gcFlg |= GcMoveFloor;
				}
				else if (mFBottom < nextTop && nextBottom < mFBottom)//�������W��������u���b�N�ɓ��������Ƃ�
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
			if (playerTop > mFBottom && playerBottom < mFTop)//�L�����ƃu���b�N�����ŏd�Ȃ��Ă��邩
			{
				if (nextLeft < mFRight && nextRight > mFRight)//�������W���E����u���b�N�ɓ��������Ƃ�
				{
					//*moveX = 0;
					//*x = mFRight + playerRadius;
				}
				else if (nextRight > mFLeft && nextLeft < mFLeft)//�������W��������u���b�N�ɓ��������Ƃ�
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

		//�L���b�`���ꂽ�Ƃ�
		if (cflg % 2 == 1) {
			block[0].cFlg = true;
			//�u���b�N�̃L���b�`�t���O��true
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
		else {	//�L���b�`����Ȃ��Ȃ�����
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

			//�L���b�`���ꂽ�Ƃ�
			if (cflg % 2 == 1) {
				//�u���b�N�̃L���b�`�t���O��true
				if (block[bn].cFlg == true) {
					//���̋������痣�ꂽ��u���b�N�𗣂�
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
				else {	//�u���b�N�̃L���b�`�t���O��false
					int CNTcflg = 0;
					for (int i = 0; i < 3; i++) {
						if (block[i].cFlg == false) {
							CNTcflg++;
						}
					}
					if (CNTcflg == 3) {	//�S�Ẵt���O��0
						float a = block[bn].x - player.x;
						float b = block[bn].y - player.y;
						float c = sqrt(a * a + b * b);
						if (c < 60) {
							block[bn].cFlg = true;
						}
					}
				}
			}
			else {	//�L���b�`����Ȃ��Ȃ�����
				block[bn].cFlg = false;
			}

			//���������珉���l�ɖ߂�
			if (block[bn].y < mapChip[mapHeight - 1][0].y - 200) {
				block[bn].x = room[nowRoom].bl[bn].bX;
				block[bn].y = room[nowRoom].bl[bn].bY;
			}

			//gcFlg�̃��Z�b�g
			block[bn].gcFlg = 0;

			BlockHit(&block[bn].x, &block[bn].y, &block[bn].moveX, &block[bn].moveY, true, bn);
			MoveFloorHit(&block[bn].x, &block[bn].y, &block[bn].moveX, &block[bn].moveY,&block[bn].gcFlg);
			MapHit(&block[bn].x, &block[bn].y, &block[bn].moveX, &block[bn].moveY, &block[bn].gcFlg, true, block[bn].flg, block[bn].cFlg);

			//���܂����珉���l�ɖ߂�
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
	//0-8�͍�(��ɏオ��)
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

	//9-16�͉E(���ɉ�����)
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
	
	//maxScore�̓ǂݍ���
	fp = fopen("Score.txt", "r");
	int b = fscanf(fp, "%d", &maxScore);
	fclose(fp);

	//score�̔�r

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
	//�����ǂݍ��ݏ���
	
	for (int rn = 0; rn < roomNum; rn++) {
		bn = 0;
						//�����ԍ�,�����lx,y,�u���b�N�̏����lx,y,�E�o�ɕK�v�ȃX�R�A
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
			//mapChip���h�A�̎��̏���
			if (mapChip[h][w].flg == 9) {
				//���̎��������s
				if (!changeStageFlg) {
					const float playerLeft = player.x - playerRadius;
					const float playerRight = player.x + playerRadius;
					const float playerBottom = player.y - playerRadius;
					const float playerTop = player.y + playerRadius;

					const float chipLeft = mapChip[h][w].baseX - mapRadius;
					const float chipRight = mapChip[h][w].baseX + mapRadius;
					const float chipBottom = mapChip[h][w].baseY - mapRadius;
					const float chipTop = mapChip[h][w].baseY + mapRadius;

					if (playerRight > chipLeft && playerLeft < chipRight && playerTop > chipBottom && playerBottom < chipTop) {	//�c�Ɖ��ŏd�Ȃ��Ă��邩
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

	//�^�C�g���̎�
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
	
	//HowtoPlay�̎�
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

	//���U���g�̎�
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
	//�����ǂݍ��ݏ���
	char ch;
	int h = 0;	//�c�̃J�E���^
	int w = 0;	//���̃J�E���^
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

	//�u���b�N�ݒu�ʒu�̃����_����
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
	//�^�C�g���̎�
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

	//HowtoPlay�̎�
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

	//�Q�[���̎�
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
	
	//���U���g�̎�
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
		sprintf_s(str, sizeof(str), " ���̃Q�[���͎w�肳�ꂽ�ꏊ��\n�u���b�N��u���Ă����Q�[������");
		SHADER.m_spriteShader.DrawString(-210, 110, str, Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		break;
	case 1:
		sprintf_s(str, sizeof(str), "     ���E�ړ��́����L�[�A\n�W�����v��Space�L�[�łł����");
		SHADER.m_spriteShader.DrawString(-200, 110, str, Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		break;
	case 2:
		sprintf_s(str, sizeof(str), "    �u���b�N���ǂ��ɒu����\n������Ȃ��Ƃ���M�L�[�������ƁA");
		SHADER.m_spriteShader.DrawString(-210, 110, str, Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		break;
	case 3:
		if (fadeFrame == 0) {
			sprintf_s(str, sizeof(str), "�u���b�N�̒u���ꏊ��������悤�ɂȂ��");
			SHADER.m_spriteShader.DrawString(-270, 110, str, Math::Vector4(1, 1, 1, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		}
		break;
	case 4:
		sprintf_s(str, sizeof(str), " ���̏�Ԃł̓L�����͓������邯�ǁA\n�u���b�N�͈ړ������邱�Ƃ͂ł��Ȃ���");
		SHADER.m_spriteShader.DrawString(-250, 110, str, Math::Vector4(1, 1, 1, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		break;
	case 5:
		sprintf_s(str, sizeof(str), "������xM�L�[�������ƌ��̏�Ԃɖ߂��");
		SHADER.m_spriteShader.DrawString(-260, 110, str, Math::Vector4(1, 1, 1, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		break;
	case 6:
		break;
	case 7:
		sprintf_s(str, sizeof(str), "�u���b�N��shift�L�[�Œ͂߂��");
		SHADER.m_spriteShader.DrawString(-210, 110, str, Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		break;
	case 8:
		sprintf_s(str, sizeof(str), "�u���b�N���w�肳�ꂽ�ꏊ�ɒu������");
		SHADER.m_spriteShader.DrawString(-240, 110, str, Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		break;
	case 9:
		sprintf_s(str, sizeof(str), "�h�A�ɍs���Ǝ��̕����ɍs�����");
		SHADER.m_spriteShader.DrawString(-220, 110, str, Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		break;
	case 10:
		sprintf_s(str, sizeof(str), "�ł́A�ő��N���A�^�C����\n  �ڎw���Ċ撣���ĂˁI");
		SHADER.m_spriteShader.DrawString(-175, 110, str, Math::Vector4(0, 0, 0, 1 - ((fadeFrame - 1) / 30.0f * 99 + 1) / 100.0f));
		break;

	default:
		break;
	}

	sprintf_s(str, sizeof(str2), "Enter�L�[�Ŏ���");
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
		for (deg = 0; deg < 360; deg += 10)//�~1�����̃��[�v
		{
			a = cos(deg * 3.14f / 180) * c;
			b = sin(deg * 3.14f / 180) * c;
			SHADER.m_spriteShader.DrawCircle(-100 + a - scroll.X, 0 + b, 3, &color);
		}
	}
	for (c = 0; c < 100; c += 15) {
		color = { rand() / 32767.0f,rand() / 32767.0f,rand() / 32767.0f,1 };
		for (deg = 0; deg < 360; deg += 10)//�~1�����̃��[�v
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



