#include <string>
#include "Card.h"


namespace solitaire
{
	Card::Card(Type type, int x, int y) :
		mX(x), mY(y),
		mIsFront( false),
		mType(type)
	{
		std::wstring filename(L"Data/card_creature_");
		switch (type)
		{
		case Type::Wolf :
			filename += L"wolf.png";
			break;
		case Type::Dragon :
			filename += L"dragon.png";
			break;
		case Type::Bear :
			filename += L"bear.png";
			break;
		}

		mFront = std::make_unique<Gdiplus::Image>(filename.c_str());
		mBack = std::make_unique<Gdiplus::Image>(L"Data/card_back.png");
	}

	bool Card::CheckClicked(int mouseX, int mouseY)
	{
		// TODO : 마우스 클릭 체크
		return false;
	}

	void Card::Flip(bool isFront)
	{
		mIsFront = isFront;
	}

	void Card::Draw(Gdiplus::Graphics& graphics)
	{
		if (mIsFront)
		{
			graphics.DrawImage(mFront.get(), mX, mY, mFront->GetWidth(),mFront->GetHeight());
		}
		else
		{
			graphics.DrawImage(mBack.get(), mX, mY, mBack->GetWidth(), mBack->GetHeight());
		}
	}

}