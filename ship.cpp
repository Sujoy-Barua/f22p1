// Project Identifier: 950181F63D0A883F183EC0A5CC67B19928FE896A

#include <getopt.h>
#include <iostream>
#include <vector>
#include <string>
#include <deque>

using namespace std;

struct Cube {
    uint8_t fromLev = 11;
    char x = '.';
    //bool set = false; //set this to true during coordinate input mode
    char fromPrev = 'x'; //current cube direction relative to prev cube 
    bool dis = false; //discovered
   
};

struct deckdata {
    uint32_t lvl = 0;
    uint32_t row = 0;
    uint32_t col = 0;
};

bool operator!=(const deckdata & lhs, const deckdata & rhs) {
    return lhs.lvl != rhs.lvl || lhs.row != rhs.row || lhs.col != rhs.col;
}

class SpStation {

private:
    Cube test;
    vector<vector<vector<Cube>>> map3D;

    deckdata startLoc;
    deckdata hangarLoc;
    deque<deckdata> deck;

    uint32_t size;
    uint32_t level;
    char oMode = 'M';
    bool solved = false;

public:
    SpStation(uint32_t level, uint32_t size) {
        // Initialize the x by y by z vector and fill with empty cubes
        map3D.resize(level);
        for (uint32_t i = 0; i < level; i++) {
            map3D[i].resize(size);
            for (uint32_t j = 0; j < size; j++) {
                map3D[i][j].resize(size, test);
            }
        }

        this->level = level;
        this->size = size;
    }


    void setOmode(char mode) {
        this->oMode = mode;
    }
    void setMap3D(char input, uint32_t lvl, uint32_t row, uint32_t col) {
        if (input == 'S') {
            startLoc = { lvl, row, col };
        }
        else if (input == 'H') {
            hangarLoc = { lvl, row, col };
        }
        else if (!(input == '.' || input == '#' || input == 'E')) {
            cerr << "Invalid map character" << endl;
            exit(1);
        }
        map3D[lvl][row][col] = {11, input, 'x', false};
    }

    void findPath(char mode) {
        deck.push_back(startLoc);
        map3D[startLoc.lvl][startLoc.row][startLoc.col].dis = true;
        deckdata nxtLoc;
        while (deck.size() != 0) { // investigator loop
            if (mode == 's') {
                nxtLoc = { deck.back().lvl, deck.back().row, deck.back().col };
                deck.pop_back();
            }
            else if (mode == 'q') {
                nxtLoc = { deck.front().lvl, deck.front().row, deck.front().col };
                deck.pop_front();
            }


            uint32_t l = nxtLoc.lvl, r = nxtLoc.row, c = nxtLoc.col;

            if (nxtLoc.row > 0) { // north
                if (map3D[l][r - 1][c].x == 'H') {
                    map3D[l][r - 1][c].dis = true;
                    map3D[l][r - 1][c].fromPrev = 'n';
                    solved = true;
                    break;
                }
                else if (map3D[l][r - 1][c].x != '#' && map3D[l][r - 1][c].dis == false) {
                    map3D[l][r - 1][c].dis = true;
                    map3D[l][r - 1][c].fromPrev = 'n';
                    deck.push_back({ l, r - 1, c });
                } // if not # and discovered
            } // if not edge

            if (nxtLoc.col < (size - 1)) { // east

                if (map3D[l][r][c + 1].x == 'H') {
                    map3D[l][r][c + 1].dis = true;
                    map3D[l][r][c + 1].fromPrev = 'e';
                    solved = true;
                    break;
                }
                else if (map3D[l][r][c + 1].x != '#' && map3D[l][r][c + 1].dis == false) {
                    map3D[l][r][c + 1].dis = true;
                    map3D[l][r][c + 1].fromPrev = 'e';
                    deck.push_back({ l, r, c + 1 });
                } // if not # and discovered
            } // if not edge

            if (nxtLoc.row < (size - 1)) { // south

                if (map3D[l][r + 1][c].x == 'H') {
                    map3D[l][r + 1][c].dis = true;
                    map3D[l][r + 1][c].fromPrev = 's';
                    solved = true;
                    break;
                }
                else if (map3D[l][r + 1][c].x != '#' && map3D[l][r + 1][c].dis == false) {
                    map3D[l][r + 1][c].dis = true;
                    map3D[l][r + 1][c].fromPrev = 's';
                    deck.push_back({ l, r + 1, c });
                } // if not # and discovered
            } // if not edge

            if (nxtLoc.col > 0) { // west
                
                if (map3D[l][r][c - 1].x == 'H') {
                    map3D[l][r][c - 1].dis = true;
                    map3D[l][r][c - 1].fromPrev = 'w';
                    solved = true;
                    break;
                }
                else if (map3D[l][r][c - 1].x != '#' && map3D[l][r][c - 1].dis == false) {
                    map3D[l][r][c - 1].dis = true;
                    map3D[l][r][c - 1].fromPrev = 'w';
                    deck.push_back({ l, r, c - 1 });
                } // if not # and discovered
            } // if not edge

            if (map3D[l][r][c].x == 'E') {
                // discover all same row and column of 0 to highest level.
                for (uint32_t i = 0; i < level; ++i) {

                    // if any other level indeed has 'E', and is not discovered,
                        // add them to search container, mark as discovered
                    if (map3D[i][r][c].x == 'E' && map3D[i][r][c].dis == false) {
                        deck.push_back({ i, r, c });
                        map3D[i][r][c].dis = true;
                        map3D[i][r][c].fromLev = static_cast<uint8_t>(l);
                    }
                } // for
            }
        } // while deck size 0
    } // find Path

