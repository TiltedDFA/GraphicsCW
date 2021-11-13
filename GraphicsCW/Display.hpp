#pragma once
#ifndef DISPLAY_HPP
#define DISPLAY_HPP
#include "Class.hpp"

namespace c0 // c0 will be the setting up / maintaing function
{
	void addCardsToVisibleVec(std::vector<std::shared_ptr<Card>>&cardsVisible, std::shared_ptr<Card> card)
	{
		cardsVisible.push_back(card);
	}
	void removeCardFromVisibleVect(std::vector<std::shared_ptr<Card>>& cardsVisible, std::shared_ptr<Card> card)
	{
		auto positionOfCardToRemove = std::find(cardsVisible.begin(), cardsVisible.end(), card);
		if (positionOfCardToRemove != cardsVisible.end()) { cardsVisible.erase(positionOfCardToRemove); }
	}
	void setUpPlayerHandPos(std::vector<std::shared_ptr<Card>>& cardsVisible, Deck& deck, std::array<Player, 2>& players)
	{
		float x = 600;
		float y = 75;
		for (int i = 0; i < 6; ++i)
		{
			std::shared_ptr<Card> card = deck.dealCard();
			card->canBeTouched = false;
			card->cardIsFaceUp = false;
			card->cardPosition = { (x + 125 * i),y };
			players[0].addToPlayerHand(card);
			cardsVisible.push_back(card);
		}
		y = 840;
		for (int i = 0; i < 6; ++i)
		{
			std::shared_ptr<Card> card = deck.dealCard();		
			card->cardPosition = { (x + 125 * i),y };
			players[1].addToPlayerHand(card);
			cardsVisible.push_back(card);
		}
		std::shared_ptr<Card> card = deck.dealCard();
		deck.setVisibleCard(card);
		deck.setMasterSuit(card->Suit);
	}
}
namespace c1 // This namespace does something e.g. finding the starting player
{
	int findStartingPlayer(std::array<Player, 2>players, Deck deck)
	{
		cardValue lowestVal[2] = { cardValue::ACE, cardValue::ACE };
		auto mS = deck.getMasterSuit();
		for (int i = 0; i < 2; ++i)
		{
			auto hSize = players[i].getPlayerHandSize();
			for (int j = 0; j < hSize; ++i)
			{
				if (players[i].getPlayerHandIndex(j)->Suit == mS && players[i].getPlayerHandIndex(j)->Value < lowestVal[i])
				{
					lowestVal[i] = players[i].getPlayerHandIndex(j)->Value;
				}
			}
		}
		if (lowestVal[0] > lowestVal[1])
		{
			return 1;
		}
		else { return 0; }
	}
	bool placeHBC(int percentCertanty, std::shared_ptr<Card> card)
	{
		std::array<Vector2, 6>pos;
		pos[0] = { 543.0, 457.0 };
		pos[1] = { 688.0, 457.0 };
		pos[2] = { 840.0, 457.0 };
		pos[3] = { 990.0, 457.0 };
		pos[4] = { 1143.0,457.0 };
		pos[5] = { 1300.0,457.0 };
		Rectangle rCard = { card->cardPosition.x, card->cardPosition.y, 120, 170 };
		for (int i = 0; i < 6; ++i)
		{
			Rectangle placeHB = { pos[i].x, pos[i].y, 120, 170 };
			if (CheckCollisionRecs(rCard, placeHB))
			{
				auto xOSet = placeHB.width - sqrt(((card->cardPosition.x - placeHB.x) * (card->cardPosition.x - placeHB.x)));
				auto yOSet = placeHB.height - sqrt(((card->cardPosition.y - placeHB.y) * (card->cardPosition.y - placeHB.y)));
				auto oSetArea = xOSet * yOSet;
				auto area = placeHB.width * placeHB.height;
				auto percentOverlap = ((oSetArea / area) * 100);
				if ((int)percentOverlap > percentCertanty)
				{
					true;
				}
			}
		}
		return false;
	}
	bool canCardBePlayed(std::array<std::array<std::shared_ptr<Card>, 2>, 6>cardsOnTable, std::shared_ptr<Card> card) // This function checks if an attacking card can be played based on the cards in the table.
	{
		if (cardsOnTable[0][0]->Name == "" && cardsOnTable[1][0]->Name == "" && cardsOnTable[2][0]->Name == "" && cardsOnTable[3][0]->Name == "" && cardsOnTable[4][0]->Name == "" && cardsOnTable[5][0]->Name == "")
		{
			return true;
		}
		if (cardsOnTable[0][0]->Value == card->Value || cardsOnTable[1][0]->Value == card->Value || cardsOnTable[2][0]->Value == card->Value || cardsOnTable[3][0]->Value == card->Value || cardsOnTable[4][0]->Value == card->Value || cardsOnTable[5][0]->Value == card->Value)
		{
			return true;
		}
		return false;
	}
	bool cardBeatsCard(const std::shared_ptr<Card> cardOne, const std::shared_ptr<Card> cardTwo, Deck deck)
	{
		cardSuit mS = deck.getMasterSuit();
		if (cardOne->Suit == mS && cardTwo->Suit != mS) { return true; }
		else { return false; }
		if (cardOne->Suit == cardTwo->Suit && cardOne->Value > cardTwo->Value) { return true; }
		else { return false; }
		return false;
	}
	Vector2 BoxColFinder(const std::shared_ptr<Card> card)
	{
		std::array<Vector2, 6>pos;
		pos[0] = { 543.0, 457.0 };
		pos[1] = { 688.0, 457.0 };
		pos[2] = { 840.0, 457.0 };
		pos[3] = { 990.0, 457.0 };
		pos[4] = { 1143.0,457.0 };
		pos[5] = { 1300.0,457.0 };
		Rectangle rCard = { card->cardPosition.x, card->cardPosition.y, 120, 170 };

		for (int i = 0; i < 6; ++i)
		{
			Rectangle box = { pos[i].x, pos[i].y, 120, 170 };
			if (CheckCollisionRecs(rCard, box)) { return pos[i]; }
		}
		return { 0,0 };
	}
}
namespace c2
{
	void cTable(std::vector<std::shared_ptr<Card>>cardsVisible, Texture2D& cBack, Texture2D& cBlank, int& cardsOnScreen) // (Xpos, Ypos, AmountOfCards, Cardtext)
	{
		for (auto i = 0; i < cardsVisible.size(); ++i)
		{
			if (cardsVisible[i]->cardIsFaceUp)
			{
				DrawTexture(cBlank, (int)cardsVisible[i]->cardPosition.x, (int)cardsVisible[i]->cardPosition.y, WHITE);
			}
			else if (!cardsVisible[i]->cardIsFaceUp)
			{
				DrawTexture(cBack, (int)cardsVisible[i]->cardPosition.x, (int)cardsVisible[i]->cardPosition.y, WHITE);
			}
		}
	}
}
#endif // !DISPLAY_HPP