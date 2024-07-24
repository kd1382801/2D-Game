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
	}
}

void Scene::Init()
{
	//�����̏�����
	srand(timeGetTime());

	//�Q�[���J�n�ŏ��̃V�[��:�^�C�g��
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
	//���̎��������s
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
	
	//�L�[�������ꂽ��F���]
	if (GetAsyncKeyState('M') & 0x8000) {
		if (!doFadeInOut) {
			doFadeInOut = true;
		}
	}
	

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
	}
}

void Scene::DrawResult()
{
	SHADER.m_spriteShader.DrawBox(0, 0, 320, 180, &wcolor, true);


	char str[80];
	sprintf_s(str, sizeof(str), "�N���A�^�C���F%d�b", clearFrame / 60);
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
	//�u���b�N�L���b�`
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
		if (!shiftKeyFlg) {
			shiftKeyFlg = true;
			cflg += 1;
		}
	}
	else {
		shiftKeyFlg = false;
	}
	//���������珉���l�ɖ߂�
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
			if (mapChip[h][w].flg != 9) {
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
						if (block) {
							if (mapChip[h][w].flg == blockColor) {
								score |= Red;
							}
							else {
								score = 0;
							}
						}
					}
					else if (chipBottom < nextTop && nextBottom < chipBottom)//�������W��������u���b�N�ɓ��������Ƃ�
					{
						*moveY = 0;
						*y = chipBottom - playerRadius;

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

void Scene::BlockHit()
{
	if (!changeStageFlg) {
		const float playerLeft = player.x - playerRadius;
		const float playerRight = player.x + playerRadius;
		const float playerBottom = player.y - playerRadius;
		const float playerTop = player.y + playerRadius;

		//�������W
		const float nextLeft = playerLeft + player.moveX;
		const float nextRight = playerRight + player.moveX;
		const float nextBottom = playerBottom + player.moveY;
		const float nextTop = playerTop + player.moveY;

		//�u���b�N���W
		const float blockLeft = block.x - blockRadius;
		const float blockRight = block.x + blockRadius;
		const float blockBottom = block.y - blockRadius;
		const float blockTop = block.y + blockRadius;

		if (playerRight > blockLeft && playerLeft < blockRight)//�L�����ƃu���b�N���c�ŏd�Ȃ��Ă��邩
		{
			if (nextBottom < blockTop && nextTop > blockTop)//�������W���ォ��u���b�N�ɓ��������Ƃ�(���n���Ȃ�)
			{
				player.moveY = 0;
				player.y = blockTop + playerRadius;
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
				player.moveX = 0;
				player.x = blockRight + playerRadius;
			}
			else if (nextRight > blockLeft && nextLeft < blockLeft)//�������W��������u���b�N�ɓ��������Ƃ�
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

	//�L���b�`���ꂽ�Ƃ�
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
	
	//���������珉���l�ɖ߂�
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
	//�����ǂݍ��ݏ���
	
	for (int rn = 0; rn < roomNum; rn++) {
						//�����ԍ�,�����lx,y,�u���b�N�̏����lx,y,�E�o�ɕK�v�ȃX�R�A
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