    void backTracker() {
        if (!solved) {
            return;
        } // if
        uint32_t l = hangarLoc.lvl, r = hangarLoc.row, c = hangarLoc.col;
        deckdata curLoc = { l, r, c };
        uint32_t prevLev;
        char dir = map3D[l][r][c].fromPrev;
        bool wasElev = false;
        while (curLoc != startLoc) { // problem
            if (wasElev && map3D[l][r][c].fromLev != 11) {
                prevLev = l;
                l = map3D[l][r][c].fromLev;
                map3D[l][r][c].x = static_cast<char>('0' + prevLev);
                dir = map3D[l][r][c].fromPrev; // added this line
            }
            if (dir == 'n') {
                dir = map3D[l][r += 1][c].fromPrev;
                if (map3D[l][r][c].x == 'E') {
                    wasElev = true;
                }
                map3D[l][r][c].x = 'n';
            }
            else if (dir == 'e') {
                dir = map3D[l][r][c -= 1].fromPrev;
                if (map3D[l][r][c].x == 'E') {
                    wasElev = true;
                }
                map3D[l][r][c].x = 'e';
            }
            else if (dir == 's') {
                dir = map3D[l][r -= 1][c].fromPrev;
                if (map3D[l][r][c].x == 'E') {
                    wasElev = true;
                }
                map3D[l][r][c].x = 's';
            }
            else if (dir == 'w') {
                dir = map3D[l][r][c += 1].fromPrev;
                if (map3D[l][r][c].x == 'E') {
                    wasElev = true;
                }
                map3D[l][r][c].x = 'w';
            }
            curLoc = { l, r, c };
        }
    }

    void printMap() {
        
        if (oMode == 'L') {
            if (solved == false) {
                cout << "//path taken" << endl;
                return;
            }
            uint32_t l = startLoc.lvl, r = startLoc.row, c = startLoc.col;
            char dir = map3D[l][r][c].x;
            cout << "//path taken\n";
            while (map3D[l][r][c].x != 'H') {
                if (dir == 'n') {
                    cout << "("
                        << l << "," << r << "," << c << ","
                        << "n)\n";
                    dir = map3D[l][r -= 1][c].x;
                }
                else if (dir == 'e') {
                    cout << "("
                        << l << "," << r << "," << c << ","
                        << "e)\n";
                    dir = map3D[l][r][c += 1].x;
                }
                else if (dir == 's') {
                    cout << "("
                        << l << "," << r << "," << c << ","
                        << "s)\n";
                    dir = map3D[l][r += 1][c].x;
                }
                else if (dir == 'w') {
                    cout << "("
                        << l << "," << r << "," << c << ","
                        << "w)\n";
                    dir = map3D[l][r][c -= 1].x;
                } 
                else {
                uint32_t i = static_cast<uint32_t>(dir - '0');
                cout << "("
                    << l << "," << r << "," << c << ","
                    << i << ")\n";
                l = i;
                dir = map3D[l][r][c].x;
                }
            } // while
        } // if output mode L
        else {
            cout << "Start in level " << startLoc.lvl
                << ", row " << startLoc.row
                << ", column " << startLoc.col << "\n";
            for (uint32_t i = 0; i < level; ++i) {
                cout << "//level " << i << "\n";
                for (uint32_t j = 0; j < size; ++j) {
                    for (uint32_t k = 0; k < size; ++k) {
                        cout << map3D[i][j][k].x;
                    }
                    cout << "\n";
                }
            }
        } // else   
    }
}; //SpStation


