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

