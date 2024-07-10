#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	//キャラ表示
	SHADER.m_spriteShader.SetMatrix(charaMat);
	SHADER.m_spriteShader.DrawTex(&charaTex, Math::Rectangle{ 0,0,64,64 }, 1.0f);
	
	//敵表示
	SHADER.m_spriteShader.SetMatrix(enemyMat);
	SHADER.m_spriteShader.DrawTex(&enemyTex, Math::Rectangle{ 0,0,64,64 }, 1.0f);

}

void Scene::Update()
{
	//マウス座標を取得
	mouse = GetMousePos();


	//左クリック
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (clickFlg == false)
		{
			clickFlg = true;
			ShowCursor(true);

			//クリックした瞬間のマウス座標を保存
			clickPos = mouse;
			system("cls");//画面クリア
			printf("clickPos.x = %d\nclickPos.y = %d\n\n", clickPos.x, clickPos.y);
		}
		
	}
	else 
	{
		//クリックされてる状態から離されたら
		if (clickFlg) 
		{
			//離した瞬間のマウス座標を保存
			releasePos = mouse;
			printf("releasePos.x = %d\nreleasePos.y = %d\n\n", releasePos.x, releasePos.y);


			//アークタンジェントを使用して角度を求める  ※atan2(高さ, 底辺)
			angle = atan2(clickPos.y - releasePos.y, clickPos.x - releasePos.x);

			//算出した角度に対して各軸の移動量を確定
			moveX = cos(angle) * speed;	//cosで求めた方向の値*速度
			moveY = sin(angle) * speed;	//sinで求めた方向の値*速度
		}
		clickFlg = false;
	}


	//敵とキャラの当たり判定(円判定)
	float a = enemyX - charaX;
	float b = enemyY - charaY;
	float c = sqrt(a * a + b * b);
	if (c < 64)
	{
		//ランダムな位置に再出現
		enemyX = rand() % (WIDTH - 64) + 1 - (WIDTH / 2 - 32);
		enemyY = rand() % (HEIGHT - 64) + 1 - (HEIGHT / 2 - 32);

	}

	//キャラ座標更新
	charaX += moveX;
	charaY += moveY;

	charaMat = Math::Matrix::CreateTranslation(charaX, charaY, 0);
	enemyMat = Math::Matrix::CreateTranslation(enemyX, enemyY, 0);
}

void Scene::Init()
{
	//コンソール
	AllocConsole();
	freopen_s(&fp_c, "CONOUT$", "w", stdout);

	srand(time(0));
	//カーソルを消す(false:非表示 true:表示)
	ShowCursor(false);

	// 画像の読み込み処理
	charaTex.Load("Texture/Chara.png");
	enemyTex.Load("Texture/enemy.png");

	//キャラ
	charaX = 0;
	charaY = 0;
	moveX = 0;
	moveY = 0;
	angle = 0;
	speed = 2;

	//敵
	enemyX = 100;
	enemyY = 200;

	//マウス
	mouse.x = 0;
	mouse.y = 0;
	clickPos = { 0,0 };//{x, y}
	releasePos = { 0,0 };
	clickFlg = false;

}

void Scene::Release()
{
	// 画像の解放処理
	charaTex.Release();
	enemyTex.Release();
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

POINT Scene::GetMousePos()
{
	POINT mousePos;

	GetCursorPos(&mousePos);
	ScreenToClient(APP.m_window.GetWndHandle(), &mousePos);
	mousePos.x -= WIDTH / 2;
	mousePos.y -= HEIGHT / 2;
	mousePos.y *= -1;
	return mousePos;
}

void Scene::GetMousePos2(POINT* mousePos)
{
	GetCursorPos(mousePos);
	ScreenToClient(APP.m_window.GetWndHandle(), mousePos);
	mousePos->x -= WIDTH / 2;
	mousePos->y -= HEIGHT / 2;
	mousePos->y *= -1;
}
