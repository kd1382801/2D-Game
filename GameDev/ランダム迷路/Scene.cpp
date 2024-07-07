#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	//ブロック表示
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
	//エンターキーを押したら迷路生成処理
	
	if (GetAsyncKeyState(VK_RETURN) & 0x8000) 
	{
		if (!KeyFlg)	//(KeyFlg == false) と同じ意味
		{
			KeyFlg = true;
			CreateMaze();
		}
	}
	else 
	{
		KeyFlg = false;
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		//迷路データの初期化
		InitMaze();
	}

}

void Scene::Init()
{
	//乱数の初期化
	srand(time(0));
	
	// 画像の読み込み処理
	blockTex.Load("Texture/block.png");	//Textureフォルダにある"block.png"

	//InitMaze()の呼び出し
	InitMaze();

	//キーフラグの初期化
	KeyFlg = false;
}

void Scene::Release()
{
	// 画像の解放処理
	blockTex.Release();
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

void Scene::InitMaze()
{
	//関数定義：関数の中身の処理
	//プロトタイプ宣言は Scene.h でする

	//mazeFlgの初期値
	for (int h = 0; h < MAZE_HEIGHT; h++)
	{
		for (int w = 0; w < MAZE_WIDTH; w++)
		{
			//四隅の行と列だけFlg=1
			if (h == 0 || h == MAZE_HEIGHT - 1 || w == 0 || w == MAZE_WIDTH - 1)
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
	//四隅以外のブロックが50%の確率でFlg=1になる処理
	for (int h = 1; h < MAZE_HEIGHT - 1; h++)
	{
		for (int w = 1; w < MAZE_WIDTH - 1; w++)
		{
			if (rand() % 2 == 1)
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
