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

	//キャラ
	matrix = Math::Matrix::CreateTranslation((64 * x) - 640, (y * -64) + 360, 0);
	SHADER.m_spriteShader.SetMatrix(matrix);
	SHADER.m_spriteShader.DrawTex(&charaTex, Math::Rectangle{ 0,0,64,64 }, 1.0f);

	//行き止まり表示
	if (!digFlg)
	{
		SHADER.m_spriteShader.DrawString(0, 0, "行き止まり！", { 1.0f,1.0f ,1.0f ,1.0f });
	}

}

void Scene::Update()
{
	//エンターキーを押したら迷路生成処理

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		if (!KeyFlg)	//(KeyFlg == false) と同じ意味
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
	charaTex.Load("Texture/Chara.png");

	//InitMaze()の呼び出し
	InitMaze();

	//キーフラグの初期化
	KeyFlg = false;

	//方向
	dir = Direction::Up;

}

void Scene::Release()
{
	// 画像の解放処理
	blockTex.Release();
	charaTex.Release();
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
			mazeFlg[h][w] = 1;
		}
	}

	//スタート位置をセット	※奇数行・奇数列じゃないとダメ
	x = 1;
	y = 3;

	//スタート地点を道にする
	mazeFlg[y][x] = 0;

	//掘れるかフラグ
	digFlg = true;

	//掘った履歴をクリア
	for (int i = 0; i < SAVE_MAX; i++)
	{
		save[i] = Direction::None;
	}

	//掘った回数を初期化
	digCnt = 0;

}

void Scene::CreateMaze()
{
	dir = (Direction)(rand() % Direction::Kind);//int型→Direction型にキャスト
	
		switch (dir)
		{
		case Direction::Up:
			//穴掘り後の位置がマップ内かどうか
			if (y - 2 >= 1 && mazeFlg[y - 2][x] == 1)
			{
				mazeFlg[y - 1][x] = 0;	//2マス掘る
				mazeFlg[y - 2][x] = 0;

				y -= 2;	//キャラの移動

				//掘った方向をsaveに保存
				save[digCnt] = Direction::Up;

				//digCntインクリメント
				digCnt++;
			}
			break;
		case Direction::Down:
			if (y + 2 < (MAZE_HEIGHT - 1) && mazeFlg[y + 2][x] == 1)
			{
				mazeFlg[y + 1][x] = 0;
				mazeFlg[y + 2][x] = 0;

				y += 2;

				//掘った方向をsaveに保存
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

				//掘った方向をsaveに保存
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

				//掘った方向をsaveに保存
				save[digCnt] = Direction::Right;
				digCnt++;
			}
			break;
		}
	
		//次のフレームで上下左右のいずれかが
		//まだ掘れる場合はdigFlgをtrueにする
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

		//掘った道を戻る処理
		//掘った履歴を参照して逆方向へ2マス戻る

		//今いる位置が行き止まりの場合
		if (!digFlg)
		{
			//digCntデクリメント
			digCnt--;
			if (digCnt < 0)
			{
				digCnt = 0;
			}
			
			//取得した方向と反対に2マス進める
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

			//save[digCnt]をクリアする
			save[digCnt] = Direction::None;

		}

}
