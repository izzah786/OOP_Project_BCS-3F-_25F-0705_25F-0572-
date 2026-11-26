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

// ===== template (module 8) =====
// our own growable array, written from scratch because stl vector isn't allowed.
// cottage, scrabble and tilerack keep their data in it, so they get deep copies
// and memory cleanup for free. templates stay in the header so the compiler sees the whole code
template <typename T>
class DynamicArray
{
private:
    T*  data;
    int count;
    int capacity;

    // doubles the space when the array is full and copies the old items over
    void growCapacity()
    {
        int newCapacity = (capacity <= 0) ? 4 : capacity * 2;
        T* biggerBlock = new T[newCapacity];

        for (int i = 0; i < count; i++)
        {
            biggerBlock[i] = data[i];
        }

        delete[] data;
        data = biggerBlock;
        capacity = newCapacity;
    }

public:
    explicit DynamicArray(int startCapacity = 4)
        : data(nullptr), count(0), capacity(startCapacity > 0 ? startCapacity : 4)
    {
        data = new T[capacity];
    }

    // copy constructor - deep copy, the new array gets its own memory
    DynamicArray(const DynamicArray<T>& other)
        : data(nullptr), count(other.count), capacity(other.capacity)
    {
        data = new T[capacity];
        for (int i = 0; i < count; i++)
        {
            data[i] = other.data[i];
        }
    }

    // assignment - also a deep copy, old memory is freed only after copying
    DynamicArray<T>& operator=(const DynamicArray<T>& other)
    {
        if (this != &other)
        {
            T* freshBlock = new T[other.capacity];
            for (int i = 0; i < other.count; i++)
            {
                freshBlock[i] = other.data[i];
            }

            delete[] data;
            data = freshBlock;
            count = other.count;
            capacity = other.capacity;
        }
        return *this;
    }

    // frees the memory so nothing leaks
    ~DynamicArray()
    {
        delete[] data;
        data = nullptr;
    }

    void add(const T& item)
    {
        if (count == capacity)
        {
            growCapacity();
        }
        data[count] = item;
        count++;
    }

    void removeLast()
    {
        if (count == 0)
        {
            throw InvalidPositionException("Cannot remove an element from an empty array.");
        }
        count--;
    }

    // [] with a range check, a bad index throws instead of reading garbage
    T& operator[](int index)
    {
        if (index < 0 || index >= count)
        {
            throw InvalidPositionException("Index " + intToString(index) + " is outside the array.");
        }
        return data[index];
    }

    const T& operator[](int index) const
    {
        if (index < 0 || index >= count)
        {
            throw InvalidPositionException("Index " + intToString(index) + " is outside the array.");
        }
        return data[index];
    }

    int getSize() const { return count; }

    int getCapacity() const { return capacity; }

    bool isEmpty() const { return count == 0; }

    void clear() { count = 0; }
};

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

// ===== tile rack (module 5) =====
// holds the letters the detective collected and the position of each one.
// the detective has two racks, one for word 1 and one for word 2
class TileRack
{
private:
    // old raw dma version, kept as the brief asks
    // Tile* rackTiles;       // new tile[maxtiles]
    // int*  rackPositions;   // new int[maxtiles]
    // int   tileCount;

    DynamicArray<Tile> tiles;
    DynamicArray<int>  positions;
    int    maxTiles;
    string rackName;

public:
    TileRack(const string& name = "Rack", int maximumTiles = 8);

    TileRack(const TileRack& other);

    TileRack& operator=(const TileRack& other);

    ~TileRack();

    // throws RackFullException if the rack is already full
    void addTile(const Tile& tile, int position);

    // throws RackEmptyException if there is nothing to remove
    Tile removeLastTile();

    void clearRack();

    bool isFull() const;
    bool isEmpty() const;
    int  getCount() const;
    int  getMaxTiles() const;

    string getLettersAsString() const;

    void display() const;

    // + merges two racks with a space in between, giving one combined clue
    TileRack operator+(const TileRack& other) const;
};

// ===== characters (modules 3 and 9) =====
// character (abstract)
// |-- victim (business owner)
// |-- detective (player)
// `-- suspect (abstract)
//     |-- businessrelations  -> partner, pa
//     |-- househelp          -> cook, guard, servant
//     `-- closerelations     -> ownerschild, pa
// pa comes from both businessrelations and closerelations (the diamond problem),
// virtual inheritance fixes it so pa only gets one suspect part
// forward declaration so the engine can be made a friend below
class GameEngine;

// abstract parent of everyone in the game
class Character
{
    // the engine is a friend class so it can mark the culprit directly
    friend class GameEngine;

protected:
    string characterName;
    string characterRole;
    string nationality;
    bool   isCulprit;
    bool   interviewed;

public:
    Character(const string& name, const string& role, const string& nation);

    // virtual destructor so deleting through a character pointer calls the right one
    virtual ~Character();

    string getName() const;
    string getRole() const;
    string getNationality() const;
    bool   getIsCulprit() const;
    bool   isInterviewed() const;

