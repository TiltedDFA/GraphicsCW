#pragma once
#ifndef SETUPFUNCTIONS_HPP
#define SETUPFUNCTIONS_HPP
#include "Class.hpp"
namespace c1 {// Used to setup or maintain
			
	inline void addCardsToVisibleVec(std::vector<std::shared_ptr<Card>>& cardsVisible, std::shared_ptr<Card> card) {
		
		cardsVisible.push_back(card);
	}
	
	inline void removeCardFromVisibleVect(std::vector<std::shared_ptr<Card>>& cardsVisible, std::shared_ptr<Card> card) {

		auto positionOfCardToRemove = std::find(cardsVisible.begin(), cardsVisible.end(), card);

		if (positionOfCardToRemove != cardsVisible.end()) { cardsVisible.erase(positionOfCardToRemove); }
	}

	inline void setUpPlayerHandPos(std::vector<std::shared_ptr<Card>>& cardsVisible, Deck& deck, std::array<Player, 2>& players) {

		float x = 600;
		
		float y = 75;
		
		for (int i = 0; i < 6; ++i)	{

			std::shared_ptr<Card> card = deck.dealCard();

			//card->canBeTouched = false; Temp disabled until ai is made

			card->cardIsFaceUp = false;

			card->cardPosition = { (x + 125 * i),y };

			players[0].addToPlayerHand(card);

			cardsVisible.push_back(card);
		}
	
		y = 840;
		
		for (int i = 0; i < 6; ++i)	{

			std::shared_ptr<Card> card = deck.dealCard();

			card->cardPosition = { (x + 125 * i),y };

			players[1].addToPlayerHand(card);

			cardsVisible.push_back(card);
		}		

		deck.setPosLastCard({ 376, 503 });	
	}
	
	inline auto findStartingPlayer(std::array<Player, 2> players, Deck& deck) -> int
	{
		
		cardValue lowestVal[2] = { cardValue::ACE, cardValue::ACE };
		
		const auto mS = Deck::masterSuit;
		
		for (int i = 0; i < 2; ++i) {
	
			const auto hSize = players[i].getPlayerHandSize();
			
			for (int j = 0; j < hSize; ++j) {
	
				if (players[i].getPlayerHandIndex(j)->Suit == mS && players[i].getPlayerHandIndex(j)->Value < lowestVal[i])	{
		
					lowestVal[i] = players[i].getPlayerHandIndex(j)->Value;
				}
			}
		}
		if (lowestVal[0] == cardValue::ACE && lowestVal[1] == cardValue::ACE){ // This is to randomise the starting player if neither has a master suit
		
			return rand() % 2 + 1;
		}
		if (lowestVal[0] > lowestVal[1]){
		
			return 1;
		}
		else { return 0; }
	}
	
	inline void lockCardsInHand(std::array<Player, 2>players, MainGame mg) {
	
		auto pTurn = mg.getPTurn();
		
		for (int i = 0; i < players[pTurn].getPlayerHandSize(); ++i) {
		
			auto card = players[pTurn].getPlayerHandIndex(i);
			
			card->canBeTouched = true;
			
			players[pTurn].setPlayerHandIndex(card, i);
		}
		
		auto otherPlayer = (pTurn + 1) % 2;
	
		for (int i = 0; i < players[otherPlayer].getPlayerHandSize(); ++i) {
		
			auto card = players[otherPlayer].getPlayerHandIndex(i);
	
			card->canBeTouched = false;
	
			players[otherPlayer].setPlayerHandIndex(card, i);
		}
	}
	
	inline void bringCardOneToTop(std::shared_ptr<Card> cardOne, std::shared_ptr<Card> cardTwo, std::vector<std::shared_ptr<Card>>& cardsVisible) {
	
		auto cOnePos = std::find(cardsVisible.begin(), cardsVisible.end(), cardOne);
	
		auto cTwoPos = std::find(cardsVisible.begin(), cardsVisible.end(), cardTwo);
	
		if (cOnePos < cTwoPos) { std::iter_swap(cOnePos, cTwoPos); }		
	}	
	
