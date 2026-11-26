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

// ===== characters =====
Character::Character(const string& name, const string& role, const string& nation)
    : characterName(name), characterRole(role), nationality(nation),
      isCulprit(false), interviewed(false) {}

Character::~Character() {}

string Character::getName() const        { return characterName; }
string Character::getRole() const        { return characterRole; }
string Character::getNationality() const { return nationality; }
bool   Character::getIsCulprit() const   { return isCulprit; }
bool   Character::isInterviewed() const  { return interviewed; }

void Character::setName(const string& name)          { characterName = name; }
void Character::setNationality(const string& nation) { nationality = nation; }
void Character::setInterviewed(bool value)           { interviewed = value; }

int Character::getRequiredCredibility() const
{
    return 0;
}

bool Character::isSuspect() const
{
    return false;
}

// virtual call, so the real object decides what it needs
bool Character::canBeInterviewed(int credibility) const
{
    return credibility >= getRequiredCredibility();
}

ostream& operator<<(ostream& out, const Character& person)
{
    out << person.characterName << " (" << person.characterRole << ")";
    return out;
}

istream& operator>>(istream& in, Character& person)
{
    string name, nation;
    if (in >> name >> nation)
    {
        person.characterName = name;
        person.nationality = nation;
    }
    return in;
}

Victim::Victim(const string& name, const string& nation)
    : Character(name, "Victim", nation), clueRowChar(WORD_2), letterClueChar(' '), boardPosChar(0) {}

Victim::~Victim() {}

void Victim::display() const
{
    cout << "  " << characterName << " | Business Owner (Victim) | " << nationality << "\n";
    cout << "  \"The locker behind my bookshelf was empty when I checked.\n"
         << "   Only someone close to this house knew which book opens it!\"\n";
}

void Victim::setClue(const Clue& clue)
{
    clueRowChar = clue.clueRow;
    letterClueChar = clue.clueLetter;
    boardPosChar = clue.cluePosition;
}

Clue Victim::getClue() const
{
    return Clue(letterClueChar, boardPosChar, clueRowChar);
}

// the owner only talks if credibility is 80 or more
int Victim::getRequiredCredibility() const
{
    return 80;
}

// racks are sized to the two word lengths
Detective::Detective(const string& name, int startCredibility)
    : Character(name, "Detective", "Malay"),
      credibility(startCredibility), turnsUsed(0),
      locationRack("Location Rack", Scrabble::WORD1_LENGTH),
      characterRack("Character Rack", Scrabble::WORD2_LENGTH) {}

Detective::Detective(const Detective& other)
    : Character(other), credibility(other.credibility), turnsUsed(other.turnsUsed),
      locationRack(other.locationRack), characterRack(other.characterRack) {}

Detective& Detective::operator=(const Detective& other)
{
    if (this != &other)
    {
        Character::operator=(other);
        credibility = other.credibility;
        turnsUsed = other.turnsUsed;
        locationRack = other.locationRack;
        characterRack = other.characterRack;
    }
    return *this;
}

Detective::~Detective() {}

void Detective::resetForNewGame(const string& name, int startCredibility)
{
    characterName = name;
    credibility = startCredibility;
    turnsUsed = 0;
    locationRack.clearRack();
    characterRack.clearRack();
}

int  Detective::getCredibility() const { return credibility; }
void Detective::setCredibility(int value) { credibility = (value < 0) ? 0 : value; }

// negative amount = penalty, credibility never goes below 0
void Detective::changeCredibility(int amount)
{
    setCredibility(credibility + amount);
}

int  Detective::getTurns() const { return turnsUsed; }
void Detective::setTurns(int turns) { turnsUsed = (turns < 0) ? 0 : turns; }

// word 1 letters go in the location rack, word 2 letters in the character rack
void Detective::collectLetter(const Clue& clue)
{
    Tile newTile(clue.clueLetter);
    if (clue.clueRow == WORD_1)
    {
        locationRack.addTile(newTile, clue.cluePosition);
    }
    else
    {
        characterRack.addTile(newTile, clue.cluePosition);
    }
}

const TileRack& Detective::getLocationRack() const  { return locationRack; }
const TileRack& Detective::getCharacterRack() const { return characterRack; }

void Detective::display() const
{
    cout << "  Detective " << characterName << " | Credibility: " << credibility
         << " | Turns used: " << turnsUsed << "\n";
}

// the detective collects clues but never gives one
void Detective::setClue(const Clue&)
{
    throw GameException("The detective does not hold a clue letter.");
}