    void setName(const string& name);
    void setNationality(const string& nation);
    void setInterviewed(bool value);

    // pure virtual, this is what makes character abstract.
    // every person overrides it and prints their own statement
    virtual void display() const = 0;

    // every person keeps and gives back their own word 2 letter
    virtual void setClue(const Clue& clue) = 0;

    virtual Clue getClue() const = 0;

    // how much credibility someone needs before they agree to talk, each role overrides it
    virtual int getRequiredCredibility() const;

    virtual bool isSuspect() const;

    bool canBeInterviewed(int credibility) const;

    // << prints "name (role)" and >> reads a new name and nationality
    friend ostream& operator<<(ostream& out, const Character& person);

    friend istream& operator>>(istream& in, Character& person);
};

// the french business owner. can be interviewed (needs 80 credibility) but can't be accused
class Victim : public Character
{
private:
    WordRow clueRowChar;
    char    letterClueChar;
    int     boardPosChar;

public:
    Victim(const string& name, const string& nation);
    ~Victim();

    void display() const override;
    void setClue(const Clue& clue) override;
    Clue getClue() const override;
    int  getRequiredCredibility() const override;
};

// the player. the engine increases the turns, the detective only stores them.
// the two racks are composition, they live and die with the detective
class Detective : public Character
{
private:
    int      credibility;
    int      turnsUsed;
    TileRack locationRack;
    TileRack characterRack;

public:
    Detective(const string& name = "Detective", int startCredibility = 100);
    Detective(const Detective& other);
    Detective& operator=(const Detective& other);
    ~Detective();

    void resetForNewGame(const string& name, int startCredibility);

    int  getCredibility() const;
    void setCredibility(int value);
    void changeCredibility(int amount);

    int  getTurns() const;
    void setTurns(int turns);

    void collectLetter(const Clue& clue);

    const TileRack& getLocationRack() const;
    const TileRack& getCharacterRack() const;

    void display() const override;
    void setClue(const Clue& clue) override;
    Clue getClue() const override;
};

// abstract parent of the 7 people who could be the culprit
class Suspect : public Character
{
public:
    Suspect(const string& name, const string& role, const string& nation);
    virtual ~Suspect();

    bool isSuspect() const override;
};

// virtual inheritance here and in closerelations is what solves the diamond for pa.
// business people need 50 credibility
class BusinessRelations : virtual public Suspect
{
public:
    BusinessRelations(const string& name, const string& role, const string& nation);
    virtual ~BusinessRelations();

    int getRequiredCredibility() const override;
};

// cook, guard and servant talk any time, so the base rule (0) is kept
class HouseHelp : public Suspect
{
public:
    HouseHelp(const string& name, const string& role, const string& nation);
    virtual ~HouseHelp();
};

// family, needs 75 credibility
class CloseRelations : virtual public Suspect
{
public:
    CloseRelations(const string& name, const string& role, const string& nation);
    virtual ~CloseRelations();

    int getRequiredCredibility() const override;
};

// the real suspects. each one keeps its own private word 2 clue (row, letter, position)
class Partner : public BusinessRelations
{
private:
    int     partnerNumber;
    WordRow clueRowChar;
    char    letterClueChar;
    int     boardPosChar;

public:
    Partner(const string& name, const string& nation, int number);
    ~Partner();

    int  getPartnerNumber() const;
    void display() const override;
    void setClue(const Clue& clue) override;
    Clue getClue() const override;
};

// pa inherits from both sides, this is the diamond.
// both parents have a credibility rule, so pa has to give its own (35)
class PA : public BusinessRelations, public CloseRelations
{
private:
    WordRow clueRowChar;
    char    letterClueChar;
    int     boardPosChar;

public:
    PA(const string& name, const string& nation);
    ~PA();

    int  getRequiredCredibility() const override;
    void display() const override;
    void setClue(const Clue& clue) override;
    Clue getClue() const override;
};

class Cook : public HouseHelp
{
private:
    WordRow clueRowChar;
    char    letterClueChar;
    int     boardPosChar;

public:
    Cook(const string& name, const string& nation);
    ~Cook();

    void display() const override;
    void setClue(const Clue& clue) override;
    Clue getClue() const override;
};

class Guard : public HouseHelp
{
private:
    WordRow clueRowChar;
    char    letterClueChar;
    int     boardPosChar;

public:
    Guard(const string& name, const string& nation);
    ~Guard();

    void display() const override;
    void setClue(const Clue& clue) override;
    Clue getClue() const override;
};

class Servant : public HouseHelp
{
private:
    WordRow clueRowChar;
    char    letterClueChar;
    int     boardPosChar;

public:
    Servant(const string& name, const string& nation);
    ~Servant();

    void display() const override;
    void setClue(const Clue& clue) override;
    Clue getClue() const override;
};

// the teenager, age is kept between 15 and 19
class OwnersChild : public CloseRelations
{
private:
    int     age;
    WordRow clueRowChar;
    char    letterClueChar;
    int     boardPosChar;

public:
    OwnersChild(const string& name, const string& nation, int childAge = 17);
    ~OwnersChild();

