#pragma once
#ifndef DISPLAY_HPP
#define DISPLAY_HPP
#include "raylib.h"
#include <string>
#include <vector>
void DisplayMenu()
{
}
namespace c0
{
	void DisplayBackOfCards(std::vector<Vector2> cPos, Texture2D& Card, int& cardsOnScreen) // (Xpos, Ypos, AmountOfCards, Cardtext)
	{
		for (int i = 0; i < cPos.size(); ++i)
		{
			DrawTexture(Card, cPos[i].x, cPos[i].y, WHITE);
			++cardsOnScreen;
		}
	}
	void mCard(std::vector<Vector2>& cPos, Vector2 mP, int i)
	{
		Vector2 oSet = { (mP.x - cPos[i].x),(mP.y - cPos[i].y) };
		cPos[i].x = (mP.x - oSet.x);
		cPos[i].y = (mP.y - oSet.y);
	}
	std::vector<Vector2> setPosCard(std::vector<Vector2>&cPos)
	{
		int i;
		float x = 600;
		float y = 75;
		for (i=0;i<6;++i){cPos.push_back({(x+125*i),y});}
		y=840;
		for (i=0;i<6;++i){cPos.push_back({(x+125*i),y});}
		return cPos;
	}
}

#endif // !DISPLAY_HPP