	inline void removeTableFromVisibleVec(Table& table, std::vector<std::shared_ptr<Card>>& cardsVisible) {
	
		const std::array<std::array<std::shared_ptr<Card>, 2>, 6> _Table = table.getEntireTable();
		
		for (const auto& i : _Table) {
		
			for (int j = 0; j < 2; ++j)	{
			
				if (i[j] != nullptr) {
				

					auto posInCardsVisible = std::find(cardsVisible.begin(), cardsVisible.end(), i[j]);
					if (posInCardsVisible != cardsVisible.end()) 
						cardsVisible.erase(posInCardsVisible);
					
				}
			}
		}
	}
			
	inline bool doesDeckHaveEnoughCardsRemaining(Deck& deck, const int numOfCardsNeeded) {
	
		int deckSize = deck.getDeckSize();
	
		if ((deckSize - numOfCardsNeeded) > 0) { return true; }
	
		return false;
	}
	
	inline void addNeededCardsToPlayerHands(std::array<Player, 2>&players, std::vector<std::shared_ptr<Card>>& cardsVisible, Deck& deck) {		
	
		for (int i = 0; i < 2; ++i) {
		
			const auto cardsNeeded = (6 - players[i].getPlayerHandSize());
			
			if (deck.getDeckSize()) { // code below will not run if deckSize == 0;
			
				if (doesDeckHaveEnoughCardsRemaining(deck, cardsNeeded) && static_cast<int>(players[i].getPlayerHandSize()) < 6) {
				
					for (auto j = 0; j < cardsNeeded; ++j) {
					
						std::shared_ptr<Card>card = deck.dealCard();
						
						card->cardPosition.y = (i) ? 850.0f : 0.0f;
						
						card->cardPosition.x = static_cast<float>(120 * j);
						
						players[i].addToPlayerHand(card);
						
						cardsVisible.push_back(card);
					}
				}
			}					
		}
	}	
}
namespace c3  {// C3 will be for reading and writing to files (for settings)

	inline void setDeckSize(const int deckSize, const int currentMusicVal) {
	
		std::ofstream save_file(fileName, std::ios::out); // This creates the file		
		if (save_file) {
	
			switch (deckSize) {
			
			case 36:
			
				save_file << "36";
				
				break;
			
			case 52:
		
				save_file << "52";
				
				break;
		
			default:
		
				save_file << "0";
				
				break;
			}

			switch (currentMusicVal) {
		
			case 1:
			
				save_file << "\n1";
				
				break;
		
			case 2:
	
				save_file << "\n2";
			
				break;
		
			case 3:
		
				save_file << "\n3";
				
				break;
		
			case 4:
		
				save_file << "\n4";
			
				break;
			
			default:
		
				throw std::runtime_error("unexpected musical Value input");
			
				break;
			}
			
			save_file.close();
		}
	}		
	
	inline const int getDeckSize() {
		
		std::ifstream save_file(fileName, std::ios::in);
	
		if (!save_file)	{
	
			setDeckSize(36, 1);
	
			return 36;
		}
	
		std::string size;
		
		save_file.seekg(std::ios::beg);
		
		std::getline(save_file, size);
		
		if (size == "36")
			return 36;
	
		else if (size == "52")
			return 52;
		
		else return 0;
	}
	
	inline void setMusicalNumber(const int num, const int currentDeckSize) {
	
		std::ofstream save_file(fileName, std::ios::out);
		
		if (save_file) {
		
			save_file << currentDeckSize;
		
			switch (num) {
		
			case 1:
			
				save_file << "\n1";
				
				break;
		
			case 2:
				
				save_file << "\n2";
	
				break;

			case 3:
			
				save_file << "\n3";
			
				break;
	
			case 4:

				save_file << "\n4";
	
				break;
		
			default:
		
				throw std::runtime_error("Unexpected input Value experianced");
		
				break;
			}
		}
	}	
	
	inline const int getMusicNumber() {
		
		std::ifstream save_file(fileName, std::ios::in);
	
		if (!save_file)	{
		
			setDeckSize(36, 1);
	
			setMusicalNumber(1, 36);
	
			return 1;
		}
		
		std::string num;
	
		std::getline(save_file, num);

		std::getline(save_file, num);

		if (num == "1")
			return 1;

		else if (num == "2")
			return 2;

		else if (num == "3")
			return 3;

		else if (num == "4")
			return 4;
		
		else
			throw std::runtime_error("Unexpected Value read");
	}
} 
#endif // !SETUPFUNCTIONS_HPP
