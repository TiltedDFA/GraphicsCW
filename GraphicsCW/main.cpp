#include "Display.hpp"

int main()
{
	//--------------------------------------------------------------------------------------	
	InitWindow(screenWidth, screenHeight, "Durak");//This initlizes the screen with the width and size 

	InitAudioDevice();// This initilizes the audio device

	SetTargetFPS(120);//This sets the target framerate	

	//---------------------------------------------------------------------------------------

	Vector2 mP = {0.0f,0.0f}; //This is to store the mouse position 

	Vector2 dMP = { 0.0f, 0.0f }; //This is to store the delta of the position

	std::array<std::pair<bool, std::string>, 4> screens;

	screens[0].first = true; screens[0].second = "Title Screen";

	screens[1].first = false; screens[1].second = "Playing Screen";

	screens[2].first = false; screens[2].second = "Win Screen";

	screens[3].first = false; screens[3].second = "Settings Screen";

	//bool _TScreen = true; // This is to help the store which screen to display
	//bool prePlayScreen = false; //This is also to help with the screen display system

	bool hC = false; //This variable is used primarily in the system that controlls the amnt of cards you pick up 	
					 //bool winScreen = false; //This is also for the system 
	
	int winner = 2;
	
	int musicalNumber = c3::getMusicNumber();
	
	Deck deck(c3::getDeckSize());
	
	Table table;
	
	DiscardedCards bPile;
	
	MainGame mainGame;
	
	std::array<Player, 2>players;
	
	std::vector<std::shared_ptr<Card>>cardsVisible;
	
	//---------------------------------------------------------------------------------------
	
	Rectangle playButton = { 773, 509, 287, 105 };
	
	Rectangle settingsButton = { 773, 754, 287, 105 };
	
	//---------------------------------------------------------------------------------------
	Image appIcon = LoadImage("appIcon.png");

	SetWindowIcon(appIcon);
	
	Sound fxButton = LoadSound("buttonfx.wav");
	
	Texture2D blankCard = LoadTexture("170pixelBlank.png");

	Texture2D TitleScreen = LoadTexture("TitleScreen.png");
	
	Texture2D SettingScreen = LoadTexture("Settings.png");

	Texture2D  _Table = LoadTexture("TableCardHole.png");
	
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
	
	srand(time(NULL));

	c2::playMusic(musicalNumber, JazzMusic, LoFiMusic, ElectroSwingMusic);

	//--------------------------------------------------------------------------------------
	// Main game loop


	//Code something that moves the cards when the mouse moves them. The co-ordinates of the cards will be the mouse co-ordinates + the offset of where the mouse clicks the card	
	c1::setUpPlayerHandPos(cardsVisible, deck, players);

	deck.setPosTopCardDeck({ 153, 470 });

	if (c1::findStartingPlayer(players, deck) == 0) { players[0].setPlyrAtk(true); mainGame.setPTurn(0); }
	
	else { players[1].setPlyrAtk(true); mainGame.setPTurn(1); }
	deck.setMasterSuit(deck.getLastCard()->Suit);
	while (!WindowShouldClose())
	{
		c2::continuePlayingMusic(musicalNumber, JazzMusic, LoFiMusic, ElectroSwingMusic);
		mP = GetMousePosition();
		hC = false;
		if (screens[1].first) // Main Game Screen;
		{
			if (c2::checkIfPlayersWon(deck, players, table) != 2) { winner = c2::checkIfPlayersWon(deck, players, table); screens[1].first = !screens[1].first; screens[2].first = !screens[2].first; }
			
			c1::lockCardsInHand(players, mainGame);

			BeginDrawing();

			ClearBackground(RAYWHITE);

			DrawTexture(progTable, 0, 0, WHITE);

			c0::displayPassButtons(players, table, mainGame, PassButtonLow, PassButtonMid, PassButtonHigh, fxButton);	

			if (players[mainGame.getPTurn()].isPlyrAtk()) { c0::displayEndButtons(players, mainGame, EndButtonLow, EndButtonMid, EndButtonHigh, fxButton, bPile, table, cardsVisible, deck); }

			else { c0::displayTakeButtons(deck, cardsVisible, players, mainGame, table, TakeButtonHigh, TakeButtonMid, TakeButtonLow); }			

			c0::displayPlayerState(AtkHigh, AtkLow, DefHigh, DefLow, players[1].isPlyrAtk());

			c0::cTable(cardsVisible, CardBacking, blankCard);			

			if (deck.getDeckSize()) { c0::displaySpecialCards(deck, CardBacking,blankCard); }

			c0::displayWhosTurnItIs(mainGame);

			DrawCircleV(GetMousePosition(), 10, WHITE);

			EndDrawing();			



			for (int i = 0; i < cardsVisible.size(); ++i)
			{
				if (cardsVisible[i]->canBeTouched)
				{
					Rectangle card = { cardsVisible[i]->cardPosition.x, cardsVisible[i]->cardPosition.y, 120, 170 };

					if (!hC)
					{
						if (CheckCollisionPointRec(mP, card) && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
						
							hC = true;
					
							cardsVisible[i]->cardIsFaceUp = !cardsVisible[i]->cardIsFaceUp; // this function changes the state of the card in the cardsVisible pointer vector
						
						}
					
						if (CheckCollisionPointRec(mP, card) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
					
							hC = true;
						
							cardsVisible[i]->cardIsHeld = true;
							
							Vector2 mD = GetMouseDelta();
							
							cardsVisible[i]->cardPosition.x = (cardsVisible[i]->cardPosition.x + mD.x);
							
							cardsVisible[i]->cardPosition.y = (cardsVisible[i]->cardPosition.y + mD.y);
						
						}
						if (!CheckCollisionPointRec(mP, card) && !IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
						
							cardsVisible[i]->cardIsHeld = false;					
						}
					}					
					if (!cardsVisible[i]->cardIsHeld && !cardsVisible[i]->inDefTablePile) {
						
						int overlap = c2::placeHBC(50, cardsVisible[i]);
					
						if (overlap >= 50) {
							
							auto box = c2::BoxColFinder(cardsVisible[i]);
							
							cardsVisible[i]->cardPosition = box.first;
						
							if (players[mainGame.getPTurn()].isPlyrAtk() && table.getCardFromTableAtk(box.second) == nullptr) {
						
								if (overlap == 100)
								{
						
									if (c2::canCardBePlayed(table, cardsVisible[i]) && table.getCardFromTableAtk(box.second) == nullptr) {																			
								
										cardsVisible[i]->canBeTouched = false;
										
										c2::moveCardFromPlayerHandToTable(players[mainGame.getPTurn()], table, cardsVisible[i], box.second);
										
										mainGame.incramentCardsPlayed();									
									}
									
									else {										
									
										cardsVisible[i]->cardPosition = { box.first.x , (box.first.y + 250.0f) };
									}
								}							
							}
							else if (!players[mainGame.getPTurn()].isPlyrAtk()) {
								
								if (c2::cardBeatsCard(cardsVisible[i], table.getCardFromTableAtk(box.second), deck)) {									
							
									cardsVisible[i]->cardPosition = Vector2Add(cardsVisible[i]->cardPosition, { 40,40 });
						
									cardsVisible[i]->canBeTouched = false;
						
									c2::moveCardFromPlayerHandToTable(players[mainGame.getPTurn()], table, cardsVisible[i], box.second);
						
									c1::bringCardOneToTop(cardsVisible[i], table.getCardFromTableAtk(box.second), cardsVisible);
								}
								else {			
								
									cardsVisible[i]->cardPosition = { box.first.x , (box.first.y + 250.0f) };
								}
							}
						}
					}
				}
			}
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
				hC = false;
		}
		else if (screens[0].first) { // Title Screen
		
			BeginDrawing();
	
			ClearBackground(RAYWHITE);

			DrawTexture(TitleScreen, 0, 0, WHITE);
			
			DrawCircleV(GetMousePosition(), 10, WHITE);
		
			EndDrawing();
		
			if (CheckCollisionPointRec(mP, playButton)) {
	
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		
					screens[1].first = true;
	
					screens[0].first = false;
				}
			}
			if (CheckCollisionPointRec(mP, settingsButton))	{
			
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
				
					screens[3].first = true;
					
					screens[0].first = false;
				}
			}
		}
		//---------------------------------------------------------------------------------
		else if (screens[2].first) { //Win screen
	
			std::string win;
		
			switch (winner) {
		
			case 0:
			
				win = "The Bot Wins";
				
				break;
		
			case 1:
			
				win = "Well Done, You win!";
				
				break;
			
			default:
		
				throw std::runtime_error("Unexpected Winstate");
				
				break;
			}
		
		BeginDrawing();
	
		ClearBackground(RAYWHITE);
	
		DrawText(win.c_str(), (screenWidth / 2), screenHeight / 2, 50, WHITE);
		
		EndDrawing();
		}
		//---------------------------------------------------------------------------------
		else if (screens[3].first) {// Settings Screen;
	
		Vector2 mP = GetMousePosition();
	
		const int deckSize = c3::getDeckSize(); // This reads the current deckSize from the file
		
		const int musicSelection = c3::getMusicNumber();
		
		//---------------------------------------------
		
		Rectangle ThirtySix = { 549, 379, 400, 86 };
		
		Rectangle FiftyTwo = { 963,378, 400, 86 };
		
		Rectangle ReturnButton = { 36, 948, 288,90 };
		
		//---------------------------------------------
		
		Rectangle NoMusic = { 547, 484, 205, 86 };
		
		Rectangle JMusic = { 755, 484, 205, 86};
		
		Rectangle LMusic = { 961, 484, 205, 86};
		
		Rectangle eSwingMusic = { 1169, 484, 205, 86};
		
		BeginDrawing();
		
		ClearBackground(RAYWHITE);
		
		DrawTexture(SettingScreen, 0, 0, WHITE);
		
		switch (deckSize) {

		case 36:

			c0::displayDeckSizeButtons(ThirtySix, FiftyTwo, true, musicalNumber);

			break;

		case 52:

			c0::displayDeckSizeButtons(ThirtySix, FiftyTwo, false, musicalNumber);

			break;

		default:

			throw std::runtime_error("Unexpected DeckSize");

			break;
		}		

		c0::displayMusicButtons(NoMusic, JMusic, LMusic, eSwingMusic, JazzMusic, LoFiMusic, ElectroSwingMusic, musicSelection, deckSize, musicalNumber);

		if(CheckCollisionPointRec(mP, ReturnButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		
			screens[3].first = false;
			
			screens[0].first = true;
		}
		EndDrawing();
		} 
	}
	// 36, 948
	// De-Initialization
	//--------------------------------------------------------------------------------------
	UnloadTexture(blankCard);
	UnloadTexture(CardBacking);
	UnloadTexture(TitleScreen);
	UnloadTexture(_Table);
	UnloadTexture(progTable);
	UnloadTexture(SettingScreen);
	UnloadTexture(PassButtonHigh);
	UnloadTexture(PassButtonMid);
	UnloadTexture(PassButtonLow);
	UnloadTexture(EndButtonHigh);
	UnloadTexture(EndButtonMid);
	UnloadTexture(EndButtonLow);
	UnloadTexture(AtkHigh);
	UnloadTexture(AtkLow);
	UnloadTexture(DefHigh);
	UnloadTexture(DefLow);
	UnloadTexture(TakeButtonHigh);
	UnloadTexture(TakeButtonMid);
	UnloadTexture(TakeButtonLow);
	UnloadSound(fxButton);
	CloseAudioDevice();
	CloseWindow();// Close window and OpenGL context
	//--------------------------------------------------------------------------------------
	system("pause>0");
	return 0;
}