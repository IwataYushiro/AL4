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

	//������
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

	//�R���C�_�[�ǉ�
	float radius = 0.6f;
	//���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(XMVECTOR({ 0.0f,radius,0.0f,0.0f }), radius));
	return true;
}

void Player::Update()
{
	Input* input = Input::GetInstance();
	//����
	if (input->PushKey(DIK_A))rotation.y -= 2.0f;
	else if (input->PushKey(DIK_D))rotation.y += 2.0f;

}

void Player::OnCollision(const CollisionInfo& info)
{
}