// Print help for the user when requested.
// argv[0] is the name of the currently executing program
void printHelp() {
    cout << "Usage: ./ship -q (queue) or -s (stack) -o M (map output)"
         << " or L(coordinate output) < input file name > output file name" << endl;
    exit(0);
}  // printHelp()



void getMode(int argc, char* argv[], SpStation & spstation) {
    // These are used with getopt_long()
    opterr = false; // Let us handle all error output for command line options
    int choice;
    int index = 0;
    bool stackOpt = false;
    bool queueOpt = false;
    struct option long_options[] = {
        // Fill in two lines, for the "mode" ('m') and
        // the "help" ('h') options.
        // ./project0 -m nosize
        // ./project0 --help
        { "stack",  no_argument,       nullptr, 's'  },
        { "queue",  no_argument,       nullptr, 'q'  },
        { "help",   no_argument,       nullptr, 'h'  },
        { "output", required_argument, nullptr, 'o'  },
        { nullptr,  0,                 nullptr, '\0' },
    };  // long_options[]

    // Fill in the double quotes, to match the mode and help options.
    while ((choice = getopt_long(argc, argv, "sqho:", long_options, &index)) != -1) {
        switch (choice) {
            
            case 's': 
                stackOpt = true;
                if (queueOpt == true) {
                    cout << "Multiple routing modes specified << endl";
                    exit(1);
                }
                spstation.findPath('s');
                break;
                
            case 'q': 
                queueOpt = true;
                if (stackOpt == true) {
                    cerr << "Multiple routing modes specified << endl";
                    exit(1);
                }
                spstation.findPath('q');
                break;

            case 'h':
                printHelp();
                exit(0);

            case 'o': {  // Need a block here to declare a variable inside a case
                string arg{ optarg }; // same as: string arg = optarg
                if (arg != "M" && arg != "L") {
                    // The first line of error output has to be a 'fixed' message
                    // for the autograder to show it to you.
                    cerr << "Invalid output mode specified" << endl;
                    exit(1);
                }  // if ..arg not valid

                else if (arg == "M") {
                    spstation.setOmode('M');
                }
                else { //if arg == "L"
                    spstation.setOmode('L');
                } // if ..arg
                break;
            }  // case 'm'
        }  // switch ..choice
    }  // while

    if (stackOpt == false && queueOpt == false) {
        cerr << "No routing mode specified\n";
        exit(1);
    }
}  // getMode()


int main(int argc, char *argv[]) {
    std::ios_base::sync_with_stdio(false);

    
    string junkstr;
    uint32_t curLvl = 0, curRow = 0, curCol = 0;
    uint32_t level;
    uint32_t size;
    char inputMode;
    char input;
    char junk;
    cin >> inputMode >> level >> size;

    SpStation spstation(level, size);
    
    if (inputMode == 'M') {
        while (cin >> input) {
            if (input == '/') {
                getline(cin, junkstr);
                continue;
            }
            spstation.setMap3D(input, curLvl, curRow, curCol);
            if (curRow == (size - 1) && curCol == (size - 1)) {
                curLvl = (curLvl + 1);
            }

            if (curCol == (size - 1)) {
                curRow = (curRow + 1) % size;
            }

            curCol = (curCol + 1) % size;
        } // while
    } // if input M
    else if (inputMode == 'L') {
        while (cin >> input) {
            if (input == '/') {
                getline(cin, junkstr);
                continue;
            }
            if (input == '(') {
                cin >> curLvl >> junk >> curRow >> junk >> curCol >> junk >> input >> junk;
                if (curLvl > level) {
                    cerr << "Invalid map level" << endl;
                    exit(1);
                }
                if (curRow >= size) {
                    cerr << "Invalid map row" << endl;
                    exit(1);
                }
                if (curCol >= size) {
                    cerr << "Invalid map column" << endl;
                    exit(1);
                }
            }
            spstation.setMap3D(input, curLvl, curRow, curCol);
        } // while
        //spstation.listInputHelper();
    } // else if input L
    else {
        cerr << "Error: Wrong input mode in input file" << endl;
        cout << "Error: Wrong input mode in input file" << endl;
    } // else

    getMode(argc, argv, spstation);
    spstation.backTracker();
    spstation.printMap();


    
} // main