    int  getAge() const;
    void display() const override;
    void setClue(const Clue& clue) override;
    Clue getClue() const override;
};

// ===== cottage (module 4) =====
// keeps the 7 rooms and lets the detective search them.
// relationship: cottage <>-- location array (aggregation)
class Cottage
{
public:
    static const int TOTAL_LOCATIONS = 7;

private:
    // old raw dma version, kept as the brief asks
    // Location* locations;   // locations = new location[total_locations];
    // int locationCount;

    DynamicArray<Location> locationList;

public:
    Cottage();
    Cottage(const Cottage& other);
    Cottage& operator=(const Cottage& other);
    ~Cottage();

    // reads locations.txt and keeps only the letter column of the chosen culprit
    void loadLocations(const string& fileName, int profileIndex);

    int  getLocationCount() const;
    bool isSearched(int index) const;
    const Location& getLocation(int index) const;

    // first search marks the room as searched and gives its clue
    Clue searchLocation(int index);

    // gives the clue again without changing anything (for searching a room twice)
    Clue getClueAt(int index) const;

    void displayLocations() const;
};

// ===== scrabble board (module 4) =====
// jagged board: row 1 has 7 slots for word 1, row 2 has 8 slots for word 2.
// a tile's position is just its index in the row
class Scrabble
{
public:
    static const int WORD1_LENGTH = 7;
    static const int WORD2_LENGTH = 8;

private:
    // old raw dma version, kept as the brief asks
    // Tile** board;                       // board = new tile*[2];
    // board[0] = new Tile[WORD1_LENGTH];  // row for word 1
    // board[1] = new Tile[WORD2_LENGTH];  // row for word 2

    DynamicArray<Tile> wordRow1;
    DynamicArray<Tile> wordRow2;
    string targetWord1;
    string targetWord2;
    string culpritRole;
    bool   word1Solved;
    bool   word2Solved;

    void buildEmptyRows();

    DynamicArray<Tile>& getRow(WordRow row);
    const DynamicArray<Tile>& getRow(WordRow row) const;

public:
    Scrabble();
    Scrabble(const Scrabble& other);
    Scrabble& operator=(const Scrabble& other);
    ~Scrabble();

    void loadTargets(const string& fileName, int profileIndex);

    // throws if the position doesn't exist or already has a letter
    void placeLetter(const Clue& clue);

    bool isRowComplete(WordRow row) const;
    bool isWordSolved(WordRow row) const;
    bool bothWordsSolved() const;

    // throws if the row isn't full yet or the typed word is wrong
    void verifyWord(WordRow row, const string& guess);

    string getTargetWord(WordRow row) const;
    string getCulpritRole() const;
    string getRowLetters(WordRow row) const;
    int    getRowLength(WordRow row) const;

    void display() const;

    // << draws the board and >> loads two new target words
    friend ostream& operator<<(ostream& out, const Scrabble& board);

    friend istream& operator>>(istream& in, Scrabble& board);
};

// ===== leader / score board (module 6) =====
// adds a line to leaderboard.txt after every game and shows all the old results
class LeaderBoard
{
private:
    string fileName;

public:
    LeaderBoard(const string& file = LEADERBOARD_FILE);

    void addScore(const string& detectiveName, const string& result,
                  int credibility, int turnsLeft, const string& culpritRole);

    void display() const;

    static string getTodayDate();
};

// ===== game engine (modules 2, 10, 12, 13, 14) =====
// runs the whole game. it's a singleton, so only one engine can ever exist.
// composition (owns them, they die with it): detective, cottage, scrabble, tilerack
// association (only uses it, doesn't own it): leaderboard
class GameEngine
{
private:
    // the one and only engine
    static GameEngine* instance;

    // composition
    Detective detective;
    Cottage   cottage;
    Scrabble  scrabble;
    TileRack  clueRack;

    // association - just a pointer, main() owns the real object
    LeaderBoard* leaderBoard;

    // victim + 7 suspects, made with new and deleted in the destructor
    DynamicArray<Character*> people;

    GameState state;
    int    culpritIndex;
    int    profileIndex;

    int    maxTurns;
    int    startingCredibility;
    int    wrongTurnPenalty;
    int    wrongWordPenalty;
    string winCondition;
    string loseCondition;

    // private constructor, only getInstance() can make the engine
    GameEngine();

    // copying a singleton makes no sense, so it's blocked
    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;

    void       setDefaultRules();
    void       loadRules();
    void       loadCharacters(int profile);
    void       clearCharacters();
    Character* createCharacter(const string& name, const string& role, const string& nation);
    void       selectCulprit();
    void       validateBoardData() const;
    string     askDetectiveName() const;


public:
    ~GameEngine();

    // gives the single engine and creates it the first time
    static GameEngine* getInstance();

    static void destroyInstance();

    void setLeaderBoard(LeaderBoard* board);

    void showTitle() const;
    void showMainMenu() const;
    void showAbout() const;
    void showRules();
    void showLeaderBoard() const;
    void startNewGame();


    GameState getState() const;
    void      setState(GameState newState);
};

#endif
