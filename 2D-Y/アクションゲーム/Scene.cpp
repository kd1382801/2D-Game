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
	
	//敵描画
	EnemyDraw();

	SHADER.m_spriteShader.SetMatrix(player.mat);				//画像に行列をセット
	SHADER.m_spriteShader.DrawTex(&charaTex, Math::Rectangle{ 0,0,64,64 }, 1.0f);	//画像の描画

	Math::Matrix lineMat = DirectX::XMMatrixIdentity();//単位行列　行列の初期化
	SHADER.m_spriteShader.SetMatrix(lineMat);
	SHADER.m_spriteShader.DrawLine(0.0f, 360.0f, 0.0f, -360.0f, &Math::Color(1.0f, 0.0f, 0.0f, 1.0f));
}

void Scene::Update()
{
	player.scaleMat = Math::Matrix::CreateScale(player.scaleX, 1, 1);					//拡大行列の作成
	player.rotationMat = Math::Matrix::CreateRotationZ(ToRadians(player.rotation));		//回転行列の作成　ToRadians()でラジアンに変換
	player.transMat = Math::Matrix::CreateTranslation(player.x - scrollX, player.y, 0);	//移動行列の作成
	player.mat = player.scaleMat * player.rotationMat * player.transMat;				//行列の合成
	//行列を合成する時には「拡大→回転→移動」の順で掛け算をする
	
	//拡大：CreateScale
	//回転：CreateRotation(X,Y,Z)※基本的に2DではZしか使わない
	//移動：CreateTranslation

	player.moveY -= Gravity;//加速処理
	player.moveX = 0;//1フレーム内の移動量を取る


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

	player.x += player.moveX;//X座標の設定
	player.y += player.moveY;//Y座標の設定

	scrollX = player.x;
	if (scrollX < scrollMin)scrollX = scrollMin;
	if (scrollX > scrollMax)scrollX = scrollMax;
	//仮地面判定
	/*if (player.y < -200)
	{
		player.y = -200.0f;
		player.moveY = 0.0f;
	}*/

	//std::cout << player.x << " " << player.y << std::endl;	//標準出力にplayer.x,player.yを書き込む
	//std::cout 標準出力に要素を書き込むために使用される
	// <<		右辺の値が標準出力に書き込まれる
	//std::endl	改行文字(\n)を書き込んで改行を行い、バッファの内容をフラッシュ(未処理のデータを吐き出す)をする
	//printf("%.2f %.2f\n", player.x, player.y);←これでもok

	//敵更新
	EnemyUpdate();

	//マップの移動行列の作成
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

	// 画像の読み込み処理
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

	//敵初期化
	EnemyInit();

}

void Scene::Release()
{
	DestroyConsole();
	
	// 画像の解放処理
	charaTex.Release();
	mapTex.Release();
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

void Scene::CreateConsole()
{
	AllocConsole();//コンソールを開く
	freopen_s(&fp_c,"CONOUT$","w",stdout);//stdout:標準出力



	//freopen_s(&fp_c, "CONIN$", "r", stdin);stdin:標準入力
}

void Scene::DestroyConsole()
{
	fclose(fp_c);
	FreeConsole();//コンソールを閉じる
}

void Scene::LoadMap()
{
	printf("\nマップの初期化\n");

	for (int h = 0; h < mapHeight; h++)
	{
		for (int w = 0; w < mapWidth; w++)
		{
			mapData[h][w] = 0;
			printf("%d", mapData[h][w]);
		}
		printf("\n");
	}

	printf("\nマップの読み込み処理\n");
	FILE* fp;
	if ((fp = fopen("Texture/Map/Stage.txt", "r")) != nullptr)	//nullptr:nullポインタ
	{
		printf("\nファイルの読み込み成功\n");

		//文字読み込み処理
		char ch;
		int h = 0;	//縦のカウンタ
		int w = 0;	//横のカウンタ

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
		printf("\nファイルの読み込み失敗\n");
	}

	printf("\n読み込んだ結果\n");
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

	//未来座標
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

			if (playerRight > chipLeft && playerLeft < chipRight)//キャラとブロックが縦で重なっているか
			{
				if (nextBottom < chipTop && nextTop > chipTop)//未来座標が上からブロックに当たったとき(着地時など)
				{
					player.moveY = 0;
					player.y = chipTop + charaRadius;
					playerJumpFlg = false;
				}
				else if (chipBottom < nextTop && nextBottom < chipBottom)//未来座標が下からブロックに当たったとき
				{
					player.moveY = 0;
					player.y = chipBottom - charaRadius;
				}
			}

			if(playerTop > chipBottom && playerBottom < chipTop)//キャラとブロックが横で重なっているか
			{
				if (nextLeft < chipRight && nextRight > chipRight)//未来座標が右からブロックに当たったとき
				{
					player.moveX = 0;
					player.x = chipRight + charaRadius;
				}
				else if (nextRight > chipLeft && nextLeft < chipLeft)//未来座標が左からブロックに当たったとき
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
	enemy.x = 800.0f;
	enemy.y = -250.0f;
	enemy.moveX = 5.0f;
	enemy.moveY = 0.0f;
	enemy.scaleX = 1.0f;
	moveCount = 60;
}

void Scene::EnemyUpdate()
{
	enemy.scaleMat = Math::Matrix::CreateScale(enemy.scaleX, 1, 1);					//拡大行列
	enemy.transMat = Math::Matrix::CreateTranslation(enemy.x - scrollX, enemy.y, 0);//移動行列
	enemy.mat = enemy.scaleMat * enemy.transMat;	//行列の合成

	moveCount -= 1;
	if (moveCount <= 0)
	{
		enemy.scaleX *= -1;
		enemy.moveX *= -1;
		moveCount = 60;
	}



	enemy.x += enemy.moveX;
	enemy.y += enemy.moveY;
}

void Scene::EnemyDraw()
{
	Math::Color color{ 1.0f,0.0f,0.0f,1.0f };
	SHADER.m_spriteShader.SetMatrix(enemy.mat);
	SHADER.m_spriteShader.DrawTex_Color(&charaTex, Math::Rectangle{ 0,0,64,64 }, &color);
}
