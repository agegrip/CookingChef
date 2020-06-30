#pragma once

#include <memory>

#include "GameState.h"
#include "ObjectManager.h"





class PlayState: public GameState
{
	public:
		PlayState();

	public:
		virtual ~PlayState();


	public:
		void Initialize() override;
		void Update(float elapsedTime) override;
		void Render() override;
		void Finalize() override;

    private:
		// ゲームオブジェクトマネージャー
		std::unique_ptr<ObjectManager>         m_pObjectManager;

    	// ゲーム画面のビューポート
    	D3D11_VIEWPORT                         m_viewportGame;
    
    	// 情報画面のビューポート
    	D3D11_VIEWPORT                         m_viewportInfo;
};
