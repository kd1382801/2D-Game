#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	Math::Rectangle chipRect;
	for (int h = 0; h < mapHeight; h++)
	{
		for (int w = 0; w < mapWidth; w++)
		{
			switch (mapData[h][w])
			{
			case 0:
				chipRect = { 0,0,64,64 };
				break;
			case 1:
				chipRect = { 64,0,64,64 };
				break;
			case 2:
				chipRect = { 128,0,64,64 };
				break;
			/*case 3:
				chipRect = { 192,0,64,64 };
				break;*/
			default:
				break;
			}
			SHADER.m_spriteShader.SetMatrix(mapMat[h][w]);
			SHADER.m_spriteShader.DrawTex(&mapTex, chipRect, 1.0f);
		}
	}
	
	//�G�`��
	EnemyDraw();

	SHADER.m_spriteShader.SetMatrix(player.mat);				//�摜�ɍs����Z�b�g
	SHADER.m_spriteShader.DrawTex_Color(&charaTex, Math::Rectangle{ 0,0,64,64 }, &player.color);	//�摜�̕`��

	Math::Matrix lineMat = DirectX::XMMatrixIdentity();//�P�ʍs��@�s��̏�����
	SHADER.m_spriteShader.SetMatrix(lineMat);
	SHADER.m_spriteShader.DrawLine(0.0f, 360.0f, 0.0f, -360.0f, &Math::Color(1.0f, 0.0f, 0.0f, 1.0f));
}

void Scene::Update()
{
	player.scaleMat = Math::Matrix::CreateScale(player.scaleX, 1, 1);					//�g��s��̍쐬
	player.rotationMat = Math::Matrix::CreateRotationZ(ToRadians(player.rotation));		//��]�s��̍쐬�@ToRadians()�Ń��W�A���ɕϊ�
	player.transMat = Math::Matrix::CreateTranslation(player.x - scrollX, player.y, 0);	//�ړ��s��̍쐬
	player.mat = player.scaleMat * player.rotationMat * player.transMat;				//�s��̍���
	//�s����������鎞�ɂ́u�g�偨��]���ړ��v�̏��Ŋ|���Z������
	
	//�g��FCreateScale
	//��]�FCreateRotation(X,Y,Z)����{�I��2D�ł�Z�����g��Ȃ�
	//�ړ��FCreateTranslation

	player.moveY -= Gravity;//��������
	player.moveX = 0;//1�t���[�����̈ړ��ʂ����


	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) 
	{
		player.moveX += 5.0f;
		player.scaleX = 1.0f;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) 
	{
		player.moveX -= 5.0f;
		player.scaleX = -1.0f;
	}
	if (GetAsyncKeyState('R') & 0x8000)
	{
		player.rotation += 5.0f;
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (playerJumpFlg == false)
		{
			player.moveY = JumpPower;
			playerJumpFlg = true;
		}
	}
	/*else 
	{
		playerJumpFlg = false;
	}*/

	PlayerMapHit();
	PlayerEnemyHit();

	player.x += player.moveX;//X���W�̐ݒ�
	player.y += player.moveY;//Y���W�̐ݒ�

	scrollX = player.x;
	if (scrollX < scrollMin)scrollX = scrollMin;
	if (scrollX > scrollMax)scrollX = scrollMax;
	//���n�ʔ���
	/*if (player.y < -200)
	{
		player.y = -200.0f;
		player.moveY = 0.0f;
	}*/

	//std::cout << player.x << " " << player.y << std::endl;	//�W���o�͂�player.x,player.y����������
	//std::cout �W���o�͂ɗv�f���������ނ��߂Ɏg�p�����
	// <<		�E�ӂ̒l���W���o�͂ɏ������܂��
	//std::endl	���s����(\n)����������ŉ��s���s���A�o�b�t�@�̓��e���t���b�V��(�������̃f�[�^��f���o��)������
	//printf("%.2f %.2f\n", player.x, player.y);������ł�ok

	//�G�X�V
	EnemyUpdate();

	//�}�b�v�̈ړ��s��̍쐬
	for (int h = 0; h < mapHeight; h++)
	{
		for (int w = 0; w < mapWidth; w++)
		{
			mapMat[h][w] = Math::Matrix::CreateTranslation(mapX[h][w] - scrollX, mapY[h][w], 0);
		}
	}
}

