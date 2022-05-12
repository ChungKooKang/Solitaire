#include <string>
#include "Game.h"

namespace solitaire
{
	void Game::Init(HWND hwnd)
	{
		mHwnd = hwnd;
		mBackground = std::make_unique<Gdiplus::Image>(L"Data/bg_blank.png");

		CreateCards();
	}

	void Game::Release()
	{
		mDeck.clear();
		// mBackground.release(); // delete p;
		mBackground.reset();   // delete p; p =nullptr
	}

	void Game::Draw(Gdiplus::Graphics& graphics)
	{
		graphics.DrawImage(mBackground.get(), 0, 0, mBackground->GetWidth(), mBackground->GetHeight());

		for (auto& card : mDeck)
		{
			card.Draw(graphics);
		}

		// 글자 만들기
		Gdiplus::PointF pos(895.0f, 20.0f);

		Gdiplus::SolidBrush brush(Gdiplus::Color(255, 79, 64));
		Gdiplus::Font font(L"맑은 고딕", 20);

		graphics.DrawString(L"클릭수 : ", -1, &font, pos, &brush); // -1 넣어주면 \0까지 알아서 새어줌

		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentCenter);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

		graphics.DrawString(
			std::to_wstring(mFlipCount).c_str(),
			-1, &font, mCountRect, &format, &brush
			);
	}

	void Game::OnClick(int mouseX, int mouseY)
	{
		for (auto& card : mDeck)
		{
			if (card.CheckClicked(mouseX, mouseY))
			{
				//TODO : 클릭처리
			}
		}
	}

	void Game::CreateCards()
	{
		//TODO : 카드 섞기

		mDeck.push_back(Card(mHwnd, Type::Bear, 0, 0));
		mDeck.push_back(Card(mHwnd, Type::Wolf, 120, 0));
		mDeck.push_back(Card(mHwnd, Type::Dragon, 240, 0));
	}

}