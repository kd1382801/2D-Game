#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	//��ʃN���A
	color = { 0,0,0,1 };
	SHADER.m_spriteShader.DrawBox(0, 0, 360, 200, &color, true);

	DrawStars();
	DrawTri();

	//�~�O��
	float a, b, c = 100, deg = 30;

	//���S�_
	color = { 1,1,1,1 };
	SHADER.m_spriteShader.DrawPoint(0, 0, &color);

	c = 200;
	for (deg = 0; deg < 360; deg += 1)
	{
		a = cos(deg * 3.14f / 180) * c;
		b = sin(deg * 3.14f / 180) * c;
		color = { 0,1,0,1 };
		SHADER.m_spriteShader.DrawPoint(0 + a, 0 + b, &color);
	}

	//�ȉ~�̕`��
	c = 200;
	for (deg = 0; deg < 360; deg += 1)
	{
		a = cos(deg * 3.14f / 180) * c;
		b = sin(deg * 3.14f / 180) * c/2;
		color = { 0,1,0,1 };
		SHADER.m_spriteShader.DrawPoint(0 + a, 0 + b, &color);
	}

	//�Q���̕`��
	c = 10;
	for (deg = 0; deg < 360 * 3; deg += 1)
	{
		a = cos((deg + frame) * 3.14f / 180) * c;
		b = sin((deg + frame) * 3.14f / 180) * c;
		color = { 0,1,0,1 };
		SHADER.m_spriteShader.DrawPoint(0 + a, 0 + b, &color);

		c += 0.1f;
	}

	DrawEnemy();

	//������̓e�N�X�`���Ȃǂ�`�悵����ɏ�������
	// ������\��
	SHADER.m_spriteShader.DrawString(180, 170, "��{�}�`�R", Math::Vector4(0, 0, 0, 1));
	SHADER.m_spriteShader.DrawString(180+8, 170-5, "��{�}�`�R", Math::Vector4(1, 1, 0, frame / 180.0f));
}


void Scene::Update()
{
	UpdateStars();
	UpdateTri();
	UpdateEnemy();
	//�t���[�����𑝂₷
	frame++;

}

void Scene::Init()
{
	//�����̏�����
	srand(timeGetTime());

	InitStars();
	InitTri();
	InitEnemy();
	frame = 0;
}

void Scene::Release()
{
	
}

void Scene::InitTri()
{
	triX1 = rand() % 640 - 320;
	triY1 = rand() % 360 - 180;

	triX2 = rand() % 640 - 320;
	triY2 = rand() % 360 - 180;

	triX3 = rand() % 640 - 320;
	triY3 = rand() % 360 - 180;

	triMoveX1 = 3;
	triMoveY1 = 3;
	triMoveX2 = 3;
	triMoveY2 = 3;
	triMoveX3 = 3;
	triMoveY3 = 3;
}

void Scene::UpdateTri()
{
	//���_1
	triX1 += triMoveX1;
	triY1 += triMoveY1;
	if (triX1 > screenRight) //�E�[����
	{
		triMoveX1 *= -1;
	}
	if (triX1 < screenLeft)	 //���[����
	{
		triMoveX1 *= -1;
	}
	if (triY1 > screenTop) 	 //��[����
	{
		triMoveY1 *= -1;
	}
	if (triY1 < screenBottom)//���[����
	{
		triMoveY1 *= -1;
	}
	
	//���_2
	triX2 += triMoveX2;
	triY2 += triMoveY2;

	if (triX2 > screenRight) //�E�[����
	{
		triMoveX2 *= -1;
	}
	if (triX2 < screenLeft)	 //���[����
	{
		triMoveX2 *= -1;
	}
	if (triY2 > screenTop) 	 //��[����
	{
		triMoveY2 *= -1;
	}
	if (triY2 < screenBottom)//���[����
	{
		triMoveY2 *= -1;
	}
	
	//���_3
	triX3 += triMoveX3;
	triY3 += triMoveY3;

	if (triX3 > screenRight) //�E�[����
	{
		triMoveX3 *= -1;
	}
	if (triX3 < screenLeft)	 //���[����
	{
		triMoveX3 *= -1;
	}
	if (triY3 > screenTop) 	 //��[����
	{
		triMoveY3 *= -1;
	}
	if (triY3 < screenBottom)//���[����
	{
		triMoveY3 *= -1;
	}
	
	
	
}

void Scene::DrawTri()
{
	color = { 0,1,0,0.2f };
	SHADER.m_spriteShader.DrawTriangle(triX1, triY1, triX2, triY2, triX3, triY3, &color, true);
}

void Scene::InitEnemy()
{
	enemyCx = 0;
	enemyCy = 0;
	enemyRadius = 100;
	enemyDeg = 90;
}

void Scene::UpdateEnemy()
{
	//enemyDeg += 5; //�����v���
	enemyDeg -= 5; //���v���

	//����
	//enemyCx += 0.2f;

	//�Q��
	//enemyRadius -= 0.1f;
}

void Scene::DrawEnemy()
{
	float a, b;
	//a = cos(enemyDeg * 3.14f / 180) * enemyRadius;
	a = cos(enemyDeg * 3.14f / 180) * enemyRadius * 2; //�ȉ~�̓���
	b = sin(enemyDeg * 3.14f / 180) * enemyRadius;

	color = { 1,0,1,1 };
	SHADER.m_spriteShader.DrawCircle(enemyCx + a, enemyCy + b, 15, &color, true);
}

//��������
void Scene::InitStars()
{
	for (int i = 0; i < starNum; i++) 
	{
		starX[i] = rand() % 640 - 320;
		starY[i] = rand() % 360 - 180;
	}
}

//���X�V
void Scene::UpdateStars()
{
	//���𓮂���
	for (int i = 0; i < starNum; i++)
	{
		//if (i < starNum / 2){starX[i] -= 1;}		if(i % 2 == 0)�ł�ok
		//else {starX[i] += 1;}

		starY[i] -= 1;

		//��ʉ��[�`�F�b�N
		if (starY[i] < screenBottom)
		{
			starY[i] = screenTop + 1;
		}
	}
}

//���`��
void Scene::DrawStars()
{
	for (int i = 0; i < starNum; i++)
	{
		color = {0.5f + rand() / 32767.0f,0.5f + rand() / 32767.0f,0.5f + rand() / 32767.0f,1};
		SHADER.m_spriteShader.DrawPoint(starX[i]  ,starY[i]  , &color);
		SHADER.m_spriteShader.DrawPoint(starX[i]+1,starY[i]  , &color);
		SHADER.m_spriteShader.DrawPoint(starX[i]  ,starY[i]+1, &color);
		SHADER.m_spriteShader.DrawPoint(starX[i]+1,starY[i]+1, &color);
		//SHADER.m_spriteShader.DrawCircle(starX[i],starY[i],3, &color);
	}
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
