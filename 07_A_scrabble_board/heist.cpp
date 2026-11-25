// heist.cpp - the locker heist
// function bodies for everything declared in heist.h
#include "heist.h"

// ===== exceptions =====
GameException::GameException(const string& errorMessage) : message(errorMessage) {}

GameException::~GameException() {}

// puts the type label first, like "[Invalid Input] please enter a number"
string GameException::getMessage() const
{
    return "[" + getType() + "] " + message;
}

string GameException::getType() const
{
    return "Game Error";
}

bool GameException::countsAsWrongStep() const
{
    return true;
}

// child exceptions, each one only changes its label
InvalidInputException::InvalidInputException(const string& errorMessage) : GameException(errorMessage) {}
string InvalidInputException::getType() const { return "Invalid Input"; }

InvalidPositionException::InvalidPositionException(const string& errorMessage) : GameException(errorMessage) {}
string InvalidPositionException::getType() const { return "Invalid Position"; }

SlotFilledException::SlotFilledException(const string& errorMessage) : GameException(errorMessage) {}
string SlotFilledException::getType() const { return "Slot Filled"; }

RackFullException::RackFullException(const string& errorMessage) : GameException(errorMessage) {}
string RackFullException::getType() const { return "Rack Full"; }

RackEmptyException::RackEmptyException(const string& errorMessage) : GameException(errorMessage) {}
string RackEmptyException::getType() const { return "Rack Empty"; }

InvalidWordException::InvalidWordException(const string& errorMessage) : GameException(errorMessage) {}
string InvalidWordException::getType() const { return "Wrong Word"; }

IncompleteWordException::IncompleteWordException(const string& errorMessage) : GameException(errorMessage) {}
string IncompleteWordException::getType() const { return "Word Incomplete"; }

LockedCharacterException::LockedCharacterException(const string& errorMessage) : GameException(errorMessage) {}
string LockedCharacterException::getType() const { return "Interview Refused"; }

WrongAccusationException::WrongAccusationException(const string& errorMessage) : GameException(errorMessage) {}
string WrongAccusationException::getType() const { return "Wrong Accusation"; }

FileException::FileException(const string& errorMessage) : GameException(errorMessage) {}
string FileException::getType() const { return "File Error"; }

// file problems aren't the player's fault
bool FileException::countsAsWrongStep() const { return false; }

// ===== helper functions =====
string intToString(int value)
{
    if (value == 0)
    {
        return "0";
    }

    bool isNegative = (value < 0);
    long long number = value;
    if (isNegative)
    {
        number = -number;
    }

    string digits = "";
    while (number > 0)
    {
        char digit = static_cast<char>('0' + (number % 10));
        digits = digit + digits;
        number /= 10;
    }

    return isNegative ? "-" + digits : digits;
}

string twoDigits(int value)
{
    if (value >= 0 && value < 10)
    {
        return "0" + intToString(value);
    }
    return intToString(value);
}

bool stringToInt(const string& text, int& result)
{
    string cleanText = trim(text);
    if (cleanText.empty())
    {
        return false;
    }

    int index = 0;
    bool isNegative = false;
    if (cleanText[0] == '-' || cleanText[0] == '+')
    {
        isNegative = (cleanText[0] == '-');
        index = 1;
    }

    if (index >= static_cast<int>(cleanText.length()))
    {
        return false;
    }

    long long value = 0;
    for (; index < static_cast<int>(cleanText.length()); index++)
    {
        char current = cleanText[index];
        if (current < '0' || current > '9')
        {
            return false;
        }
        value = value * 10 + (current - '0');
        if (value > 1000000)
        {
            return false;
        }
    }

    result = static_cast<int>(isNegative ? -value : value);
    return true;
}

