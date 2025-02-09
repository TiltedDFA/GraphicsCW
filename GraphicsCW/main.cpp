#include "AI.hpp"

int main()
{
	//--------------------------------------------------------------------------------------	
	InitWindow(screenWidth, screenHeight, "Durak");//This initlizes the screen with the width and size 

	InitAudioDevice();// This initilizes the audio device

	SetTargetFPS(90);//This sets the target framerate	

	//void SetMusicVolume(Music music, float volume);                               
	// Set volume for music (1.0 is max level)

	//---------------------------------------------------------------------------------------
	Rectangle playButton = { 773, 509, 287, 105 };

	Rectangle settingsButton = { 773, 754, 287, 105 };

	constexpr Vector2 top_card_pos = { 153, 470 };

	Vector2 mP = { 0.0f,0.0f }; //This is to store the mouse position

	std::array<std::pair<bool, std::string>, 4> screens;
	
	screens[0].first = true; screens[0].second = "Title Screen";

	screens[1].first = false; screens[1].second = "Playing Screen";

	screens[2].first = false; screens[2].second = "Win Screen";

	screens[3].first = false; screens[3].second = "Settings Screen";

	bool hC = false; //This variable is used primarily in the system that controlls the amnt of cards you pick up 	
					 //bool winScreen = false; //This is also for the system 	
	int musicalNumber = c3::getMusicNumber();

	bool pirivadnoy = c3::get_pirivadnoy_state();
	
	Deck deck(c3::getDeckSize());
	
	Table table;
	
	Discardediscarded_cards bPile;
	
	MainGame mainGame;
	
	std::array<Player, 2>players;
	
	std::vector<std::shared_ptr<Card>>cardsVisible;

	//---------------------------------------------------------------------------------------
	//These load in the visual and auditory assets
	Sound fxButton = LoadSound("buttonfx.wav");

	Texture2D blankCard = LoadTexture("170pixelBlank.png");

	Texture2D TitleScreen = LoadTexture("TitleScreen.png");

	Texture2D SettingScreen = LoadTexture("Settings.png");

	Texture2D progTable = LoadTexture("TableWithButton.png");

	Texture2D CardBacking = LoadTexture("170CardBacking.png");

	Texture2D PassButtonHigh = LoadTexture("PassHigh.png");

	Texture2D PassButtonMid = LoadTexture("PassMid.png");

	Texture2D PassButtonLow = LoadTexture("PassLow.png");

	Texture2D EndButtonHigh = LoadTexture("EndHigh.png");

	Texture2D EndButtonMid = LoadTexture("EndMid.png");

	Texture2D EndButtonLow = LoadTexture("EndLow.png");

	Texture2D TakeButtonHigh = LoadTexture("TakeHigh.png");

	Texture2D TakeButtonMid = LoadTexture("TakeMid.png");

	Texture2D TakeButtonLow = LoadTexture("TakeLow.png");

	Texture2D AtkHigh = LoadTexture("AtkHigh.png");

	Texture2D AtkLow = LoadTexture("AtkLow.png");

	Texture2D DefHigh = LoadTexture("DefHigh.png");

	Texture2D DefLow = LoadTexture("DefLow.png");

	Music JazzMusic = LoadMusicStream("JazzMusic.mp3");

	Music LoFiMusic = LoadMusicStream("LoFiMusic.mp3");

	Music ElectroSwingMusic = LoadMusicStream("ElectroSwingMusic.mp3");
	
	Image appIcon = LoadImage("appIcon.png");	

	//---------------------------------------------------------------------------------------

	SetWindowIcon(appIcon);

	c2::play_music(musicalNumber, JazzMusic, LoFiMusic, ElectroSwingMusic); 
	// This function begins playing the music which is selected by the musical number

	c1::init_player_hands(cardsVisible, deck, players);
	/*this function draws 12 cards from the deck, distrobutes those cards to the players and
	assigns them their right position and orientation.*/

	deck.setPosTopCardDeck(top_card_pos);
	//This sets up the position of the top card of the deck

	if (c1::findStartingPlayer(players) == 0) {players[0].setPlyrAtk(true); mainGame.setPTurn(0); }

	else { players[1].setPlyrAtk(true); mainGame.setPTurn(1); }
	/*This checks which player should be the starting player and then it changes the player states
	as well as the player turn variable to whichever is the correct set up*/

	deck.setmaster_suit(static_cast<card_suit>(encrypt_data(deck.getLastCard()->get_card_suit())));
	//This sets the master suit for the game based off of the last card in the gamee

	//--------------------------------------------------------------------------------------
	// Main game loop
	while (!WindowShouldClose())
	{
		c2::continuePlayingMusic(musicalNumber, JazzMusic, LoFiMusic, ElectroSwingMusic); 
		//used to play the next segment of the music selected at the start music

		mP = GetMousePosition();
		//gets the mouse position and updates the variable

		hC = false;
		//this is the main variable used to ensure the user only picks up one card

		if (screens[1].first) // Main Game Screen;
		{
			if (c2::check_for_winner(deck, players, table) != 2) {
				players[c2::check_for_winner(deck, players, table)].set_player_winstate(true); screens[1].first = !screens[1].first; screens[2].first = !screens[2].first; }
			//This checks if someone has won

			c1::lockCardsInHand(players, mainGame);
			/*This stops the players from being able to use the
			other's cards. With the ai it allows the AI to use it's 
			cards when it is it's turn.*/

			BeginDrawing();

			ClearBackground(RAYWHITE);

			DrawTexture(progTable, 0, 0, WHITE);

			c0::displayPassButtons(players, table, mainGame, PassButtonLow, PassButtonMid, PassButtonHigh, fxButton);
			/*
			This draws the buttons used to pass
			*/

			if (players[mainGame.getPTurn()].isPlyrAtk()) {
				c0::displayEndButtons(players, mainGame, EndButtonLow, EndButtonMid, EndButtonHigh, fxButton, bPile, table, cardsVisible, deck); 
			}
			else { 
				c0::displayTakeButtons(deck, cardsVisible, players, mainGame, table, TakeButtonHigh, TakeButtonMid, TakeButtonLow);
			}
			/*
			This displays either a "Take" or "End" button depending on whether the user is currently attacking or defending
			*/


			c0::displayPlayerState(AtkHigh, AtkLow, DefHigh, DefLow, players[1].isPlyrAtk());
			//This displays the player's state in the top right of the game screen

			c0::cTable(cardsVisible, CardBacking, blankCard);
			//This displays every card within the cards visible vector


			if (deck.getDeckSize()) { c0::displaySpecialCards(deck, CardBacking, blankCard); }
			//This displays the last card if the decksize is >= 1 

			c0::displayWhosTurnItIs(mainGame,players[mainGame.getPTurn()].isPlyrAtk());
			//This displays whos turn it is

			DrawCircleV(GetMousePosition(), 10, WHITE);
			//This draws a circle around the cursor for better contrast on the background

			EndDrawing();

			for (int i = 0; i < cardsVisible.size(); ++i)
			//This loop iterates through ever card within cardsVisible
			{
				if (!mainGame.getPTurn() && players[0].isPlyrAtk()) { //If turn = 0 (ai turn)

					c4::attack(table, deck, mainGame, bPile, cardsVisible, players);
					//This instructs the ai to attack if it is player 0's turn and player 0 is attacking
				}
				else if (!mainGame.getPTurn() && !players[0].isPlyrAtk()) {

					c4::defend(players, table, mainGame, cardsVisible, deck);
					//This instructs the ai to defend if it is player 0's turn and player 0 is defending
				}
				else
				{

					if (cardsVisible[i]->canBeTouched)
						/*
						Will move onto the next card if the card is unable to be touched by the user
						to not process unecessary information
						*/
					{
						Rectangle card = { cardsVisible[i]->card_position.x, cardsVisible[i]->card_position.y, 120, 170 };

						if (!hC)
							//If the user is not holding a card
						{
							if (CheckCollisionPointRec(mP, card) && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {

								hC = true;

								cardsVisible[i]->is_card_face_up = !cardsVisible[i]->is_card_face_up; 
								//This will register the user as holding a card if they're
								//hovering over it and right clicking on it

							}

							if (CheckCollisionPointRec(mP, card) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

								hC = true;

								cardsVisible[i]->cardIsHeld = true;

								Vector2 mD = GetMouseDelta();

								cardsVisible[i]->card_position.x = (cardsVisible[i]->card_position.x + mD.x);

								cardsVisible[i]->card_position.y = (cardsVisible[i]->card_position.y + mD.y);
								/*This will register the user holding the card if their cursor is over the card
								and they are holding down the left mouse button. It will also move the card based
								on how far the user moved their cursor from the last pass of this loop*/

							}
							if (!CheckCollisionPointRec(mP, card) && !IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

								cardsVisible[i]->cardIsHeld = false;
							}
						}

						if (!cardsVisible[i]->cardIsHeld && !cardsVisible[i]->inDefTablePile) {

							const auto overlap = c2::collision_percent_finder(50, cardsVisible[i]);
							/*
							* This checks whether a card is colliding with a potential placement
							* box and if it is, it will calculate the percantage of the card 
							* which is overlaping with the placement box
							*/

							if (overlap >= 50) {

								auto box = c2::find_colliding_placement(cardsVisible[i]);
								// This functions finds which placement box the card is colliding with.

								cardsVisible[i]->card_position = box.first;
								/*
								* This sets the card's position to be the same as the position of the 
								* placement box which it is overlapping with
								*/

								if (players[mainGame.getPTurn()].isPlyrAtk() && table.getCardFromTableAtk(box.second) == nullptr) {
									/*
									* This checks whether the "defending pile" from the card placement slot 
									* which the card is currently over is empty.
									*/

									if (overlap == 100)
									{

										if (c2::can_attacker_attack(table, cardsVisible[i]) && table.getCardFromTableAtk(box.second) == nullptr) {
											//This checks whether the attack is able to legally attack

											cardsVisible[i]->canBeTouched = false;

											c2::hand_to_table(players[mainGame.getPTurn()], table, cardsVisible[i], box.second);
											/*This moves the card which is currently overlapping the placement box 
											* and places it into the table vector
											*/

											mainGame.incramentCardsPlayed();
										}

										else {

											cardsVisible[i]->card_position = { box.first.x , (box.first.y + 250.0f) };
										}
									}
								}
								else if (!players[mainGame.getPTurn()].isPlyrAtk()) {
									//This checks if the player is defending

									if (pirivadnoy && c2::pirivadnoy_checker(table, cardsVisible[i])) {
										/*
										* This checks whether pirivandoy has been enabled 
										* and whether the the player would be able to 
										* use pirivadnoy
										*/

										c2::privadi(mainGame, players);
										//This carries out the action of pirivadit

										cardsVisible[i]->card_position = Vector2Add(cardsVisible[i]->card_position, { 40,40 });

										cardsVisible[i]->canBeTouched = false;
									}
									else if (c2::card_beats_card_def(cardsVisible[i], table.getCardFromTableAtk(box.second))) {
										//This checks whether the card placed is able to defend against the card underneath it 

										cardsVisible[i]->card_position = Vector2Add(cardsVisible[i]->card_position, { 40,40 });

										cardsVisible[i]->canBeTouched = false;

										c2::hand_to_table(players[mainGame.getPTurn()], table, cardsVisible[i], box.second);

										c1::bringCardOneToTop(cardsVisible[i], table.getCardFromTableAtk(box.second), cardsVisible);
									}
									else {

										cardsVisible[i]->card_position = { box.first.x , (box.first.y + 250.0f) };
										/*
										* This shifts the cards position under the placement box if the card
										is unable to cover it
										*/
									}
								}
								else
								{
									cardsVisible[i]->card_position = { box.first.x , (box.first.y + 250.0f) };
								}
							}
						}
					}
				}
			}
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
				hC = false;
		}
		else if (screens[0].first) { 
		// Title Screen
	
			BeginDrawing();

			ClearBackground(RAYWHITE);

			DrawTexture(TitleScreen, 0, 0, WHITE);

			DrawCircleV(GetMousePosition(), 10, WHITE);

			EndDrawing();

			if (CheckCollisionPointRec(mP, playButton) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))	{

				screens[1].first = true;

				screens[0].first = false;				
			}
			if (CheckCollisionPointRec(mP, settingsButton) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {				

				screens[3].first = true;

				screens[0].first = false;
				
			}
		}
		//---------------------------------------------------------------------------------
		else if (screens[2].first) { //Win screen

			std::string win;

			if (players[0].get_player_winstate())
				win = "The Bot Wins";

			else
				win = "Well Done, You win!";

			BeginDrawing();

			ClearBackground(RAYWHITE);

			DrawRectangle(0, 0, 1920, 1080, BLUE);

			DrawText(win.c_str(), (screenWidth / 2), screenHeight / 2, 50, WHITE);

			EndDrawing();
		}
		//---------------------------------------------------------------------------------
		else if (screens[3].first) {// Settings Screen;

			const int deckSize = c3::getDeckSize(); 
			// This reads the current deckSize from the file

			const int musicSelection = c3::getMusicNumber(); 
			//This reads the current music selection

			//---------------------------------------------

			Rectangle ThirtySix = { 549, 379, 400, 86 };

			Rectangle FiftyTwo = { 963,378, 400, 86 };

			Rectangle ReturnButton = { 36, 948, 288,90 };

			//---------------------------------------------

			Rectangle NoMusic = { 547, 484, 206, 86 };

			Rectangle JMusic = { 755, 484, 205, 86 };

			Rectangle LMusic = { 961, 484, 206, 86 };

			Rectangle eSwingMusic = { 1169, 484, 205, 86 };

			BeginDrawing();

			ClearBackground(RAYWHITE);

			DrawTexture(SettingScreen, 0, 0, WHITE);

			switch (deckSize) {

			case 36:

				c0::displayDeckSizeButtons(ThirtySix, FiftyTwo, true, musicalNumber, pirivadnoy);

				break;

			case 52:

				c0::displayDeckSizeButtons(ThirtySix, FiftyTwo, false, musicalNumber, pirivadnoy);

				break;

			default:

				throw std::runtime_error("Unexpected DeckSize");
			}

			c0::displayMusicButtons(NoMusic, JMusic, LMusic, eSwingMusic, JazzMusic, LoFiMusic, ElectroSwingMusic, musicSelection, deckSize, musicalNumber, pirivadnoy);

			c0::display_pirivadnoy_buttons(musicalNumber, deckSize, pirivadnoy);

			if (CheckCollisionPointRec(mP, ReturnButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

				screens[3].first = false;

				screens[0].first = true;
			}
			EndDrawing();
		}
	}
	// De-Initialization
	//--------------------------------------------------------------------------------------
	UnloadSound(fxButton);
	UnloadTexture(blankCard);
	UnloadTexture(TitleScreen);
	UnloadTexture(SettingScreen);
	UnloadTexture(progTable);
	UnloadTexture(CardBacking);
	UnloadTexture(PassButtonHigh);
	UnloadTexture(PassButtonMid);
	UnloadTexture(PassButtonLow);	
	UnloadTexture(EndButtonHigh);
	UnloadTexture(EndButtonMid);
	UnloadTexture(EndButtonLow);
	UnloadTexture(TakeButtonHigh);
	UnloadTexture(TakeButtonMid);
	UnloadTexture(TakeButtonLow);
	UnloadTexture(AtkHigh);
	UnloadTexture(AtkLow);
	UnloadTexture(DefHigh);
	UnloadTexture(DefLow);
	UnloadMusicStream(JazzMusic);
	UnloadMusicStream(LoFiMusic);
	UnloadMusicStream(ElectroSwingMusic);
	UnloadImage(appIcon);
	CloseAudioDevice();
	CloseWindow();// Close window and OpenGL context
	//--------------------------------------------------------------------------------------
	system("pause>0");
	return 0;
}