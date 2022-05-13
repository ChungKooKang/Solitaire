#include <string>
#include <vector>
#include <random>
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
		Card* pCard{};
		for (auto& card : mDeck)
		{
			if (card.CheckClicked(mouseX, mouseY))
			{
				pCard = &card;
				break;
			}
		}

		if (pCard)
		{
			mFlipCount++;
			RECT rct{
				static_cast<LONG>(mCountRect.GetLeft()),
				static_cast<LONG>(mCountRect.GetTop()),
				static_cast<LONG>(mCountRect.GetRight()),
				static_cast<LONG>(mCountRect.GetBottom())
			};

			InvalidateRect(mHwnd, &rct, false);

			if (mpSelectedCard == nullptr)
			{
				mpSelectedCard = pCard;
			}
			else
			{
				if (pCard->GetType() == mpSelectedCard->GetType())
				{
					mpSelectedCard->Invalidate();
					mDeck.remove_if([&](Card& card) {
						return (card.GetIndex() == pCard->GetIndex() || card.GetIndex() == mpSelectedCard->GetIndex());
						}
					);
					mpSelectedCard = nullptr;
				}
				else
				{
					UpdateWindow(mHwnd);
					Sleep(500);
					pCard->Flip(false);
					mpSelectedCard->Flip(false);

					mpSelectedCard = nullptr;
				}
			}

		}
	}

	void Game::CreateCards()
	{
		std::vector<Type> types;
		while (types.size() < static_cast<size_t>(BOARD_COLUMN * BOARD_ROW))
		{
			int temp = types.size() % 6;
			switch (temp)
			{
			case 0 :
				types.push_back(Type::Bear);
				types.push_back(Type::Bear);
				break;

			case 2  :
				types.push_back(Type::Wolf);
				types.push_back(Type::Wolf);
				break;

			case 4 :
				types.push_back(Type::Dragon);
				types.push_back(Type::Dragon);
				break;
			}
		}

		std::random_device rd;
		std::mt19937 gen(rd());
		std::shuffle(types.begin(), types.end(), gen);

		int index{};
		int posX{ 15 }, posY{ 10 };
		for (int x = 0; x < BOARD_COLUMN; ++x)
		{
			posY = { 10 };

			for (int y = 0; y < BOARD_ROW; ++y)
			{
				mDeck.push_back(Card(mHwnd, index, types[index++], posX, posY));

				posY += 140 + 10;
			}
			posX += 100 + 10;
		}
	}

}