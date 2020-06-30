#include "pch.h"

#include "PlayState.h"

#include <Keyboard.h>

#include <CommonStates.h>
#include "DebugFont.h"
#include "DeviceResources.h"
#include "GameContext.h"
#include "GameStateManager.h"
#include "GameObjectManager.h"
#include "ObjectManager.h"
#include "InfoWindow.h"
#include "GameWindow.h"


PlayState::PlayState()
	: GameState()
{
}



PlayState::~PlayState()
{
}



void PlayState::Initialize()
{
	DirectX::CommonStates* state           = GameContext::Get<DirectX::CommonStates>();
	DX::DeviceResources*   deviceResources = GameContext::Get<DX::DeviceResources>();
	ID3D11Device*          device          = deviceResources->GetD3DDevice();
	ID3D11DeviceContext*   deviceContext   = deviceResources->GetD3DDeviceContext();

	m_pObjectManager = std::make_unique<ObjectManager>();

	GameContext::Register<ObjectManager>(m_pObjectManager);

	// ビューポートの領域の設定(ゲーム画面)
	m_viewportGame = CD3D11_VIEWPORT(
		0.0f,
		0.0f,
		static_cast<float>(960),
		static_cast<float>(720)
	);
	// ビューポートの領域の設定(情報画面)
	m_viewportInfo = CD3D11_VIEWPORT(
		static_cast<float>(960),
		0.0f,
		static_cast<float>(InfoWindow::SCREEN_W),
		static_cast<float>(InfoWindow::SCREEN_H)
	);

	// 情報ウィンドウ生成
	std::unique_ptr<InfoWindow> pInfoWindow = std::make_unique<InfoWindow>();
	// 生ポインタを登録
	GameContext::Register<InfoWindow>(pInfoWindow.get());
	// 情報ウィンドウを登録
	GameContext::Get<ObjectManager>()->GetInfoOM()->Add(std::move(pInfoWindow));
	// 情報ウィンドウ初期化
	GameContext::Get<InfoWindow>()->Initialize();

	// ゲームウィンドウ作成
	std::unique_ptr<GameWindow> pGameWindow = std::make_unique<GameWindow>();
	// 生ポインタを登録
	GameContext::Register<GameWindow>(pGameWindow.get());
	// ゲームウィンドウを登録
	GameContext::Get<ObjectManager>()->GetGameOM()->Add(std::move(pGameWindow));
	// ゲームウィンドウを初期化
	GameContext::Get<GameWindow>()->Initialize();


}



void PlayState::Update(float elapsedTime)
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	if (keyState.IsKeyDown(DirectX::Keyboard::P))
	{
		GameStateManager* gameStateManager = GameContext::Get<GameStateManager>();
		gameStateManager->PushState("Pause");
	}
	// 情報画面のオブジェクト更新
	m_pObjectManager->GetInfoOM()->Update(elapsedTime);
	// ゲーム画面のオブジェクト更新
	m_pObjectManager->GetGameOM()->Update(elapsedTime);
}



void PlayState::Render()
{
	DX::DeviceResources* deviceResources = GameContext::Get<DX::DeviceResources>();

	//--------------------//
	// ゲーム画面の描画   //
	//--------------------//
	//ビューポートを変更する(左側へ描画エリアを変更する)
	GameContext::Get<DX::DeviceResources>()->GetD3DDeviceContext()->RSSetViewports(1, &m_viewportGame);

	GameContext::Get<DirectX::SpriteBatch>()->Begin(DirectX::SpriteSortMode_Deferred, GameContext::Get<DirectX::CommonStates>()->NonPremultiplied());

	// TODO: ビュー行列とプロジェクション行列を設定
	DirectX::SimpleMath::Matrix viewMat, projMat;
	// ゲーム画面のオブジェクト描画
	m_pObjectManager->GetGameOM()->Render();

	GameContext::Get<DirectX::SpriteBatch>()->End();
	//--------------------//
	// 情報画面の描画     //
	//--------------------//
	//ビューポートを変更する(右側へ描画エリアを変更する)
	GameContext::Get<DX::DeviceResources>()->GetD3DDeviceContext()->RSSetViewports(1, &m_viewportInfo);

	GameContext::Get<DirectX::SpriteBatch>()->Begin(DirectX::SpriteSortMode_Deferred, GameContext::Get<DirectX::CommonStates>()->NonPremultiplied());

	// 情報画面のオブジェクト描画
	m_pObjectManager->GetInfoOM()->Render();

	GameContext::Get<DirectX::SpriteBatch>()->End();
	
	// ビューポートを変更する(画面全体)
	auto viewport = GameContext::Get<DX::DeviceResources>()->GetScreenViewport();
	GameContext::Get<DX::DeviceResources>()->GetD3DDeviceContext()->RSSetViewports(1, &viewport);

	DebugFont* debugFont = DebugFont::GetInstance();
	debugFont->print(10, 10, L"PlayState");
	debugFont->draw();
	debugFont->print(10, 40, L"[P] Pause");
	debugFont->draw();
}



void PlayState::Finalize()
{
	// 消えるフラグ設定
	GameContext::Reset<ObjectManager>();
	m_pObjectManager.reset();

	// GameContextからはずす
	GameContext::Reset<InfoWindow>();	
	GameContext::Reset<GameWindow>();
}
