#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	//��ʃN���A
	color = { 0.0f,0.5f,0.5f,1.0f };//RGBA
	SHADER.m_spriteShader.DrawBox(0, 0, 320, 180, &color, true);
	
	//�l�p�`
	color = { 0.9f,0.5f,0.0f,1.0f };//RGBA
	//SHADER.m_spriteShader.DrawBox(0, 0, 20, 10, &color, true);

	//�~
	color = { 1.0f,1.0f,0.0f,1.0f };
	//SHADER.m_spriteShader.DrawCircle(playerX, playerY, 50, &color, true);

	//�_
	color = { 0.0f,1.0f,1.0f,1.0f };
	//SHADER.m_spriteShader.DrawPoint(0, 0, &color);
	//SHADER.m_spriteShader.DrawPoint(1, 0, &color);
	//SHADER.m_spriteShader.DrawPoint(0, 1, &color);
	//SHADER.m_spriteShader.DrawPoint(1, 1, &color);

	//��������
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

	//���@�̕`��
	DrawPlayer();

	//�G�̕`��
	DrawEnemy();

	//�G�t�F�N�g�̕`��
	DrawCircleEffect();


	//������͐}�`�Ȃǂ�`�悵����ɏ�������
	// ������\��
	SHADER.m_spriteShader.DrawString(100, 170, "��{�}�`�P", Math::Vector4(0, 0, 0, 1));
	SHADER.m_spriteShader.DrawString(100 + 8, 170 - 5, "��{�}�`�P", Math::Vector4(1, 1, 0, frame / 180.0f));
	
}

void Scene::Update()
{
	
	UpdatePlayer();
	
	UpdateEnemy();

	UpdateCircleEffect();

	//�t���[�����𑝂₷
	frame++;

	//gameStep�̐؂�ւ�
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
	//�����̏�����
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


//���@������
void Scene::InitPlayer()
{
	//���W�̏�����
	playerX = 0;
	playerY = -150;
}

//���@�X�V
void Scene::UpdatePlayer()
{
	//���W�X�V
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

//���@�`��
void Scene::DrawPlayer()
{
	//���@����
	color = { 1.0f,1.0f,0.0f,1.0f };
	SHADER.m_spriteShader.DrawCircle(playerX, playerY, 20, &color, true);
	color = { 0.0f,0.0f,0.0f,1.0f };
	SHADER.m_spriteShader.DrawBox(playerX - 6, playerY + 3, 2, 8, &color, true);
	SHADER.m_spriteShader.DrawBox(playerX + 6, playerY + 3, 2, 8, &color, true);
	SHADER.m_spriteShader.DrawBox(playerX , playerY - 12, 6, 1, &color, true);
}

//�G������
void Scene::InitEnemy()
{
	enemyX = 0;
	enemyY = 100;
	enemyMoveX = 3;
	enemyMoveY = 3;
}

//�G�X�V
void Scene::UpdateEnemy()
{
	if (gameStep == 1)
	{
		//���o�E���h
		
		//���W�X�V
		enemyX += enemyMoveX;

		//�E�[����
		if (enemyX > screenRight - 16)
		{
			//enemyMoveX = -3;
			enemyMoveX *= -1;	//�������]
		}

		//���[����
		if (enemyX < screenLeft + 16)
		{
			//enemyMoveX = 3;
			enemyMoveX *= -1;
		}
	}
	else if (gameStep == 2)
	{
		//�c�o�E���h
		
		enemyY += enemyMoveY;

		//����
		//��[����
		if (enemyY > screenTop - 16)
		{
			enemyMoveY *= -1;
		}

		//���[����
		if (enemyY < screenBottom + 16)
		{
			enemyMoveY *= -1;
		}
	}
	else if (gameStep == 3)
	{
		//�΂߃o�E���h

		enemyX += enemyMoveX;
		enemyY += enemyMoveY;

		//����
		//�E�[����
		if (enemyX > screenRight - 16)
		{
			enemyMoveX *= -1;	//�������]
		}

		//���[����
		if (enemyX < screenLeft + 16)
		{
			enemyMoveX *= -1;
		}
		//��[����
		if (enemyY > screenTop - 16)
		{
			enemyMoveY *= -1;
		}

		//���[����
		if (enemyY < screenBottom + 16)
		{
			enemyMoveY *= -1;
		}
	}
}

//�G�`��
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

//�G�t�F�N�g������
void Scene::InitCircleEffect()
{
	circleX = 0;
	circleY = 0;
	circleRadius = 10;
	circleColor = { 1,1,1,1 };
	circleFlg = false;
}

//�G�t�F�N�g�X�V
void Scene::UpdateCircleEffect()
{
	//�X�y�[�X�L�[�ŃG�t�F�N�g����
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

//�G�t�F�N�g�`��
void Scene::DrawCircleEffect()
{
	if (circleFlg)
	{
		SHADER.m_spriteShader.DrawCircle(circleX, circleY, circleRadius, &circleColor, false);
		SHADER.m_spriteShader.DrawCircle(circleX, circleY, circleRadius - 10, &circleColor, false);
	}
}



//ctrl + mm�ł����߂�
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