#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	//現在のシーンの描画処理を実行
	switch (nowScene)
	{
	case Title:
		//↑タグ名省略可(同じ名前のものがある場合は Scene:: が必要)
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
	//現在のシーンの描画処理を実行
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
	//ゲーム開始最初のシーン:タイトル
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

	// デバッグウィンドウ
	if (ImGui::Begin("Debug Window"))
	{
		ImGui::Text("FPS : %d", APP.m_fps);
	}
	ImGui::End();
}

void Scene::DrawTitle()
{
	// 文字列表示
	SHADER.m_spriteShader.DrawString(titleX, titleY, "タイトル画面", Math::Vector4(1, 1, 0, 1));
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
	SHADER.m_spriteShader.DrawString(gameX, gameY, "ゲーム画面", Math::Vector4(1, 1, 1, 1));
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
	SHADER.m_spriteShader.DrawString(ResultX, ResultY, "リザルト画面", Math::Vector4(1, 0, 0, 1));
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