Clue Detective::getClue() const
{
    throw GameException("The detective does not hold a clue letter.");
}

// ===== suspects =====
Suspect::Suspect(const string& name, const string& role, const string& nation)
    : Character(name, role, nation) {}

Suspect::~Suspect() {}

bool Suspect::isSuspect() const
{
    return true;
}

BusinessRelations::BusinessRelations(const string& name, const string& role, const string& nation)
    : Suspect(name, role, nation) {}

BusinessRelations::~BusinessRelations() {}

int BusinessRelations::getRequiredCredibility() const
{
    return 50;
}

HouseHelp::HouseHelp(const string& name, const string& role, const string& nation)
    : Suspect(name, role, nation) {}

HouseHelp::~HouseHelp() {}

CloseRelations::CloseRelations(const string& name, const string& role, const string& nation)
    : Suspect(name, role, nation) {}

CloseRelations::~CloseRelations() {}

int CloseRelations::getRequiredCredibility() const
{
    return 75;
}

// suspect is a virtual base, so the last class in the chain has to build it itself
Partner::Partner(const string& name, const string& nation, int number)
    : Suspect(name, "Partner " + intToString(number), nation),
      BusinessRelations(name, "Partner " + intToString(number), nation),
      partnerNumber(number), clueRowChar(WORD_2), letterClueChar(' '), boardPosChar(0) {}

Partner::~Partner() {}

int Partner::getPartnerNumber() const { return partnerNumber; }

void Partner::display() const
{
    cout << "  " << characterName << " | Business Partner " << partnerNumber << " | " << nationality << "\n";
    cout << "  \"I was discussing next quarter's contracts all evening.\n"
         << "   Ask anyone - I never went upstairs.\"\n";
}

void Partner::setClue(const Clue& clue)
{
    clueRowChar = clue.clueRow;
    letterClueChar = clue.clueLetter;
    boardPosChar = clue.cluePosition;
}

Clue Partner::getClue() const
{
    return Clue(letterClueChar, boardPosChar, clueRowChar);
}

// diamond: pa builds the one shared suspect first, then both parents
PA::PA(const string& name, const string& nation)
    : Suspect(name, "PA", nation),
      BusinessRelations(name, "PA", nation),
      CloseRelations(name, "PA", nation),
      clueRowChar(WORD_2), letterClueChar(' '), boardPosChar(0) {}

PA::~PA() {}

// pa hired the detective, so they talk easily (35)
int PA::getRequiredCredibility() const
{
    return 35;
}

void PA::display() const
{
    cout << "  " << characterName << " | Personal Assistant | " << nationality << "\n";
    cout << "  \"I know the owner's secrets better than anyone. That is exactly\n"
         << "   why I hired you - find who did this before the news spreads.\"\n";
}

void PA::setClue(const Clue& clue)
{
    clueRowChar = clue.clueRow;
    letterClueChar = clue.clueLetter;
    boardPosChar = clue.cluePosition;
}

Clue PA::getClue() const
{
    return Clue(letterClueChar, boardPosChar, clueRowChar);
}

Cook::Cook(const string& name, const string& nation)
    : HouseHelp(name, "Cook", nation), clueRowChar(WORD_2), letterClueChar(' '), boardPosChar(0) {}

Cook::~Cook() {}

void Cook::display() const
{
    cout << "  " << characterName << " | Cook | " << nationality << "\n";
    cout << "  \"I was in the kitchen the whole night! The sauce does not\n"
         << "   stir itself, you know.\"\n";
}

void Cook::setClue(const Clue& clue)
{
    clueRowChar = clue.clueRow;
    letterClueChar = clue.clueLetter;
    boardPosChar = clue.cluePosition;
}

Clue Cook::getClue() const
{
    return Clue(letterClueChar, boardPosChar, clueRowChar);
}

Guard::Guard(const string& name, const string& nation)
    : HouseHelp(name, "Guard", nation), clueRowChar(WORD_2), letterClueChar(' '), boardPosChar(0) {}

Guard::~Guard() {}

void Guard::display() const
{
    cout << "  " << characterName << " | Guard | " << nationality << "\n";
    cout << "  \"Nobody left through the main gate after nine.\n"
         << "   Whoever took it is still inside this cottage.\"\n";
}

void Guard::setClue(const Clue& clue)
{
    clueRowChar = clue.clueRow;
    letterClueChar = clue.clueLetter;
    boardPosChar = clue.cluePosition;
}