void Scene::Init()
{
	CreateConsole();

	// �摜�̓ǂݍ��ݏ���
	charaTex.Load("Texture/Player/Chara.png");

	player.x = 0.0f;
	player.y = 0.0f;
	player.moveX = 0.0f;
	player.moveY = 0.0f;
	player.scaleX = 1.0f;
	player.rotation = 0.0f;
	playerJumpFlg = true;
	

	LoadMap();
	mapTex.Load("Texture/Map/MapChip.png");
	for (int h = 0; h < mapHeight; h++) 
	{
		for (int w = 0; w < mapWidth; w++) 
		{
			mapX[h][w] = (64.0f * w) - 640;
			mapY[h][w] = (-64.0f * h) + 360;
		}
	}

	scrollX = 0.0f;
	scrollMin = mapX[0][0] + 640;
	scrollMax = mapX[0][mapWidth - 1] - 640;

	//�G������
	EnemyInit();

}

void Scene::Release()
{
	DestroyConsole();
	
	// �摜�̉������
	charaTex.Release();
	mapTex.Release();
}

void Scene::ImGuiUpdate()
{
	return;

	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Once);

	// �f�o�b�O�E�B���h�E
	if (ImGui::Begin("Debug Window"))
	{
		ImGui::Text("FPS : %d", APP.m_fps);
	}
	ImGui::End();
}

void Scene::CreateConsole()
{
	AllocConsole();//�R���\�[�����J��
	freopen_s(&fp_c,"CONOUT$","w",stdout);//stdout:�W���o��



	//freopen_s(&fp_c, "CONIN$", "r", stdin);stdin:�W������
}

void Scene::DestroyConsole()
{
	fclose(fp_c);
	FreeConsole();//�R���\�[�������
}

void Scene::LoadMap()
{
	printf("\n�}�b�v�̏�����\n");

	for (int h = 0; h < mapHeight; h++)
	{
		for (int w = 0; w < mapWidth; w++)
		{
			mapData[h][w] = 0;
			printf("%d", mapData[h][w]);
		}
		printf("\n");
	}

	printf("\n�}�b�v�̓ǂݍ��ݏ���\n");
	FILE* fp;
	if ((fp = fopen("Texture/Map/Stage.txt", "r")) != nullptr)	//nullptr:null�|�C���^
	{
		printf("\n�t�@�C���̓ǂݍ��ݐ���\n");

		//�����ǂݍ��ݏ���
		char ch;
		int h = 0;	//�c�̃J�E���^
		int w = 0;	//���̃J�E���^

		while ((ch = fgetc(fp)) != EOF)
		{
			if (ch == '\n')
			{
				h++;
				w = 0;
			}
			else 
			{
				mapData[h][w] = atoi(&ch);
				w++;
			}
		}
		fclose(fp);
	}
	else 
	{
		printf("\n�t�@�C���̓ǂݍ��ݎ��s\n");
	}

	printf("\n�ǂݍ��񂾌���\n");
	for (int h = 0; h < mapHeight; h++)
	{
		for (int w = 0; w < mapWidth; w++)
		{
			printf("%d", mapData[h][w]);
		}
		printf("\n");
	}
}

