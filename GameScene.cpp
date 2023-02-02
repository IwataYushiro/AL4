#include "GameScene.h"
#include <cassert>
#include "Collision.h"
#include <sstream>
#include <iomanip>

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete spriteBG;
	delete objSphere;
	delete objGround;
	delete objRay;

	delete modelSphere;
	delete modelGround;
	delete modelRay;

	//前景スプライト解放
	delete sprite1;
	delete sprite2;
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->input = input;

	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/background.png");
	//テクスチャ2番に読み込み
	Sprite::LoadTexture(2, L"Resources/texture.png");

	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	// 3Dオブジェクト生成
	modelSphere = Model::LoadFromOBJ("sphere");
	modelGround = Model::LoadFromOBJ("ground");
	modelRay = Model::LoadFromOBJ("ray");

	objSphere = Object3d::Create();
	objGround = Object3d::Create();
	objRay = Object3d::Create();

	objSphere->SetModel(modelSphere);
	objGround->SetModel(modelGround);
	objRay->SetModel(modelRay);

	//前景スプライト生成
	//座標{0,0}にテクスチャ2番のスプライトを生成
	sprite1 = Sprite::Create(2, { 0.0f,0.0f });
	//座標{500,500}にテクスチャ2番のスプライトを生成
	sprite2 = Sprite::Create(2, { 500.0f,500.0f }, { 1.0f,0.0f,0.0f,1.0f }, { 0,0 }, false, true);

	//球の初期値を設定
	sphere.center = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);	//中心点座標
	sphere.radius = 1.0f;	//半径

	//平面の初期値を設定
	plane.normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);		//法線ベクトル
	plane.distance = 0.0f;	//原点からの座標

	//三角形の初期値を設定
	triangle.p0 = XMVectorSet(-1.0f, 0.0f, -1.0f, 1.0f);	//左手前
	triangle.p1 = XMVectorSet(-1.0f, 0.0f, +1.0f, 1.0f);	//左奥
	triangle.p2 = XMVectorSet(+1.0f, 0.0f, -1.0f, 1.0f);	//右手前
	triangle.normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);	//上向き

	//レイの初期値を設定
	ray.start = XMVectorSet(0.0f, 5.0f, 0.0f, 1.0f);		//原点やや上
	ray.dir = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);			//下向き

	scene = SxP;
}

