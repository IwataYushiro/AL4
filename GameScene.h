﻿#pragma once

#include "CollisionPrimitive.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "Object3d.h"
#include "DebugText.h"

class CollisionManager;
class Player;
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;

public: // メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, Input* input);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//当たり判定　球と平面
	void IsCollisionSphereAndPlane();
	//当たり判定　球と三角形
	void IsCollisionSphereAndTriangle();

private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	DebugText debugText;	

	//スプライト
	Sprite* sprite1 = nullptr;
	Sprite* sprite2 = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	Sprite* spriteBG = nullptr;
	Object3d* objSphere = nullptr;
	Object3d* objGround = nullptr;
	Player* objFighter = nullptr;

	Model* modelSphere = nullptr;
	Model* modelGround = nullptr;
	Model* modelFighter = nullptr;

	//当たり判定　球
	Sphere sphere;
	//当たり判定　平面
	Plane plane;
	//当たり判定　三角形
	Triangle triangle;
	//当たり判定　レイ
	Ray ray;
	//衝突マネージャ
	CollisionManager* colManager = nullptr;
};

