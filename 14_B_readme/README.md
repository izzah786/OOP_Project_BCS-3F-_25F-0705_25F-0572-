# THE LOCKER HEIST

Console detective game for CL1004 – Object Oriented Programming (Fall 2026).

## Team
| Name | Roll Number | Work |
|------|-------------|------|
| Student 1 | 25F-XXXX | ... |
| Student 2 | 25F-XXXX | ... |

## Files
| File | Purpose |
|------|---------|
| `heist.h` | Declarations: structs, enums, exceptions, `DynamicArray<T>`, all classes |
| `heist.cpp` | Definitions of everything in `heist.h` |
| `main.cpp` | Driver: Main Menu, game loop, catches every `GameException` |
| `clue.txt` | Culprit profiles: `Culprit_Role|Word_1|Word_2` |
| `locations.txt` | 7 locations with Word 1 position + letter for every profile |
| `characters.txt` | 8 people with Word 2 position + letter for every profile |
| `rules.txt` | Max turns, starting credibility, penalties, win/lose conditions |
| `leaderboard.txt` | Saved results (appended after every game) |
| `about.txt` | Team and game information |

## Build / Run (Visual Studio 2022)
1. Create an *Empty Project* (C++).
2. Add `heist.h` to *Header Files*, and `heist.cpp` + `main.cpp` to *Source Files*.
3. Copy all `.txt` files into the project folder (the folder that holds the `.vcxproj`).
4. Press **Ctrl+F5**.

## Deviations from the manual (with reason)
- `characters.txt` has 8 lines (victim + 7 suspects), not 7, because all 8 people give a Word 2 letter.
- The leader/score board file is named `leaderboard.txt` because `/` cannot be used in a file name.
- Every sub-menu has a `[0] Back` option; going back does not use a turn.
- Typing a wrong word costs −10 and the detective keeps typing until it is correct (or credibility ends).
- Quitting a running game is saved to the leader board as `LOSE`.
- A wrong accusation costs −5 credibility; the game continues.

## Known Limitations
- Console only; output is ASCII (no Unicode box characters) so it shows correctly on every Windows console.
- One player per game, no save/resume in the middle of a game.
- Culprit is chosen with `rand()`, so the same culprit can appear in back-to-back games.
- Data files must keep the exact `|`-separated format; a corrupted file stops a new game from starting.
