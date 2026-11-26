// main.cpp - the locker heist
// driver file: gets the one game engine, runs the main menu and the game loop,
// and catches every game exception that gets thrown during the game
#include "heist.h"

int main()
{
    // seed rand() once so a different culprit gets picked every run
    srand(static_cast<unsigned int>(time(nullptr)));

    // the leader board lives here, the engine only gets a pointer to it (association)
    LeaderBoard leaderBoard(LEADERBOARD_FILE);

    // singleton: there is only ever one game engine
    GameEngine* engine = GameEngine::getInstance();
    engine->setLeaderBoard(&leaderBoard);

    engine->showTitle();

    while (engine->getState() != EXITED)
    {
        try
        {
            engine->showMainMenu();
            int choice = readNumber("  Choice: ");

            switch (static_cast<MenuChoice>(choice))
            {
            case NEW_GAME:
                engine->startNewGame();
                engine->setState(MENU);
                break;

            case LEADER_BOARD:
                engine->showLeaderBoard();
                pauseScreen();
                clearScreen();
                break;

            case ABOUT:
                engine->showAbout();
                pauseScreen();
                clearScreen();
                break;

            case RULES:
                engine->showRules();
                pauseScreen();
                clearScreen();
                break;

            case EXIT_GAME:
                engine->setState(EXITED);
                cout << "\n  Case closed. Goodbye, Detective!\n";
                break;

            default:
                throw InvalidInputException("Main Menu options are 1 to 5 only.");
            }
        }
        catch (const GameException& error)
        {
            // errors outside a game (bad menu input, missing files) just show up
            // and the player goes back to the main menu
            cout << "\n  " << error.getMessage() << "\n";
            engine->setState(MENU);
        }
    }

    // free the engine, which also deletes every character it made
    GameEngine::destroyInstance();
    return 0;
}