string trim(const string& text)
{
    int start = 0;
    int end = static_cast<int>(text.length()) - 1;

    while (start <= end && (text[start] == ' ' || text[start] == '\t' ||
                            text[start] == '\r' || text[start] == '\n'))
    {
        start++;
    }
    while (end >= start && (text[end] == ' ' || text[end] == '\t' ||
                            text[end] == '\r' || text[end] == '\n'))
    {
        end--;
    }

    if (start > end)
    {
        return "";
    }
    return text.substr(start, end - start + 1);
}

char toUpperChar(char letter)
{
    if (letter >= 'a' && letter <= 'z')
    {
        return static_cast<char>(letter - 'a' + 'A');
    }
    return letter;
}

string toUpperCase(const string& text)
{
    string result = text;
    for (int i = 0; i < static_cast<int>(result.length()); i++)
    {
        result[i] = toUpperChar(result[i]);
    }
    return result;
}

string replaceChar(const string& text, char from, char to)
{
    string result = text;
    for (int i = 0; i < static_cast<int>(result.length()); i++)
    {
        if (result[i] == from)
        {
            result[i] = to;
        }
    }
    return result;
}

string padRight(const string& text, int width)
{
    string result = text;
    while (static_cast<int>(result.length()) < width)
    {
        result += ' ';
    }
    return result;
}

static int splitLine(const string& line, char delimiter, DynamicArray<string>& parts)
{
    parts.clear();
    string current = "";

    for (int i = 0; i < static_cast<int>(line.length()); i++)
    {
        if (line[i] == delimiter)
        {
            parts.add(trim(current));
            current = "";
        }
        else
        {
            current += line[i];
        }
    }
    parts.add(trim(current));
    return parts.getSize();
}

string readLine()
{
    string line;
    if (!getline(cin, line))
    {
        cout << "\n  Input closed. Goodbye, Detective!\n";
        exit(0);
    }
    return trim(line);
}

int readNumber(const string& prompt)
{
    cout << prompt;
    string answer = readLine();

    int value = 0;
    if (!stringToInt(answer, value))
    {
        throw InvalidInputException("'" + answer + "' is not a valid number.");
    }
    return value;
}