void GameScene::Update()
{
	XMVECTOR sphereMoveY;
	XMVECTOR interSP;
	bool hitSP;
	XMFLOAT4 colorSP=objSphere->GetColor();

	std::ostringstream spherestr;
	std::ostringstream raystr;

	XMVECTOR rayMoveX;
	XMVECTOR interRP;
	float distance;
	bool hitRP;
	XMFLOAT4 colorRP = objGround->GetColor();

	switch (scene)
	{
	case SxP:
		//球移動
		sphereMoveY = XMVectorSet(0.0f, 0.03f, 0.0f, 0.0f);
		// 移動の計算
		if (isReverseSphere)
		{
			sphere.center -= sphereMoveY;
		}
		else
		{
			sphere.center += sphereMoveY;
		}

		if (sphere.center.m128_f32[1] >= 3.0f) {
			isReverseSphere = true;
		}
		if (sphere.center.m128_f32[1] <= -3.0f) {
			isReverseSphere = false;
		}

		objSphere->SetPosition({ sphere.center.m128_f32[0]
		,sphere.center.m128_f32[1],sphere.center.m128_f32[2] });

		spherestr << "Sphere:("
			<< std::fixed << std::setprecision(2)		//小数点以下2桁まで
			<< sphere.center.m128_f32[0] << ","			//x
			<< sphere.center.m128_f32[1] << ","			//y
			<< sphere.center.m128_f32[2] << ")";		//z

		debugText.Print(spherestr.str(), 50, 180, 1.0f);

		//球と平面の当たり判定
		hitSP = Collision::ChackSphere2Plane(sphere, plane, &interSP);
		if (hitSP)
		{
			colorSP = { 1.0f,0.0f,0.0f,1.0f };
			objSphere->SetColor(colorSP);
			debugText.Print("HIT Plane!", 700, 180, 1.0f);

			//stringstreamをリセットし、交点座標を埋め込む
			spherestr.str("");
			spherestr.clear();
			spherestr << "("
				<< std::fixed << std::setprecision(2)
				<< interSP.m128_f32[0] << ","
				<< interSP.m128_f32[1] << ","
				<< interSP.m128_f32[2] << ")";

			debugText.Print(spherestr.str(), 50, 200, 1.0f);
		}
		else
		{
			colorSP = { 1.0f,1.0f,1.0f,1.0f };
			objSphere->SetColor(colorSP);
		}

		if (input->TriggerKey(DIK_SPACE))
		{
			ray.start = XMVectorSet(0.0f, 5.0f, 0.0f, 1.0f);		//原点やや上
			scene = RxP;
			break;
		}

		break;

	case RxP:
		//レイ移動
		
		rayMoveX = XMVectorSet(0.05f, 0.00f, 0.0f, 0.0f);
		// 移動の計算
		if (isReverseRay)
		{
			ray.start -= rayMoveX;
		}
		else
		{
			ray.start += rayMoveX;
		}

		if (ray.start.m128_f32[0] >= 20.0f) {
			isReverseRay = true;
		}
		if (ray.start.m128_f32[0] <= -20.0f) {
			isReverseRay = false;
		}

		objRay->SetPosition({ ray.start.m128_f32[0]
		,ray.start.m128_f32[1],ray.start.m128_f32[2] });

		raystr << "Ray Start:("
			<< std::fixed << std::setprecision(2)		//小数点以下2桁まで
			<< ray.start.m128_f32[0] << ","				//x
			<< ray.start.m128_f32[1] << ","				//y
			<< ray.start.m128_f32[2] << ")";			//z

		debugText.Print(raystr.str(), 50, 240, 1.0f);

		hitRP = Collision::ChackRay2Plane(ray, plane, &distance, &interRP);
		if (hitRP)
		{
			colorRP = { 0.5f,0.0f,0.0f,1.0f };
			objGround->SetColor(colorRP);
			debugText.Print("Ray HIT Plane!", 700, 240, 1.0f);

			//stringstreamをリセットし、交点座標を埋め込む
			raystr.str("");
			raystr.clear();
			raystr << "("
				<< std::fixed << std::setprecision(2)
				<< interRP.m128_f32[0] << ","
				<< interRP.m128_f32[1] << ","
				<< interRP.m128_f32[2] << ")";

			debugText.Print(raystr.str(), 50, 260, 1.0f);
		}
		else
		{
			colorRP = { 1.0f,1.0f,1.0f,1.0f };
			objGround->SetColor(colorRP);
		}
		if (input->TriggerKey(DIK_SPACE))
		{
			sphere.center = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);	//中心点座標
			scene = SxP;
			break;
		}
		break;

	case RxS:

		break;

	}


	// オブジェクト移動
	//if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	//{
	//	// 現在の座標を取得
	//	XMFLOAT3 position = objSphere->GetPosition();

	//	// 移動後の座標を計算
	//	if (input->PushKey(DIK_UP)) { position.y += 1.0f; }
	//	else if (input->PushKey(DIK_DOWN)) { position.y -= 1.0f; }
	//	if (input->PushKey(DIK_RIGHT)) { position.x += 1.0f; }
	//	else if (input->PushKey(DIK_LEFT)) { position.x -= 1.0f; }

	//	// 座標の変更を反映
	//	objSphere->SetPosition(position);
	//}

	////球移動
	//XMVECTOR sphereMoveY = XMVectorSet(0.0f, 0.01f, 0.0f, 0.0f);
	//if (input->PushKey(DIK_UP)) { sphere.center += sphereMoveY; }
	//else if (input->PushKey(DIK_DOWN)) { sphere.center -= sphereMoveY; }

	//XMVECTOR  sphereMoveX = XMVectorSet(0.01f, 0.0f, 0.0f, 0.0f);
	//if (input->PushKey(DIK_RIGHT)) { sphere.center += sphereMoveX; }
	//else if (input->PushKey(DIK_LEFT)) { sphere.center -= sphereMoveX; }

	////stringstreamで変数の値を埋め込んで整形する
	//std::ostringstream spherestr;
	//spherestr << "Sphere:("
	//	<< std::fixed << std::setprecision(2)		//小数点以下2桁まで
	//	<< sphere.center.m128_f32[0] << ","			//x
	//	<< sphere.center.m128_f32[1] << ","			//y
	//	<< sphere.center.m128_f32[2] << ")";		//z

	//debugText.Print(spherestr.str(), 50, 180, 1.0f);

	////球と平面の当たり判定
	//XMVECTOR inter;
	//bool hit = Collision::ChackSphere2Plane(sphere, plane, &inter);
	//if (hit)
	//{
	//	debugText.Print("HIT Plane!", 50, 200, 1.0f);

	//	//stringstreamをリセットし、交点座標を埋め込む
	//	spherestr.str("");
	//	spherestr.clear();
	//	spherestr << "("
	//		<< std::fixed << std::setprecision(2)
	//		<< inter.m128_f32[0] << ","
	//		<< inter.m128_f32[1] << ","
	//		<< inter.m128_f32[2] << ")";

	//	debugText.Print(spherestr.str(), 50, 220, 1.0f);
	//}

	//球と三角形の当たり判定
	//XMVECTOR inter;
	//bool hit = Collision::ChackSphere2Triangle(sphere, triangle, &inter);
	//if (hit)
	//{
	//	debugText.Print("HIT Triangle!", 50, 240, 1.0f);

	//	//stringstreamをリセットし、交点座標を埋め込む
	//	spherestr.str("");
	//	spherestr.clear();
	//	spherestr << "("
	//		<< std::fixed << std::setprecision(2)
	//		<< inter.m128_f32[0] << ","
	//		<< inter.m128_f32[1] << ","
	//		<< inter.m128_f32[2] << ")";

	//	debugText.Print(spherestr.str(), 50, 260, 1.0f);
	//}

	//レイと平面の当たり判定
	//XMVECTOR inter;
	//float distance;
	//bool hit = Collision::ChackRay2Plane(ray, plane, &distance, &inter);
	//if (hit)
	//{
	//	debugText.Print("Ray HIT Plane!", 50, 240, 1.0f);

	//	//stringstreamをリセットし、交点座標を埋め込む
	//	raystr.str("");
	//	raystr.clear();
	//	raystr << "("
	//		<< std::fixed << std::setprecision(2)
	//		<< inter.m128_f32[0] << ","
	//		<< inter.m128_f32[1] << ","
	//		<< inter.m128_f32[2] << ")";

	//	debugText.Print(raystr.str(), 50, 260, 1.0f);
	//}

	//レイと三角形の当たり判定
	//XMVECTOR inter;
	//float distance;
	//bool hit = Collision::ChackRay2Triangle(ray, triangle, &distance, &inter);
	//if (hit)
	//{
	//	debugText.Print("Ray HIT Triangle!", 50, 240, 1.0f);

	//	//stringstreamをリセットし、交点座標を埋め込む
	//	raystr.str("");
	//	raystr.clear();
	//	raystr << "inter:("
	//		<< std::fixed << std::setprecision(2)
	//		<< inter.m128_f32[0] << ","
	//		<< inter.m128_f32[1] << ","
	//		<< inter.m128_f32[2] << ")";

	//	debugText.Print(raystr.str(), 50, 260, 1.0f);

	//	raystr.str("");
	//	raystr.clear();
	//	raystr << "distance:("
	//		<< std::fixed << std::setprecision(2)
	//		<< distance << ")";

	//	debugText.Print(raystr.str(), 50, 280, 1.0f);
	//}

	//XMVECTOR inter;
	//float distance;
	//bool hit = Collision::ChackRay2Sphere(ray, sphere, &distance, &inter);
	//if (hit)
	//{
	//	debugText.Print("Ray HIT Sphere!", 50, 240, 1.0f);

	//	//stringstreamをリセットし、交点座標を埋め込む
	//	raystr.str("");
	//	raystr.clear();
	//	raystr << "inter:("
	//		<< std::fixed << std::setprecision(2)
	//		<< inter.m128_f32[0] << ","
	//		<< inter.m128_f32[1] << ","
	//		<< inter.m128_f32[2] << ")";

	//	debugText.Print(raystr.str(), 50, 260, 1.0f);

	//	raystr.str("");
	//	raystr.clear();
	//	raystr << "distance:("
	//		<< std::fixed << std::setprecision(2)
	//		<< distance << ")";

	//	debugText.Print(raystr.str(), 50, 280, 1.0f);
	//}

	// カメラ移動
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		if (input->PushKey(DIK_W)) { Object3d::CameraMoveVector({ 0.0f,-0.3f,0.0f }); }
		else if (input->PushKey(DIK_S)) { Object3d::CameraMoveVector({ 0.0f,+0.3f,0.0f }); }
		if (input->PushKey(DIK_D)) { Object3d::CameraMoveVector({ -0.3f,0.0f,0.0f }); }
		else if (input->PushKey(DIK_A)) { Object3d::CameraMoveVector({ +0.3f,0.0f,0.0f }); }
	}

	objSphere->Update();
	objGround->Update();
	objRay->Update();
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブジェクトの描画
	switch (scene)
	{
	case SxP:
		objSphere->Draw();
		objGround->Draw();
		break;

	case RxP:
		objGround->Draw();
		objRay->Draw();
		break;

	case RxS:
		objSphere->Draw();
		objRay->Draw();

		break;

	}
	/*objSphere->Draw();
	objGround->Draw();
	objRay->Draw();*/

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	//sprite1->Draw();
	//sprite2->Draw();
	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
