#pragma once


#include "GameState.h"
#include "SpriteBatch.h"
#include "SimpleMath.h"
#include <Keyboard.h>

class TitleState: public GameState
{
	private:
		int m_count;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_titleTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pushSpaceTexture;
		bool m_displayFlag;
public:
		TitleState();

	public:
		virtual ~TitleState();


	public:
		void Initialize() override;
		void Update(float elapsedTime) override;
		void Render() override;
		void Finalize() override;
};
