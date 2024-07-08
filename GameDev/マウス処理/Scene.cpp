#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	//キャラ表示
	SHADER.m_spriteShader.SetMatrix(charaMat);
	SHADER.m_spriteShader.DrawTex(&charaTex, Math::Rectangle{ 0,0,64,64 }, 1.0f);
	

}

void Scene::Update()
{
	////マウス座標取得
	//GetCursorPos(&mouse);	//ディスプレイ上のマウス座標を取得(ディスプレイの左上が(0,0))
	//
	////指定のウィンドウ基準のマウス座標に変換(ウィンドウの左上が(0,0))
	//ScreenToClient(APP.m_window.GetWndHandle(), &mouse);
	//
	////マウスの座標系を実行ウィンドウの座標系(中心が(0,0))に補正
	//mouse.x -= 640;
	//mouse.y -= 360;
	//mouse.y *= -1;

	//左クリックでカーソル復活
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		ShowCursor(true);
	}

	//mouse = GetMousePos();
	GetMousePos2(&mouse);

	//キャラの座標を更新
	charaX = mouse.x;
	charaY = mouse.y;

	charaMat = Math::Matrix::CreateTranslation(charaX, charaY, 0);
}

void Scene::Init()
{
	//カーソルを消す(false:非表示 true:表示)
	ShowCursor(false);

	// 画像の読み込み処理
	charaTex.Load("Texture/player.png");
	charaX = 0;
	charaY = 0;

	//マウス
	mouse.x = 0;
	mouse.y = 0;

}

void Scene::Release()
{
	// 画像の解放処理
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

POINT Scene::GetMousePos()
{
	POINT mousePos;

	GetCursorPos(&mousePos);
	ScreenToClient(APP.m_window.GetWndHandle(), &mousePos);
	mousePos.x -= 640;
	mousePos.y -= 360;
	mousePos.y *= -1;
	return mousePos;
}

void Scene::GetMousePos2(POINT* mousePos)
{
	GetCursorPos(mousePos);
	ScreenToClient(APP.m_window.GetWndHandle(), mousePos);
	mousePos->x -= 640;
	mousePos->y -= 360;
	mousePos->y *= -1;
}
