#pragma once
#ifndef DISPLAY_HPP
#define DISPLAY_HPP
#include "Class.hpp"

namespace c0 // c0 will be the setting up / maintaing function
{
	void setTopCards(Deck& deck)
	{
		//376, 503
		std::pair<std::shared_ptr<Card>, std::shared_ptr<Card>> cPair;
		cPair.first = deck.dealCard();
		cPair.second = deck.dealCard();
		cPair.first->cardPosition = { 153, 470 };
		cPair.second->cardPosition = { 153, 470 };
		deck.setTopOfDeck(cPair);
	}
	void addCardsToVisibleVec(std::vector<std::shared_ptr<Card>>& cardsVisible, std::shared_ptr<Card> card)
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
			//card->canBeTouched = false; Temp disabled until ai is made
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
		card->cardPosition = { 376, 503 };
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
			for (int j = 0; j < hSize; ++j)
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
	int  placeHBC(int percentCertanty, std::shared_ptr<Card> card)
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
				if (static_cast<int>(percentOverlap > percentCertanty))
				{
					return static_cast<int>(percentOverlap);
				}
			}
		}
		return 0;
	}
	bool canCardBePlayed(Table& table, std::shared_ptr<Card> card) // This function checks if an attacking card can be played based on the cards in the table.
	{
		

		if (!card->cardIsFaceUp) { return false; }
		std::array<std::array<std::shared_ptr<Card>, 2>, 6>cardsOnTable = table.getEntireTable();
		//auto cardPosInTable = std::find(cardsOnTable.begin(), cardsOnTable.end(), card);
		//if (cardPosInTable != cardsOnTable.end()) { return false; }
		if (cardsOnTable[0][0] == nullptr && cardsOnTable[1][0] == nullptr && cardsOnTable[2][0] == nullptr && cardsOnTable[3][0] == nullptr && cardsOnTable[4][0] == nullptr && cardsOnTable[5][0] == nullptr)
		{
			if (cardsOnTable[0][1] == nullptr && cardsOnTable[1][1] == nullptr && cardsOnTable[2][1] == nullptr && cardsOnTable[3][1] == nullptr && cardsOnTable[4][1] == nullptr && cardsOnTable[5][1] == nullptr)
			{
				return true;
			}
		}		
		for (auto const i : cardsOnTable)
		{
			for (int j = 0; j < 2; ++j)
			{
				if(i[j] != nullptr){ if (i[j]->Value == card->Value) { return true; } }				
			}
		}
	return false;
	}
	bool cardBeatsCard(const std::shared_ptr<Card> cardOne, const std::shared_ptr<Card> cardTwo, Deck deck)
	{
		if (cardTwo == nullptr) { return true; }// This code assumes that the only cards that will be passed to the function will be from the table
		cardSuit mS = deck.getMasterSuit();
		if (cardOne->Suit == mS && cardTwo->Suit != mS) { return true; }
		else if (cardOne->Suit == cardTwo->Suit && cardOne->Value > cardTwo->Value) { return true; }
		return false;
	}
	std::pair<Vector2, int> BoxColFinder(const std::shared_ptr<Card> card)
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
			if (CheckCollisionRecs(rCard, box)) { return { pos[i], i }; }
		}
		return { { 0,0 }, 7 };
	}
	void moveCardFromPlayerHandToTable(Player& player, Table& table, std::shared_ptr<Card> card, const int cardPile)
	{
		std::vector<std::shared_ptr<Card>> playerHand = player.getEntireHand();
		auto posInHand = std::find(playerHand.begin(), playerHand.end(), card);
		if (posInHand != playerHand.end())
		{
			if (player.isPlyrAtk()) { table.addCardToTableAtk(std::move(*posInHand), cardPile); playerHand.erase(posInHand); }

			else { table.addCardToTableDef(std::move(*posInHand), cardPile); playerHand.erase(posInHand);}

			player.setEntireHand(playerHand);
		}
	}
	void moveAllTableToBPile(DiscardedCards& bPile, Table& table)
	{
		std::array<std::array<std::shared_ptr<Card>, 2>, 6>cardsOnTable = table.getEntireTable();
		for (auto& i : cardsOnTable)
		{
			for (int j = 0; j < 2; ++j)
			{
				if (i[j] != nullptr) { bPile.addToPile(std::move(i[j])); }
			}
		}
		
	}
	void lockCardsInHand(std::array<Player, 2>players, MainGame mg)
	{
		auto pTurn = mg.getPTurn();
		for (int i = 0; i < players[pTurn].getPlayerHandSize(); ++i)
		{
			auto card = players[pTurn].getPlayerHandIndex(i);
			card->canBeTouched = true;
			players[pTurn].setPlayerHandIndex(card, i);
		}
		auto otherPlayer = (pTurn + 1) % 2;
		for (int i = 0; i < players[otherPlayer].getPlayerHandSize(); ++i)
		{
			auto card = players[otherPlayer].getPlayerHandIndex(i);
			card->canBeTouched = false;
			players[otherPlayer].setPlayerHandIndex(card, i);
		}
	}
}
namespace c2 // This namespace is used to display
{
	void cTable(std::vector<std::shared_ptr<Card>>cardsVisible, Texture2D& cBack, Texture2D& cBlank) // (Xpos, Ypos, AmountOfCards, Cardtext)
	{
		for (auto i = 0; i < cardsVisible.size(); ++i)
		{
			if (cardsVisible[i]->cardIsFaceUp)
			{
				DrawTexture(cBlank, static_cast<int>(cardsVisible[i]->cardPosition.x), static_cast<int>(cardsVisible[i]->cardPosition.y), WHITE);
				cardsVisible[i]->displayCardWithValueText(cardsVisible[i]);
			}
			else if (!cardsVisible[i]->cardIsFaceUp)
			{
				DrawTexture(cBack, static_cast<int>(cardsVisible[i]->cardPosition.x), static_cast<int>(cardsVisible[i]->cardPosition.y), WHITE);
			}
			
		}
	}
	void displayDeckExtraCards(Deck& deck, Texture2D& backOfCard, Texture2D& frontOfCard)
	{
		auto vCard = deck.getVisibleCard();
		auto topCards = deck.getTopOfDeck();
		//Still need to display visible card
		DrawTexture(backOfCard, static_cast<int>(topCards.first->cardPosition.x), static_cast<int>(topCards.first->cardPosition.y), WHITE);
		DrawTexture(backOfCard, static_cast<int>(topCards.second->cardPosition.x), static_cast<int>(topCards.second->cardPosition.y), WHITE);
	}
	void displayPassButtons(MainGame& mg, Texture2D& passLow, Texture2D& passMid, Texture2D& passHigh, Sound& fxButton)
	{
		// 1520, 936
		//CheckCollisionPointRec
		int cardsPlayedThisRound = mg.getCardsPlayed();
		if (!(cardsPlayedThisRound > 0))
		{
			DrawTexture(passLow, 1520, 936, WHITE); 
		}
		else
		{
			Vector2 mP = GetMousePosition();
			Rectangle buttonHitBox = { 1520, 936, 150, 75 };
			if (CheckCollisionPointRec(mP, buttonHitBox))
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
				{
					DrawTexture(passMid, 1520, 936, WHITE);
				}
				else
				{
					DrawTexture(passHigh, 1520, 936, WHITE);
				}
				if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) { PlaySound(fxButton); mg.switchPTurn(); }
			}
			else
			{
				DrawTexture(passLow, 1520, 936, WHITE);
			}
		}
	}
	void displayEndButtons(MainGame& mg, Texture2D& endLow, Texture2D& endMid, Texture2D& endHigh, Sound& fxButton)
	{
		// 1733, 936
		int cardsPlayedThisRound = mg.getCardsPlayed();
		if (!(cardsPlayedThisRound > 0))
		{
			DrawTexture(endLow, 1733, 936, WHITE);
		}
		else
		{
			Vector2 mP = GetMousePosition();
			Rectangle buttonHitBox = { 1733, 936, 150, 75 };
			if (CheckCollisionPointRec(mP, buttonHitBox))
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
				{
					DrawTexture(endMid, 1733, 936, WHITE);
				}
				else
				{
					DrawTexture(endHigh, 1733, 936, WHITE);
				}
				if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) { PlaySound(fxButton); }
			}
			else
			{
				DrawTexture(endLow, 1733, 936, WHITE);
			}
		}
	}
	void displayPlayerState(Texture2D& atkHigh, Texture2D& atkLow, Texture2D& defHigh, Texture2D& defLow, bool atkState)
	{
		//Atk pos is: 1658. 42
		//Def pos is: 1507.44
		if (atkState)
		{
			DrawTexture(atkHigh, 1658, 42, WHITE);
			DrawTexture(defLow, 1507, 44, WHITE);
		}
		else
		{
			DrawTexture(atkLow, 1658, 42, WHITE);
			DrawTexture(defHigh, 1507, 44, WHITE);
		}
	}
	void displayWhosTurnItIs(MainGame& mg)
	{
		auto turn = mg.getPTurn();
		if (turn) // turn is either 1 or 0 which represents true or false (kinda)
		{
			DrawRectangle(1658, 102, 100, 20, WHITE);
		}
		else if (!turn)
		{
			DrawRectangle(1507, 102, 100, 20, WHITE);
		}
	}
}
namespace c3 // This will be used to preform game functions
{
}
#endif // !DISPLAY_HPP