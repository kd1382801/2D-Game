#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	//�u���b�N�\��
	for (int h = 0; h < MAZE_HEIGHT; h++)
	{
		for (int w = 0; w < MAZE_WIDTH; w++)
		{
			if (mazeFlg[h][w] == 1)
			{
				matrix = Math::Matrix::CreateTranslation((w * 64) - 640, (h * -64) + 360, 0);
				SHADER.m_spriteShader.SetMatrix(matrix);
				SHADER.m_spriteShader.DrawTex(&blockTex, Math::Rectangle{ 0,0,64,64 }, 1.0f);
			}
		}
	}

}

void Scene::Update()
{
	//�G���^�[�L�[������������H��������

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		if (!KeyFlg)	//(KeyFlg == false) �Ɠ����Ӗ�
		{
			InitMaze();
			CreateMaze();
			KeyFlg = true;
		}
	}
	else
	{
		KeyFlg = false;
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		//���H�f�[�^�̏�����
		InitMaze();
	}

}

void Scene::Init()
{
	//�����̏�����
	srand(time(0));

	// �摜�̓ǂݍ��ݏ���
	blockTex.Load("Texture/block.png");	//Texture�t�H���_�ɂ���"block.png"

	//InitMaze()�̌Ăяo��
	InitMaze();

	//�L�[�t���O�̏�����
	KeyFlg = false;

	//����
	dir = Direction::Up;

}

void Scene::Release()
{
	// �摜�̉������
	blockTex.Release();
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

void Scene::InitMaze()
{
	//�֐���`�F�֐��̒��g�̏���
	//�v���g�^�C�v�錾�� Scene.h �ł���

	//mazeFlg�̏����l
	for (int h = 0; h < MAZE_HEIGHT; h++)
	{
		for (int w = 0; w < MAZE_WIDTH; w++)
		{
			//�l���̍s�Ɨ񂾂�Flg=1
			if (h == 0 || h == MAZE_HEIGHT - 1 || w == 0 || w == MAZE_WIDTH - 1)
			{
				mazeFlg[h][w] = 1;
			}
			//�����s��������̃u���b�N�̕\��
			else if((h % 2 == 0) && (w % 2 == 0)) 
			{
				mazeFlg[h][w] = 1;
			}
			else
			{
				mazeFlg[h][w] = 0;
			}
		}
	}
}

void Scene::CreateMaze()
{
	for (int h = 2; h < (MAZE_HEIGHT - 2); h += 2)
	{
		for (int w = 2; w < (MAZE_WIDTH - 2); w += 2)
		{
			dir = (Direction)(rand() % Direction::Kind);//int�^��Direction�^�ɃL���X�g

			switch (dir)
			{
			case Direction::Up:
				mazeFlg[h - 1][w] = 1;
				break;
			case Direction::Down:
				mazeFlg[h + 1][w] = 1;
				break;
			case Direction::Left:
				mazeFlg[h][w - 1] = 1;
				break;
			case Direction::Right:
				mazeFlg[h][w + 1] = 1;
				break;
			}
		}
	}
}
