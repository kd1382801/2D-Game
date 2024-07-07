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

	//�L����
	matrix = Math::Matrix::CreateTranslation((64 * x) - 640, (y * -64) + 360, 0);
	SHADER.m_spriteShader.SetMatrix(matrix);
	SHADER.m_spriteShader.DrawTex(&charaTex, Math::Rectangle{ 0,0,64,64 }, 1.0f);

	//�s���~�܂�\��
	if (!digFlg)
	{
		SHADER.m_spriteShader.DrawString(0, 0, "�s���~�܂�I", { 1.0f,1.0f ,1.0f ,1.0f });
	}

}

void Scene::Update()
{
	//�G���^�[�L�[������������H��������

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		if (!KeyFlg)	//(KeyFlg == false) �Ɠ����Ӗ�
		{
			CreateMaze();
			//KeyFlg = true;
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
	charaTex.Load("Texture/Chara.png");

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
	charaTex.Release();
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
			mazeFlg[h][w] = 1;
		}
	}

	//�X�^�[�g�ʒu���Z�b�g	����s�E��񂶂�Ȃ��ƃ_��
	x = 1;
	y = 3;

	//�X�^�[�g�n�_�𓹂ɂ���
	mazeFlg[y][x] = 0;

	//�@��邩�t���O
	digFlg = true;

	//�@�����������N���A
	for (int i = 0; i < SAVE_MAX; i++)
	{
		save[i] = Direction::None;
	}

	//�@�����񐔂�������
	digCnt = 0;

}

void Scene::CreateMaze()
{
	dir = (Direction)(rand() % Direction::Kind);//int�^��Direction�^�ɃL���X�g
	
		switch (dir)
		{
		case Direction::Up:
			//���@���̈ʒu���}�b�v�����ǂ���
			if (y - 2 >= 1 && mazeFlg[y - 2][x] == 1)
			{
				mazeFlg[y - 1][x] = 0;	//2�}�X�@��
				mazeFlg[y - 2][x] = 0;

				y -= 2;	//�L�����̈ړ�

				//�@����������save�ɕۑ�
				save[digCnt] = Direction::Up;

				//digCnt�C���N�������g
				digCnt++;
			}
			break;
		case Direction::Down:
			if (y + 2 < (MAZE_HEIGHT - 1) && mazeFlg[y + 2][x] == 1)
			{
				mazeFlg[y + 1][x] = 0;
				mazeFlg[y + 2][x] = 0;

				y += 2;

				//�@����������save�ɕۑ�
				save[digCnt] = Direction::Down;
				digCnt++;
			}
			break;
		case Direction::Left:
			if (x - 2 >= 1 && mazeFlg[y][x - 2] == 1)
			{
				mazeFlg[y][x - 1] = 0;
				mazeFlg[y][x - 2] = 0;

				x -= 2;

				//�@����������save�ɕۑ�
				save[digCnt] = Direction::Left;
				digCnt++;
			}
			break;
		case Direction::Right:
			if (x + 2 < (MAZE_WIDTH - 1) && mazeFlg[y][x + 2] == 1)
			{
				mazeFlg[y][x + 1] = 0;
				mazeFlg[y][x + 2] = 0;

				x += 2;

				//�@����������save�ɕۑ�
				save[digCnt] = Direction::Right;
				digCnt++;
			}
			break;
		}
	
		//���̃t���[���ŏ㉺���E�̂����ꂩ��
		//�܂��@���ꍇ��digFlg��true�ɂ���
		digFlg = false;

		if (y - 2 >= 1 && mazeFlg[y - 2][x] == 1)
		{
			digFlg = true;
		}
		else if (y + 2 < (MAZE_HEIGHT - 1) && mazeFlg[y + 2][x] == 1)
		{
			digFlg = true;
		}
		else if (x - 2 >= 1 && mazeFlg[y][x - 2] == 1)
		{
			digFlg = true;
		}
		else if (x + 2 < (MAZE_WIDTH - 1) && mazeFlg[y][x + 2] == 1)
		{
			digFlg = true;
		}

		//�@��������߂鏈��
		//�@�����������Q�Ƃ��ċt������2�}�X�߂�

		//������ʒu���s���~�܂�̏ꍇ
		if (!digFlg)
		{
			//digCnt�f�N�������g
			digCnt--;
			if (digCnt < 0)
			{
				digCnt = 0;
			}
			
			//�擾���������Ɣ��΂�2�}�X�i�߂�
			switch (save[digCnt])
			{
			case Direction::Up:
				y += 2;
				break;
			case Direction::Down:
				y -= 2;
				break;
			case Direction::Left:
				x += 2;
				break;
			case Direction::Right:
				x -= 2;
				break;
			}

			//save[digCnt]���N���A����
			save[digCnt] = Direction::None;

		}

}
