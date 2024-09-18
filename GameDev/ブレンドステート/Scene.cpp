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
		SHADER.m_spriteShader.SetMatrix(sun[s].mat);
		SHADER.m_spriteShader.DrawTex(&sunTex, Math::Rectangle{ 0,0,128,128 }, 1.0f);
	}
}

void Scene::Update()
{
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

	for (int s = 0; s < sunNum; s++) {
		sun[s].x += sun[s].moveX;
		sun[s].y += sun[s].moveY;

		//画面端判定
		if (sun[s].x <=  (-640 + 64)) {
			sun[s].x = (-640 + 64);
			sun[s].moveX *= -1;
		}
		if (sun[s].x >= (640 - 64)) {
			sun[s].x = (640 - 64);
			sun[s].moveX *= -1;
		}
		if (sun[s].y <= (-360 + 64)) {
			sun[s].y = (-360 + 64);
			sun[s].moveY *= -1;
		}
		if (sun[s].y >= (360 - 64)) {
			sun[s].y = (360 - 64);
			sun[s].moveY *= -1;
		}
		
		sun[s].mat = Math::Matrix::CreateTranslation(sun[s].x, sun[s].y, 0);
	}

	backMat = Math::Matrix::CreateTranslation(0, 0, 0);
}

void Scene::Init()
{
	srand(time(0));

	// 画像の読み込み処理
	backTex.Load("Texture/back.png");
	sunTex.Load("Texture/Sun.png");

	//太陽
	for (int s = 0; s < SUN_MAX; s++) {
		sun[s].x = rand() % (1280 - 128 + 1) - (640 - 64);
		sun[s].y = rand() % (720 - 128 + 1) - (360 - 64);
		sun[s].moveX = rand() % 2 * 10 - 5;	//2のあまりで0か1 → 0*10=0 か 1*10=10 
		sun[s].moveY = rand() % 2 * 10 - 5;	//→0-5=-5 か 10-5=5しかでない
	}
	//太陽の初期数
	sunNum = 1;
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