bool readYesNo(const string& prompt)
{
    while (true)
    {
        cout << prompt;
        string answer = toUpperCase(readLine());

        if (answer == "Y" || answer == "YES")
        {
            return true;
        }
        if (answer == "N" || answer == "NO")
        {
            return false;
        }
        cout << "  Please type Y or N.\n";
    }
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen()
{
    cout << "\n  Press Enter to continue...";
    readLine();
}

void printBoxBorder()
{
    cout << "  +" << string(BOX_WIDTH, '=') << "+\n";
}

void printBoxDivider()
{
    cout << "  |" << string(BOX_WIDTH, '-') << "|\n";
}

void printBoxLine(const string& text, bool centered)
{
    string content = text;
    if (static_cast<int>(content.length()) > BOX_WIDTH - 2)
    {
        content = content.substr(0, BOX_WIDTH - 2);
    }

    int freeSpace = BOX_WIDTH - static_cast<int>(content.length());
    int leftSpace = centered ? freeSpace / 2 : 2;
    int rightSpace = freeSpace - leftSpace;

    cout << "  |" << string(leftSpace, ' ') << content << string(rightSpace, ' ') << "|\n";
}

// ===== structs =====
Tile::Tile() : tileLetter(' '), filled(false) {}

Tile::Tile(char letter) : tileLetter(toUpperChar(letter)), filled(letter != ' ') {}

// comparing letters (operator overloading), used when checking words against clue.txt
bool Tile::operator==(const Tile& other) const
{
    return tileLetter == other.tileLetter;
}

bool Tile::operator==(char letter) const
{
    return filled && tileLetter == toUpperChar(letter);
}

bool Tile::operator!=(char letter) const
{
    return !(*this == letter);
}

ostream& operator<<(ostream& out, const Tile& tile)
{
    if (tile.filled)
    {
        out << "[" << tile.tileLetter << "]";
    }
    else
    {
        out << "[ ]";
    }
    return out;
}

istream& operator>>(istream& in, Tile& tile)
{
    char letter;
    if (in >> letter)
    {
        tile = Tile(letter);
    }
    return in;
}

Clue::Clue() : clueLetter(' '), cluePosition(0), clueRow(WORD_1) {}

Clue::Clue(char letter, int position, WordRow row)
    : clueLetter(toUpperChar(letter)), cluePosition(position), clueRow(row) {}

bool Clue::operator==(const Clue& other) const
{
    return clueLetter == other.clueLetter &&
           cluePosition == other.cluePosition &&
           clueRow == other.clueRow;
}

Location::Location()
    : locName("Unknown"), letterClueLoc(' '), boardPosLoc(0), locRow(WORD_1), locSearchFlag(false) {}

Location::Location(const string& name, char letter, int position)
    : locName(name), letterClueLoc(toUpperChar(letter)), boardPosLoc(position),
      locRow(WORD_1), locSearchFlag(false) {}

ostream& operator<<(ostream& out, const Location& location)
{
    out << padRight(replaceChar(location.locName, '_', ' '), 14) << " -  "
        << (location.locSearchFlag ? "Searched" : "NOT Searched");
    return out;
}

istream& operator>>(istream& in, Location& location)
{
    string name;
    char letter;
    int position;
    if (in >> name >> letter >> position)
    {
        location = Location(name, letter, position);
    }
    return in;
}

// ===== tile rack =====
TileRack::TileRack(const string& name, int maximumTiles)
    : tiles(maximumTiles), positions(maximumTiles), maxTiles(maximumTiles), rackName(name)
{
    // old raw dma version, kept as the brief asks
    // rackTiles = new Tile[maxTiles];
    // rackPositions = new int[maxTiles];
    // tileCount = 0;
}

// copy constructor, the dynamic array members already deep copy themselves
TileRack::TileRack(const TileRack& other)
    : tiles(other.tiles), positions(other.positions), maxTiles(other.maxTiles), rackName(other.rackName)
{
    // old raw dma version, kept as the brief asks
    // rackTiles = new Tile[maxTiles];
    // rackPositions = new int[maxTiles];
    // for (int i = 0; i < other.tileCount; i++) { rackTiles[i] = other.rackTiles[i]; ... }
}

TileRack& TileRack::operator=(const TileRack& other)
{
    if (this != &other)
    {
        tiles = other.tiles;
        positions = other.positions;
        maxTiles = other.maxTiles;
        rackName = other.rackName;
    }
    return *this;
}

TileRack::~TileRack()
{
    // old raw dma version, kept as the brief asks
    // delete[] rackTiles;
    // delete[] rackPositions;
}

void TileRack::addTile(const Tile& tile, int position)
{
    if (isFull())
    {
        throw RackFullException(rackName + " is full (" + intToString(maxTiles) + " tiles).");
    }
    tiles.add(tile);
    positions.add(position);
}

Tile TileRack::removeLastTile()
{
    if (isEmpty())
    {
        throw RackEmptyException(rackName + " has no tiles to remove.");
    }
    Tile lastTile = tiles[tiles.getSize() - 1];
    tiles.removeLast();
    positions.removeLast();
    return lastTile;
}

void TileRack::clearRack()
{
    tiles.clear();
    positions.clear();
}

bool TileRack::isFull() const      { return tiles.getSize() >= maxTiles; }
bool TileRack::isEmpty() const     { return tiles.getSize() == 0; }
int  TileRack::getCount() const    { return tiles.getSize(); }
int  TileRack::getMaxTiles() const { return maxTiles; }

string TileRack::getLettersAsString() const
{
    string letters = "";
    for (int i = 0; i < tiles.getSize(); i++)
    {
        letters += tiles[i].tileLetter;
    }
    return letters;
}

void TileRack::display() const
{
    cout << rackName << " (" << getCount() << "/" << maxTiles << "): ";
    if (isEmpty())
    {
        cout << "(empty)";
    }
    for (int i = 0; i < tiles.getSize(); i++)
    {
        if (tiles[i].filled)
        {
            cout << tiles[i].tileLetter << "(" << positions[i] << ") ";
        }
        else
        {
            cout << "  ";
        }
    }
    cout << endl;
}

// merge: this rack, then a space tile, then the other rack
TileRack TileRack::operator+(const TileRack& other) const
{
    TileRack merged("Merged Clue", maxTiles + other.maxTiles + 1);

    for (int i = 0; i < tiles.getSize(); i++)
    {
        merged.addTile(tiles[i], positions[i]);
    }

    merged.addTile(Tile(' '), 0);

    for (int i = 0; i < other.tiles.getSize(); i++)
    {
        merged.addTile(other.tiles[i], other.positions[i]);
    }

    return merged;
}

// ===== cottage =====
Cottage::Cottage() : locationList(TOTAL_LOCATIONS)
{
    // old raw dma version, kept as the brief asks
    // locations = new Location[TOTAL_LOCATIONS];
    // locationCount = 0;
}

Cottage::Cottage(const Cottage& other) : locationList(other.locationList)
{
    // old raw dma version, kept as the brief asks
    // locations = new Location[TOTAL_LOCATIONS];
    // for (int i = 0; i < other.locationCount; i++) locations[i] = other.locations[i];
}

Cottage& Cottage::operator=(const Cottage& other)
{
    if (this != &other)
    {
        locationList = other.locationList;
    }
    return *this;
}

// the dynamic array member frees the rooms and the array itself
Cottage::~Cottage()
{
    // old raw dma version, kept as the brief asks
    // delete[] locations;
    // locations = nullptr;
}

// each line: Place|W1Pos|Cook|Guard|Servant|P1|P2|PA|Child
// we take the letter from the column of the chosen culprit
void Cottage::loadLocations(const string& fileName, int profileIndex)
{
    ifstream file(fileName.c_str());
    if (!file)
    {
        throw FileException(fileName + " was not found.");
    }

    locationList.clear();
    DynamicArray<string> parts(12);
    string line;
    int lineNumber = 0;

    getline(file, line);
    lineNumber++;

    while (getline(file, line))
    {
        lineNumber++;
        line = trim(line);
        if (line.empty())
        {
            continue;
        }

        if (splitLine(line, '|', parts) != 2 + TOTAL_PROFILES)
        {
            throw FileException(fileName + " is corrupted at line " + intToString(lineNumber) + ".");
        }

        int position = 0;
        string letterText = parts[2 + profileIndex];
        if (!stringToInt(parts[1], position) || letterText.length() != 1)
        {
            throw FileException(fileName + " has a bad value at line " + intToString(lineNumber) + ".");
        }

        locationList.add(Location(parts[0], letterText[0], position));
    }

    if (locationList.getSize() != TOTAL_LOCATIONS)
    {
        throw FileException(fileName + " must contain exactly " + intToString(TOTAL_LOCATIONS) + " locations.");
    }
}

int Cottage::getLocationCount() const
{
    return locationList.getSize();
}

bool Cottage::isSearched(int index) const
{
    return locationList[index].locSearchFlag;
}

const Location& Cottage::getLocation(int index) const
{
    return locationList[index];
}

Clue Cottage::searchLocation(int index)
{
    locationList[index].locSearchFlag = true;
    return getClueAt(index);
}

Clue Cottage::getClueAt(int index) const
{
    const Location& place = locationList[index];
    return Clue(place.letterClueLoc, place.boardPosLoc, place.locRow);
}

void Cottage::displayLocations() const
{
    cout << "\n  ============== Location Menu ==============\n";
    for (int i = 0; i < locationList.getSize(); i++)
    {
        cout << "  [" << (i + 1) << "] " << locationList[i] << "\n";
    }
    cout << "  [0] Back\n";
    cout << "  ===========================================\n";
}

// ===== scrabble =====
// fills both rows with empty tiles
void Scrabble::buildEmptyRows()
{
    wordRow1.clear();
    wordRow2.clear();
    for (int i = 0; i < WORD1_LENGTH; i++)
    {
        wordRow1.add(Tile());
    }
    for (int i = 0; i < WORD2_LENGTH; i++)
    {
        wordRow2.add(Tile());
    }
    word1Solved = false;
    word2Solved = false;
}

DynamicArray<Tile>& Scrabble::getRow(WordRow row)
{
    return (row == WORD_1) ? wordRow1 : wordRow2;
}

const DynamicArray<Tile>& Scrabble::getRow(WordRow row) const
{
    return (row == WORD_1) ? wordRow1 : wordRow2;
}

Scrabble::Scrabble()
    : wordRow1(WORD1_LENGTH), wordRow2(WORD2_LENGTH),
      targetWord1(""), targetWord2(""), culpritRole(""),
      word1Solved(false), word2Solved(false)
{
    // old raw dma version, kept as the brief asks
    // board = new Tile*[2];
    // board[0] = new Tile[WORD1_LENGTH];
    // board[1] = new Tile[WORD2_LENGTH];
    buildEmptyRows();
}

Scrabble::Scrabble(const Scrabble& other)
    : wordRow1(other.wordRow1), wordRow2(other.wordRow2),
      targetWord1(other.targetWord1), targetWord2(other.targetWord2),
      culpritRole(other.culpritRole),
      word1Solved(other.word1Solved), word2Solved(other.word2Solved) {}

Scrabble& Scrabble::operator=(const Scrabble& other)
{
    if (this != &other)
    {
        wordRow1 = other.wordRow1;
        wordRow2 = other.wordRow2;
        targetWord1 = other.targetWord1;
        targetWord2 = other.targetWord2;
        culpritRole = other.culpritRole;
        word1Solved = other.word1Solved;
        word2Solved = other.word2Solved;
    }
    return *this;
}

Scrabble::~Scrabble()
{
    // old raw dma version, kept as the brief asks
    // delete[] board[0];
    // delete[] board[1];
    // delete[] board;
}

// each line: Culprit_Role|Word_1|Word_2 and the line number is the profile number
void Scrabble::loadTargets(const string& fileName, int profileIndex)
{
    ifstream file(fileName.c_str());
    if (!file)
    {
        throw FileException(fileName + " was not found.");
    }

    DynamicArray<string> parts(4);
    string line;
    getline(file, line);

    int profileCounter = 0;
    bool found = false;
    while (getline(file, line))
    {
        line = trim(line);
        if (line.empty())
        {
            continue;
        }

        if (profileCounter == profileIndex)
        {
            if (splitLine(line, '|', parts) != 3)
            {
                throw FileException(fileName + " is corrupted on profile " + intToString(profileIndex + 1) + ".");
            }
            culpritRole = parts[0];
            targetWord1 = toUpperCase(parts[1]);
            targetWord2 = toUpperCase(parts[2]);
            found = true;
            break;
        }
        profileCounter++;
    }

    if (!found)
    {
        throw FileException(fileName + " has no profile number " + intToString(profileIndex + 1) + ".");
    }
    if (static_cast<int>(targetWord1.length()) != WORD1_LENGTH ||
        static_cast<int>(targetWord2.length()) != WORD2_LENGTH)
    {
        throw FileException(fileName + ": Word_1 must have 7 letters and Word_2 must have 8 letters.");
    }

    buildEmptyRows();
}

// checks the slot exists and is empty before placing the letter
void Scrabble::placeLetter(const Clue& clue)
{
    DynamicArray<Tile>& row = getRow(clue.clueRow);
    int index = clue.cluePosition - 1;
    int wordNumber = (clue.clueRow == WORD_1) ? 1 : 2;

    if (index < 0 || index >= row.getSize())
    {
        throw InvalidPositionException("Position " + intToString(clue.cluePosition) +
                                       " does not exist in Word " + intToString(wordNumber) + ".");
    }
    if (row[index].filled)
    {
        throw SlotFilledException("Position " + intToString(clue.cluePosition) + " of Word " +
                                  intToString(wordNumber) + " already holds a letter.");
    }

    row[index] = Tile(clue.clueLetter);
}

bool Scrabble::isRowComplete(WordRow row) const
{
    const DynamicArray<Tile>& tiles = getRow(row);
    for (int i = 0; i < tiles.getSize(); i++)
    {
        if (!tiles[i].filled)
        {
            return false;
        }
    }
    return true;
}

bool Scrabble::isWordSolved(WordRow row) const
{
    return (row == WORD_1) ? word1Solved : word2Solved;
}

bool Scrabble::bothWordsSolved() const
{
    return word1Solved && word2Solved;
}

// compares the typed word with the tiles on the board and the word from clue.txt
void Scrabble::verifyWord(WordRow row, const string& guess)
{
    int wordNumber = (row == WORD_1) ? 1 : 2;
    if (!isRowComplete(row))
    {
        throw IncompleteWordException("Word " + intToString(wordNumber) +
                                      " still has empty slots. Collect all its letters first.");
    }

    const DynamicArray<Tile>& tiles = getRow(row);
    string typedWord = toUpperCase(trim(guess));
    string target = getTargetWord(row);

    if (static_cast<int>(typedWord.length()) != tiles.getSize())
    {
        throw InvalidWordException("'" + typedWord + "' must have exactly " +
                                   intToString(tiles.getSize()) + " letters.");
    }

    for (int i = 0; i < tiles.getSize(); i++)
    {
        if (tiles[i] != typedWord[i] || !(tiles[i] == Tile(target[i])))
        {
            throw InvalidWordException("'" + typedWord + "' is not the hidden word.");
        }
    }

    if (row == WORD_1)
    {
        word1Solved = true;
    }
    else
    {
        word2Solved = true;
    }
}

string Scrabble::getTargetWord(WordRow row) const
{
    return (row == WORD_1) ? targetWord1 : targetWord2;
}

string Scrabble::getCulpritRole() const
{
    return culpritRole;
}

string Scrabble::getRowLetters(WordRow row) const
{
    const DynamicArray<Tile>& tiles = getRow(row);
    string letters = "";
    for (int i = 0; i < tiles.getSize(); i++)
    {
        letters += tiles[i].filled ? tiles[i].tileLetter : '_';
    }
    return letters;
}

int Scrabble::getRowLength(WordRow row) const
{
    return getRow(row).getSize();
}

void Scrabble::display() const
{
    cout << *this;
}

ostream& operator<<(ostream& out, const Scrabble& board)
{
    out << "\n  ---------------- SCRABBLE BOARD ----------------\n";
    out << "  Position:  1   2   3   4   5   6   7   8\n";

    out << "  Word 1:   ";
    for (int i = 0; i < board.wordRow1.getSize(); i++)
    {
        out << board.wordRow1[i] << " ";
    }
    out << "    ";
    if (board.word1Solved)                      out << "SOLVED";
    else if (board.isRowComplete(WORD_1))       out << "COMPLETE";
    out << "\n";

    out << "  Word 2:   ";
    for (int i = 0; i < board.wordRow2.getSize(); i++)
    {
        out << board.wordRow2[i] << " ";
    }
    if (board.word2Solved)                      out << "SOLVED";
    else if (board.isRowComplete(WORD_2))       out << "COMPLETE";
    out << "\n";
    out << "  ------------------------------------------------\n";
    return out;
}

istream& operator>>(istream& in, Scrabble& board)
{
    string first, second;
    if (in >> first >> second)
    {
        board.targetWord1 = toUpperCase(first);
        board.targetWord2 = toUpperCase(second);
        board.buildEmptyRows();
    }
    return in;
}

