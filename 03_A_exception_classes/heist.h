// heist.h - the locker heist
// all the structs, enums, exceptions, the template and the class declarations are here.
// the function bodies are in heist.cpp and main.cpp runs the game
#ifndef HEIST_H
#define HEIST_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// names of all the text files in one place, so renaming a file is a one line change
const string CLUE_FILE        = "clue.txt";
const string LOCATIONS_FILE   = "locations.txt";
const string CHARACTERS_FILE  = "characters.txt";
const string RULES_FILE       = "rules.txt";
const string ABOUT_FILE       = "about.txt";
const string LEADERBOARD_FILE = "leaderboard.txt";

// number of culprit profiles (lines) in clue.txt
const int TOTAL_PROFILES = 7;

// width of the boxes we draw on screen
const int BOX_WIDTH = 60;

// ===== enums =====
// enums give names to a small fixed set of values, so we write WORD_2 or PLAYING
// instead of magic numbers like 1 or 2
// which row of the board a letter goes to. word 1 comes from rooms, word 2 from people
enum WordRow
{
    WORD_1 = 0,
    WORD_2 = 1
};

// state of the game, main() checks it to know which loop should be running
enum GameState
{
    MENU,
    PLAYING,
    WON,
    LOST,
    EXITED
};

// main menu options, starts at 1 so it matches what the player types
enum MenuChoice
{
    NEW_GAME = 1,
    LEADER_BOARD,
    ABOUT,
    RULES,
    EXIT_GAME
};

// ===== exceptions (module 7) =====
// base class for every error in the game. all other exceptions inherit from it,
// so main() can catch every one of them with a single catch block
class GameException
{
protected:
    string message;

public:
    GameException(const string& errorMessage = "Something went wrong.");

    virtual ~GameException();

    string getMessage() const;

    virtual string getType() const;

    // true if the error was the player's mistake, then it costs credibility
    virtual bool countsAsWrongStep() const;
};

// the classes below only change the label shown in front of the message
class InvalidInputException : public GameException
{
public:
    InvalidInputException(const string& errorMessage);

    string getType() const override;
};

class InvalidPositionException : public GameException
{
public:
    InvalidPositionException(const string& errorMessage);

    string getType() const override;
};

class SlotFilledException : public GameException
{
public:
    SlotFilledException(const string& errorMessage);

    string getType() const override;
};

class RackFullException : public GameException
{
public:
    RackFullException(const string& errorMessage);

    string getType() const override;
};

class RackEmptyException : public GameException
{
public:
    RackEmptyException(const string& errorMessage);

    string getType() const override;
};

class InvalidWordException : public GameException
{
public:
    InvalidWordException(const string& errorMessage);

    string getType() const override;
};

class IncompleteWordException : public GameException
{
public:
    IncompleteWordException(const string& errorMessage);

    string getType() const override;
};

class LockedCharacterException : public GameException
{
public:
    LockedCharacterException(const string& errorMessage);

    string getType() const override;
};

class WrongAccusationException : public GameException
{
public:
    WrongAccusationException(const string& errorMessage);

    string getType() const override;
};

class FileException : public GameException
{
public:
    FileException(const string& errorMessage);

    string getType() const override;

    // a missing or broken file isn't the player's fault, so no penalty
    bool countsAsWrongStep() const override;
};

// ===== helper functions =====
// stl isn't allowed, so these small helpers replace things like stoi and to_string
string intToString(int value);

string twoDigits(int value);

bool stringToInt(const string& text, int& result);

string trim(const string& text);

string toUpperCase(const string& text);

char toUpperChar(char letter);

string replaceChar(const string& text, char from, char to);

string padRight(const string& text, int width);

string readLine();

int readNumber(const string& prompt);

bool readYesNo(const string& prompt);

void clearScreen();

void pauseScreen();

void printBoxBorder();
void printBoxDivider();
void printBoxLine(const string& text, bool centered = false);

// ===== structs (module 1) =====
// structs keep related values together so we can pass them around as one thing,
// which makes the code more reusable
// tile = one letter square, on the board or inside a rack
struct Tile
{
    char tileLetter;
    bool filled;

    Tile();

    Tile(char letter);

    // == compares letters, used to check board letters against the words in clue.txt
    bool operator==(const Tile& other) const;

    bool operator==(char letter) const;

    bool operator!=(char letter) const;

    // << prints [A] or [ ] and >> reads one letter
    friend ostream& operator<<(ostream& out, const Tile& tile);

    friend istream& operator>>(istream& in, Tile& tile);
};

// a clue just carries a letter + position from a room or a person to the board
struct Clue
{
    char    clueLetter;
    int     cluePosition;
    WordRow clueRow;

    Clue();

    Clue(char letter, int position, WordRow row);

    bool operator==(const Clue& other) const;
};

// one room of the cottage, it gives one letter for word 1
struct Location
{
    string  locName;
    char    letterClueLoc;
    int     boardPosLoc;
    WordRow locRow;
    bool    locSearchFlag;

    Location();

    Location(const string& name, char letter, int position);

    friend ostream& operator<<(ostream& out, const Location& location);

    friend istream& operator>>(istream& in, Location& location);
};

#endif
