#include "Player.h"
#include "Input.h"
#include "DebugText.h"
#include "SphereCollider.h"
#include "ParticleManager.h"

using namespace DirectX;

Player* Player::Create(Model* model)
{
	Player* ins = new Player();
	if(ins==nullptr)return nullptr;

	//初期化
	if (!ins->Initialize())
	{
		delete ins;
		assert(0);
	}
	if (model)ins->SetModel(model);
	
	return ins;
}

bool Player::Initialize()
{
	if (!Object3d::Initialize())return false;

	//コライダー追加
	float radius = 0.6f;
	//半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(XMVECTOR({ 0.0f,radius,0.0f,0.0f }), radius));
	return true;
}

void Player::Update()
{
	Input* input = Input::GetInstance();
	//旋回
	if (input->PushKey(DIK_A))rotation.y -= 2.0f;
	else if (input->PushKey(DIK_D))rotation.y += 2.0f;

}

void Player::OnCollision(const CollisionInfo& info)
{
}