void Scene::PlayerMapHit()
{
	const float playerLeft = player.x - charaRadius;
	const float playerRight = player.x + charaRadius;
	const float playerBottom = player.y - charaRadius;
	const float playerTop = player.y + charaRadius;

	//�������W
	const float nextLeft = playerLeft + player.moveX;
	const float nextRight = playerRight + player.moveX;
	const float nextBottom = playerBottom + player.moveY;
	const float nextTop = playerTop + player.moveY;

	for (int h = 0; h < mapHeight; h++)
	{
		for (int w = 0; w < mapWidth; w++)
		{
			if (mapData[h][w] == 0)//mapData[h][w] != 1
			{
				continue;
			}

			const float chipLeft = mapX[h][w] - mapRadius;
			const float chipRight = mapX[h][w] + mapRadius;
			const float chipBottom = mapY[h][w] - mapRadius;
			const float chipTop = mapY[h][w] + mapRadius;

			if (playerRight > chipLeft && playerLeft < chipRight)//�L�����ƃu���b�N���c�ŏd�Ȃ��Ă��邩
			{
				if (nextBottom < chipTop && nextTop > chipTop)//�������W���ォ��u���b�N�ɓ��������Ƃ�(���n���Ȃ�)
				{
					player.moveY = 0;
					player.y = chipTop + charaRadius;
					playerJumpFlg = false;
				}
				else if (chipBottom < nextTop && nextBottom < chipBottom)//�������W��������u���b�N�ɓ��������Ƃ�
				{
					player.moveY = 0;
					player.y = chipBottom - charaRadius;
				}
			}

			if(playerTop > chipBottom && playerBottom < chipTop)//�L�����ƃu���b�N�����ŏd�Ȃ��Ă��邩
			{
				if (nextLeft < chipRight && nextRight > chipRight)//�������W���E����u���b�N�ɓ��������Ƃ�
				{
					player.moveX = 0;
					player.x = chipRight + charaRadius;
				}
				else if (nextRight > chipLeft && nextLeft < chipLeft)//�������W��������u���b�N�ɓ��������Ƃ�
				{
					player.moveX = 0;
					player.x = chipLeft - charaRadius;
				}
			}
		}
	}
}

void Scene::EnemyInit()
{
	for (int i = 0; i < enemyNum; i++)
	{
		enemy[i].x = 800.0f;
		enemy[i].y = -250.0f + (i * 64);
		enemy[i].moveX = 5.0f;
		enemy[i].moveY = 0.0f;
		enemy[i].scaleX = 1.0f;
		moveCount[i] = 60;
	}
}

void Scene::EnemyUpdate()
{
	for (int i = 0; i < enemyNum; i++)
	{
		enemy[i].scaleMat = Math::Matrix::CreateScale(enemy[i].scaleX, 1, 1);				     //�g��s��
		enemy[i].transMat = Math::Matrix::CreateTranslation(enemy[i].x - scrollX, enemy[i].y, 0);//�ړ��s��
		enemy[i].mat = enemy[i].scaleMat * enemy[i].transMat;	//�s��̍���

		moveCount[i] -= 1;
		if (moveCount[i] <= 0)
		{
			enemy[i].scaleX *= -1;
			enemy[i].moveX *= -1;
			moveCount[i] = 60;
		}



		enemy[i].x += enemy[i].moveX;
		enemy[i].y += enemy[i].moveY;
	}
}

void Scene::EnemyDraw()
{
	for (int i = 0; i < enemyNum; i++)
	{
		enemy[i].color = { 1.0f,0.0f,0.0f,1.0f };
		SHADER.m_spriteShader.SetMatrix(enemy[i].mat);
		SHADER.m_spriteShader.DrawTex_Color(&charaTex, Math::Rectangle{ 0,0,64,64 }, &enemy[i].color);
	}
}

void Scene::PlayerEnemyHit()
{
	for (int i = 0; i < enemyNum; i++)
	{
		//�v���C���[
		const float playerLeft   = player.x - charaRadius;
		const float playerRight  = player.x + charaRadius;
		const float playerBottom = player.y - charaRadius;
		const float playerTop    = player.y + charaRadius;

		//�G
		const float enemyLeft   = enemy[i].x - charaRadius;
		const float enemyRight  = enemy[i].x + charaRadius;
		const float enemyBottom = enemy[i].y - charaRadius;
		const float enemyTop    = enemy[i].y + charaRadius;

		if (playerRight > enemyLeft && playerLeft < enemyRight && playerTop > enemyBottom && playerBottom < enemyTop)	//�c�Ɖ��ŏd�Ȃ��Ă��邩
		{
				player.color = { 1.0f,0.5f,0.0f,1.0f };
				break;
		}
		else
		{
			player.color = { 1.0f,1.0f,1.0f,1.0f };
		}
	}
}
