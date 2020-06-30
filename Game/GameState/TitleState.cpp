#include "pch.h"

#include "TitleState.h"

#include "DeviceResources.h"
#include "DebugFont.h"
#include "GameContext.h"
#include "GameStateManager.h"
#include "WICTextureLoader.h"
#include "EffectMask.h"
#include "Blink.h"

TitleState::TitleState()
	: GameState()
	, m_count(0)
{

}



TitleState::~TitleState()
{
}



void TitleState::Initialize()
{
	m_count = 0;
	m_displayFlag = false;
	
	DX::DeviceResources* deviceResources = GameContext::Get<DX::DeviceResources>();
	GameContext::Get<EffectMask>()->Open();
	DirectX::CreateWICTextureFromFile(deviceResources->GetD3DDevice(),
		L"Resources\\Textures\\title.png", nullptr, m_titleTexture.GetAddressOf());
	DirectX::CreateWICTextureFromFile(deviceResources->GetD3DDevice(),
		L"Resources\\Textures\\push_space.png", nullptr, m_pushSpaceTexture.GetAddressOf());
}



void TitleState::Update(float elapsedTime)
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	
	// スペースが押されたら
	if (GameContext::Get<EffectMask>()->IsOpen() && keyState.Space)
	{
		// エフェクトマスクを閉じる
		GameContext::Get<EffectMask>()->Close();
	}
	// エフェクトマスクが閉じきったら
	if (GameContext::Get<EffectMask>()->IsClose())
	{
		// プレイへ
		GameStateManager* gameStateManager = GameContext::Get<GameStateManager>();
		gameStateManager->RequestState("Play");
	}
}



void TitleState::Render()
{
	DebugFont* debugFont = DebugFont::GetInstance();
	debugFont->print(10, 10, L"TitleState");
	debugFont->draw();
	debugFont->print(10, 40, L"%3d / 120", m_count);
	debugFont->draw();

	GameContext::Get<DirectX::SpriteBatch>()->Begin();

	// タイトルの描画
	RECT destRect = { 0,0,1280,720 };
	GameContext::Get<DirectX::SpriteBatch>()->Draw(m_titleTexture.Get(), destRect);

	// PUSH STARTの描画
	if (m_displayFlag)
	{
		GameContext::Get<DirectX::SpriteBatch>()->Draw(m_pushSpaceTexture.Get(), DirectX::Colors::Red);
	}

	GameContext::Get<DirectX::SpriteBatch>()->End();
}



void TitleState::Finalize()
{

}
