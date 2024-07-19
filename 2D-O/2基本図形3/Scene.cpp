#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	//画面クリア
	color = { 0,0,0,1 };
	SHADER.m_spriteShader.DrawBox(0, 0, 360, 200, &color, true);

	if (GetAsyncKeyState('C') & 0x8000) {
		for (int i = 0; i < 50; i++) {
			color = { rand() / 32767.0f,rand() / 32767.0f,rand() / 32767.0f,1 };
			DrawCircleEx(rand() % 640 - 320, rand() % 360 - 180, rand() % 100, rand() % 100, &color);
		}
	}
	
	color = { 0,1,0,1 };
	DrawPolygon(0, 0, 100, 100, frame / 60 + 1, 90, &color, false); 

	color = { 1,1,0,1 };
	DrawPolygon(200, 50, size, size, 6, degree, &color, false);
	DrawPolygon(200, 50, size, size);

	DrawFireworks(-200,50);

	//文字列はテクスチャなどを描画した後に書くこと
	// 文字列表示
	SHADER.m_spriteShader.DrawString(180, 170, "基本図形３", Math::Vector4(0, 0, 0, 1));
	SHADER.m_spriteShader.DrawString(180+8, 170-5, "基本図形３", Math::Vector4(1, 1, 0, frame / 180.0f));
}



void Scene::Update()
{
	//上下キーで拡大縮小
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		size += 1;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		size -= 1;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		degree += 2;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		degree -= 2;
	}
	


	//フレーム数を増やす
	frame++;
}

void Scene::Init()
{
	//乱数の初期化
	srand(timeGetTime());

	size = 20;
	degree = 90;
	frame = 0;
}

void Scene::Release()
{
	
}

void Scene::DrawCircleEx(float cx, float cy, float radiusX, float radiusY, Math::Color* pColor)
{
	//円軌道
	float a, b, deg;

	
	for (deg = 0; deg < 360; deg += 1)
	{
		a = cos(deg * 3.14f / 180) * radiusX;
		b = sin(deg * 3.14f / 180) * radiusY;
		SHADER.m_spriteShader.DrawPoint(cx + a, cy + b, pColor);
	}

}

void Scene::DrawPolygon(float cx, float cy, float radiusX, float radiusY, float vertexNum,float startDeg, Math::Color* pColor,bool paintFlg)
{
	//円軌道
	float a, b, deg;				//現在の点の情報
	float aNext, bNext, degNext;	//次の点の情報
	float intervalDeg;				//角度の間隔
	intervalDeg = 360 / vertexNum;	//頂点数で割る

	for (deg = startDeg; deg < 360 + startDeg; deg += intervalDeg)
	{
		//現在の点
		a = cos(DirectX::XMConvertToRadians(deg)) * radiusX;
		b = sin(DirectX::XMConvertToRadians(deg)) * radiusY;
		//SHADER.m_spriteShader.DrawPoint(cx + a, cy + b, pColor);
		//SHADER.m_spriteShader.DrawCircle(cx + a, cy + b, 10, pColor, false);

		//次の点
		degNext = deg + intervalDeg;
		aNext = cos(DirectX::XMConvertToRadians(degNext)) * radiusX;
		bNext = sin(DirectX::XMConvertToRadians(degNext)) * radiusY;
		//SHADER.m_spriteShader.DrawPoint(cx + aNext, cy + bNext, pColor);
		//SHADER.m_spriteShader.DrawCircle(cx + aNext, cy + bNext, 20, pColor, false);

		if (!paintFlg) {
			//現在の点と次の点を線で結ぶ
			SHADER.m_spriteShader.DrawLine(cx + a, cy + b, cx + aNext, cy + bNext, pColor);
		}
		else {
			//塗りつぶしの場合
			SHADER.m_spriteShader.DrawTriangle(cx, cy, cx + a, cy + b, cx + aNext, cy + bNext, pColor, true);
		}
	}

}

void Scene::DrawFireworks(float cx, float cy)
{
	float a, b, c = 100, deg;

	for (c = 0; c < 100; c += 15) {
		color = { rand() / 32767.0f,rand() / 32767.0f,rand() / 32767.0f,1};
		for (deg = 0; deg < 360; deg += 10)//円1周分のループ
		{
			a = cos(deg * 3.14f / 180) * c;
			b = sin(deg * 3.14f / 180) * c;
			SHADER.m_spriteShader.DrawCircle(cx + a, cy + b, 3, &color);
		}
	}
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
