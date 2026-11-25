// heist.cpp - the locker heist
// function bodies for everything declared in heist.h
#include "heist.h"

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

