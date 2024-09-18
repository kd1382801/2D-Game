#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	////背景　合成なし
	//D3D.SetBlendState(BlendMode::NoBlend);	//色のブレンド方法の設定 NoBlend:画像をそのまま表示する(透明度も変わらない)

	////背景　半透明合成
	D3D.SetBlendState(BlendMode::Alpha);	//色のブレンド方法の設定 Alpha:半透明合成

	//背景
	SHADER.m_spriteShader.SetMatrix(backMat);
	SHADER.m_spriteShader.DrawTex(&backTex, Math::Rectangle{ 0,0,1280,720 }, 0.5f);

	if (addFlg) {
		////太陽　加算合成
		D3D.SetBlendState(BlendMode::Add);	//色のブレンド方法の設定 Add:加算合成
	}

	//太陽
	for (int s = 0; s < sunNum; s++) {
		if (sun[s].flg) {
			SHADER.m_spriteShader.SetMatrix(sun[s].mat);
			SHADER.m_spriteShader.DrawTex(&sunTex, Math::Rectangle{ 0,0,128,128 }, 1.0f);
		}
	}
}

void Scene::Update()
{
	//マウス座標取得
	GetCursorPos(&mousePos);
	ScreenToClient(APP.m_window.GetWndHandle(), &mousePos);
	mousePos.x -= 640;
	mousePos.y -= 360;
	mousePos.y *= -1;

	//ブレンドモード切替
	if (GetAsyncKeyState('A') & 0x8000) {
		addFlg = true;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		addFlg = false;
	}

	//表示数制御
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		sunNum++;
		if (sunNum >= SUN_MAX) {
			sunNum = SUN_MAX;
		}
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		sunNum--;
		if (sunNum <= 1) {
			sunNum = 1;
		}
	}

	//モード切り換え
	if (GetAsyncKeyState('Z') & 0x8000) {
		mode = 1;
		SunInit();
	}
	if (GetAsyncKeyState('X') & 0x8000) {
		mode = 2;
		SunInit();
	}
	if (GetAsyncKeyState('C') & 0x8000) {
		mode = 3;
		SunInit();
	}

	//==============================
	// 追尾モード
	//==============================
	if (mode == 1) {
		float dec = 0;	//減少量

		//speedが最大30、最小3になるような減少量を求める（最大値　-　最小値）/（要素数　-　1）
		if (sunNum > 1) {
			dec = (float)27 / (sunNum - 1);
		}

		for (int s = 0; s < sunNum; s++) {
			//後ろの太陽ほどspeedの値を小さくする
			//sun[s].speed = 30 - (s * 1);
			sun[s].speed = 30 - (s * dec);

			//移動量（マウスに向かわせる処理）
			float angle;
			if (s == 0) {
				angle = atan2(mousePos.y - sun[s].y, mousePos.x - sun[s].x);
			}
			else {
				angle = atan2(sun[s - 1].y - sun[s].y, sun[s - 1].x - sun[s].x);
			}

			sun[s].moveX = cos(angle) * sun[s].speed;
			sun[s].moveY = sin(angle) * sun[s].speed;

			sun[s].x += sun[s].moveX;
			sun[s].y += sun[s].moveY;
		}
	}

	//==============================
	// スマホゲーエフェクト風
	//==============================
	if (mode == 2) {
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
			//flgがfalseの太陽はtrueに(1つだけ)
			for (int s = 0; s < sunNum; s++) {
				if (!sun[s].flg) {
					sun[s].flg = true;
					sun[s].x = mousePos.x;
					sun[s].y = mousePos.y;
					sun[s].moveX = rand() % 2 * 2 - 1;
					sun[s].moveY = rand() % 2 * 2 - 1;
					sun[s].timer = rand() % (30 + 1) + 10;
					break;
				}
			}
			//flgがtrueの太陽は生存時間が0になるまで移動量を加算する
			for (int s = 0; s < sunNum; s++) {
				if (sun[s].flg) {
					//タイマーをデクリメント
					sun[s].timer--;
					if (sun[s].timer <= 0) {
						sun[s].flg = false;
					}
					//座標に移動量を加算
					sun[s].x += sun[s].moveX;
					sun[s].y += sun[s].moveY;
				}
			}
		}
		else {
			for (int s = 0; s < sunNum; s++) {
				sun[s].flg = false;
			}
		}
	}


	for (int s = 0; s < sunNum; s++) {
		sun[s].mat = Math::Matrix::CreateTranslation(sun[s].x, sun[s].y, 0);
	}
	
	backMat = Math::Matrix::CreateTranslation(0, 0, 0);
}

void Scene::SunInit()
{
	//太陽
	for (int s = 0; s < SUN_MAX; s++) {
		sun[s].x = 0;
		sun[s].y = 0;
		sun[s].moveX = 0; 
		sun[s].moveY = 0;
		sun[s].speed = 0;	
		sun[s].flg = true;	
		sun[s].timer = 0;	
	}
	//太陽の初期数
	sunNum = 20;
}

void Scene::Init()
{
	srand(time(0));

	// 画像の読み込み処理
	backTex.Load("Texture/back.png");
	sunTex.Load("Texture/Sun.png");

	SunInit();

	mode = 1;
}

void Scene::Release()
{
	// 画像の解放処理
	backTex.Release();
	sunTex.Release();
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