Clue Guard::getClue() const
{
    return Clue(letterClueChar, boardPosChar, clueRowChar);
}

Servant::Servant(const string& name, const string& nation)
    : HouseHelp(name, "Servant", nation), clueRowChar(WORD_2), letterClueChar(' '), boardPosChar(0) {}

Servant::~Servant() {}

void Servant::display() const
{
    cout << "  " << characterName << " | Servant | " << nationality << "\n";
    cout << "  \"I served drinks downstairs. I did dust the study this\n"
         << "   morning, but I never touch the books, sir.\"\n";
}

void Servant::setClue(const Clue& clue)
{
    clueRowChar = clue.clueRow;
    letterClueChar = clue.clueLetter;
    boardPosChar = clue.cluePosition;
}

Clue Servant::getClue() const
{
    return Clue(letterClueChar, boardPosChar, clueRowChar);
}

OwnersChild::OwnersChild(const string& name, const string& nation, int childAge)
    : Suspect(name, "Child", nation),
      CloseRelations(name, "Child", nation),
      age((childAge < 15 || childAge > 19) ? 17 : childAge),
      clueRowChar(WORD_2), letterClueChar(' '), boardPosChar(0) {}

OwnersChild::~OwnersChild() {}

int OwnersChild::getAge() const { return age; }

void OwnersChild::display() const
{
    cout << "  " << characterName << " | Owner's Child (age " << age << ") | " << nationality << "\n";
    cout << "  \"W-why are you asking me? I was in my room with my music\n"
         << "   on... I didn't hear anything, I swear!\"\n";
}

void OwnersChild::setClue(const Clue& clue)
{
    clueRowChar = clue.clueRow;
    letterClueChar = clue.clueLetter;
    boardPosChar = clue.cluePosition;
}

Clue OwnersChild::getClue() const
{
    return Clue(letterClueChar, boardPosChar, clueRowChar);
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

// ===== leader board =====
LeaderBoard::LeaderBoard(const string& file) : fileName(file) {}

// adds one line at the end, writes the header first if the file is new
void LeaderBoard::addScore(const string& detectiveName, const string& result,
                           int credibility, int turnsLeft, const string& culpritRole)
{
    bool fileExists = false;
    {
        ifstream checkFile(fileName.c_str());
        fileExists = checkFile.good();
    }

    ofstream file(fileName.c_str(), ios::app);
    if (!file)
    {
        throw FileException("Could not write to " + fileName + ".");
    }

    if (!fileExists)
    {
        file << "Detective|Result|Credibility|Turns_Left|Culprit|Date\n";
    }

    file << detectiveName << "|" << result << "|" << credibility << "|"
         << twoDigits(turnsLeft) << "|" << replaceChar(culpritRole, ' ', '_') << "|"
         << getTodayDate() << "\n";
}

void LeaderBoard::display() const
{
    ifstream file(fileName.c_str());
    if (!file)
    {
        throw FileException(fileName + " was not found. Play a game to create it.");
    }

    DynamicArray<string> parts(8);
    string line;
    getline(file, line);

    cout << "\n";
    printBoxBorder();
    printBoxLine("THE LOCKER HEIST  -  LEADER / SCORE BOARD", true);
    printBoxBorder();
    cout << "  " << padRight("Detective", 16) << padRight("Result", 8) << padRight("Cred.", 7)
         << padRight("Left", 6) << padRight("Culprit", 12) << "Date\n";
    cout << "  " << string(BOX_WIDTH, '-') << "\n";

    int rows = 0;
    while (getline(file, line))
    {
        line = trim(line);
        if (line.empty())
        {
            continue;
        }
        if (splitLine(line, '|', parts) != 6)
        {
            cout << "  (skipped a corrupted line)\n";
            continue;
        }
        cout << "  " << padRight(parts[0], 16) << padRight(parts[1], 8) << padRight(parts[2], 7)
             << padRight(parts[3], 6) << padRight(parts[4], 12) << parts[5] << "\n";
        rows++;
    }

    if (rows == 0)
    {
        cout << "  No games played yet.\n";
    }
    cout << "  " << string(BOX_WIDTH, '-') << "\n";
}

// today's date as dd-mm-yy, using the safe localtime of each compiler
string LeaderBoard::getTodayDate()
{
    time_t now = time(nullptr);
    tm localTime;
#ifdef _MSC_VER
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif
    return twoDigits(localTime.tm_mday) + "-" + twoDigits(localTime.tm_mon + 1) + "-" +
           twoDigits(localTime.tm_year % 100);
}

