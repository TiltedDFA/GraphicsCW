#pragma once
#include "File.hpp"

namespace c2 {// This namespace is for game functions

	extern inline int  collision_percent_finder(int percentCertanty, std::shared_ptr<Card> card);

	extern inline bool can_attacker_attack(Table& table, std::shared_ptr<Card> card);

	extern inline bool card_beats_card_def(const std::shared_ptr<Card>& card_one, const std::shared_ptr<Card>& card_two);

	extern inline std::pair<Vector2, int> find_colliding_placement(const std::shared_ptr<Card>& card);

	extern inline void hand_to_table(Player& player, Table& table, const std::shared_ptr<Card>& card_to_play, const int& cardPile);

	extern inline void discard_table(Discardediscarded_cards& bPile, Table& table, std::vector<std::shared_ptr<Card>>& cardsVisible);

	extern inline bool defend_can_pass(Table& table);

	extern inline void switch_player_state(std::array<Player, 2>& players);

	extern inline bool can_end_attack(Table& table);

	extern inline void table_to_hand(Player& player, Table& table);

	extern inline int check_for_winner(Deck& deck, std::array<Player, 2>& players, Table& table);

	extern inline void play_music(const int music_number, const  Music& JazzMusic, const  Music& LofiMusic, const  Music& eSwingMusic);

	extern inline void continuePlayingMusic(const int music_number, Music& JazzMusic, Music& LofiMusic, Music& eSwingMusic);

	extern inline void end_attack(Deck& deck, MainGame& mg, Discardediscarded_cards& bPile, Table& table, std::vector<std::shared_ptr<Card>>& cardsVisible, std::array<Player, 2>& players);

	extern inline void takeDefender(std::array<Player, 2>& players, std::vector<std::shared_ptr<Card>>& cardsVisible, Deck& deck, Table& table, MainGame& mg);

	extern inline bool pirivadnoy_checker(Table& table, const std::shared_ptr<Card>& card);

	extern inline void privadi(MainGame& mg, std::array<Player, 2>& players);
}


