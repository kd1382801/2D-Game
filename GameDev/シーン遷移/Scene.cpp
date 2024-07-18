#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	//���݂̃V�[���̕`�揈�������s
	switch (nowScene)
	{
	case Title:
		//���^�O���ȗ���(�������O�̂��̂�����ꍇ�� Scene:: ���K�v)
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
	//���݂̃V�[���̕`�揈�������s
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
	//�Q�[���J�n�ŏ��̃V�[��:�^�C�g��
	nowScene = SceneType::Title;

	titleX = 0;
	titleY = 0;

	gameX = 0;
	gameY = 0;

	ResultX = 0;
	ResultY = 0;

	KeyFlg = false;
}

void Scene::Release()
{
	
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

void Scene::DrawTitle()
{
	// ������\��
	SHADER.m_spriteShader.DrawString(titleX, titleY, "�^�C�g�����", Math::Vector4(1, 1, 0, 1));
}

void Scene::UpdateTitle()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		titleY += 15.0f;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		titleY -= 15.0f;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		titleX -= 15.0f;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		titleX += 15.0f;
	}
	ChangeScene();
}

void Scene::DrawGame()
{
	SHADER.m_spriteShader.DrawString(gameX, gameY, "�Q�[�����", Math::Vector4(1, 1, 1, 1));
}

void Scene::UpdateGame()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		gameY += 15.0f;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		gameY -= 15.0f;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		gameX -= 15.0f;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		gameX += 15.0f;
	}
	ChangeScene();
}

void Scene::DrawResult()
{
	SHADER.m_spriteShader.DrawString(ResultX, ResultY, "���U���g���", Math::Vector4(1, 0, 0, 1));
}

void Scene::UpdateResult()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		ResultY += 15.0f;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		ResultY -= 15.0f;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		ResultX -= 15.0f;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		ResultX += 15.0f;
	}
	ChangeScene();
}

void Scene::ChangeScene()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		if (!KeyFlg)
		{
			nowScene = SceneType::Game;
			KeyFlg = true;
		}
	}
	else
	{
		KeyFlg = false;
	}
}
