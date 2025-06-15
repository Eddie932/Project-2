/*
Name: Edward Garay
Date: 05/1/25
Purpose: Egyptian_V5
*/

//System Libraries
#include <iostream>     // Console I/O
#include <fstream>      // File I/O
#include <iomanip>      // Output formatting
#include <cstdlib>      // Random number generation
#include <ctime>        // Time functions
#include <string>       // String manipulation
#include <cmath>        // Math utilities
using namespace std;

// Function Prototypes
void rstDeck(bool used[], int size);                             // Reset deck usage
int drawIdx(bool used[], int size);                              // Draw a unique card index
string crdName(int val);                                         // Convert card number to name
void shwStat(int win, int pts, int face, float avg, int mxStrk, int ply); // Show stats per player
void logTime(ofstream &f);                                       // Log start time
void rptStks(ofstream &f, int win1, int win2, int pts1, int pts2, int face1, int face2, float avg1, float avg2, int mxS1, int mxS2, string p1, string p2); // Report end-of-game stats
void printDeckUsage(bool used[], int size);                      // Print which cards have been used
float calcWinRate(int wins, int rounds);                         // Calculate win rate percentage
string getWinner(string p1, string p2, int pts1, int pts2);      // Determine overall winner

int main(int argc, char **argv){
    srand(static_cast<unsigned int>(time(0)));                   // Seed random number generator

    const int RNDMAX = 100;                                      // Max number of rounds
    const int DEKSIZ = 52;                                       // Number of cards in deck

    bool used[DEKSIZ] = {false};                                 // Array to track used cards

    // Arrays for tracking each round's history
    int c1Hist[RNDMAX], c2Hist[RNDMAX];                          // Card values per round
    int s1Hist[RNDMAX], s2Hist[RNDMAX];                          // Slap speeds
    char winHis[RNDMAX];                                         // Win result per round
    string su1His[RNDMAX], su2His[RNDMAX];                       // Suit history

    // Stats tracking
    int face1 = 0, face2 = 0;                                    // Count of face cards drawn
    int sumS1 = 0, sumS2 = 0;                                    // Total slap speed sum
    int strk1 = 0, strk2 = 0, mxS1 = 0, mxS2 = 0;                // Streak tracking

    string suit[4] = {"\u2663", "\u2666", "\u2665", "\u2660"};   // Unicode suits: Clubs, Diamonds, Hearts, Spades

    // Round variables
    int val1 = 0, val2 = 0;                                      // Card face values
    int slap1 = 0, slap2 = 0;                                    // Slap speeds
    int win1 = 0, win2 = 0, tie = 0;                             // Win/tie counters
    int pts1 = 0, pts2 = 0;                                      // Points
    int rnd = 1;                                                 // Round counter
    char cont = 'y';                                             // Continue flag
    string crd1 = "", crd2 = "";                                 // Card name strings

    // Player input
    string p1, p2;
    cout << "Enter name for Player 1: ";
    getline(cin, p1);
    cout << "Enter name for Player 2: ";
    getline(cin, p2);

    // File setup
    ofstream f;
    f.open("ers_log.txt");
    f << fixed;
    cout << fixed;

    logTime(f);                                                  // Log start time in file

    // Game intro
    cout << "===============================\n";
    cout << " Egyptian\n";
    cout << "===============================\n\n";
    cout << "Players draw cards and slap on matches.\n";
    cout << "Slap speed is simulated (1=fastest, 10=slowest).\n\n";
    f << "EGYPTIAN - GAME LOG\n\n";

    // Game loop
    while(cont == 'y' || cont == 'Y'){
        int cnt = 0;
        for(int i = 0; i < DEKSIZ; i++) if(used[i]) cnt++;       // Count used cards
        if(cnt >= DEKSIZ - 2) rstDeck(used, DEKSIZ);             // Reset deck if nearly full

        // Draw two cards
        int idx1 = drawIdx(used, DEKSIZ);
        int idx2 = drawIdx(used, DEKSIZ);

        // Determine card values and suits
        val1 = (idx1 % 13) + 1;
        val2 = (idx2 % 13) + 1;
        string s1 = suit[idx1 / 13];
        string s2 = suit[idx2 / 13];

        // Convert value to card name
        crd1 = crdName(val1);
        crd2 = crdName(val2);

        // Simulate slap speeds
        slap1 = 1 + rand() % 10;
        slap2 = 1 + rand() % 10;

        // Save round history
        c1Hist[rnd - 1] = val1;
        c2Hist[rnd - 1] = val2;
        s1Hist[rnd - 1] = slap1;
        s2Hist[rnd - 1] = slap2;
        su1His[rnd - 1] = s1;
        su2His[rnd - 1] = s2;
        sumS1 += slap1;
        sumS2 += slap2;

        // Track face cards
        if(val1 >= 11) face1++;
        if(val2 >= 11) face2++;

        // Display round info
        cout << "Round #" << rnd << "\n";
        cout << p1 << " draws: " << crd1 << s1 << " (slap: " << slap1 << ")\n";
        cout << p2 << " draws: " << crd2 << s2 << " (slap: " << slap2 << ")\n";

        // Log round to file
        f << "Round #" << rnd << ": ";
        f << p1 << "[" << crd1 << s1 << "," << slap1 << "] ";
        f << p2 << "[" << crd2 << s2 << "," << slap2 << "] ";

        // Check for match
        if(val1 == val2){
            cout << "It's a MATCH! Slap!!\n";
            if(slap1 < slap2){
                cout << p1 << " slapped faster!\n";
                win1++; pts1 += 2;
                winHis[rnd - 1] = '1';
                strk1++; strk2 = 0;
                mxS1 = max(mxS1, strk1);
                f << "-> " << p1 << " wins\n";
            } else if(slap2 < slap1){
                cout << p2 << " slapped faster!\n";
                win2++; pts2 += 2;
                winHis[rnd - 1] = '2';
                strk2++; strk1 = 0;
                mxS2 = max(mxS2, strk2);
                f << "-> " << p2 << " wins\n";
            } else {
                cout << "Tie! Slap speeds were close.\n";
                tie++;
                winHis[rnd - 1] = 'T';
                f << "-> Tie\n";
                strk1 = strk2 = 0;
            }
        } else {
            cout << "No match. Round ends.\n";
            winHis[rnd - 1] = 'N';
            f << "-> No match\n";
            strk1 = strk2 = 0;
        }

        printDeckUsage(used, DEKSIZ);                            // Show card usage

        // Ask if user wants to continue
        if(rnd < RNDMAX){
            cout << "Play next round? (y/n): ";
            cin >> cont;
            cin.ignore();
        } else {
            cont = 'n';
        }

        // Pause before next round
        char tmp;
        cout << "Press any key and Enter to continue...\n";
        cin >> tmp;
        cin.ignore();

        cout << "=============================\n";
        rnd++;
    }

    // Final Results
    cout << "\n=== FINAL RESULTS ===\n";
    cout << "Rounds: " << rnd - 1 << "\n";
    shwStat(win1, pts1, face1, static_cast<float>(sumS1)/(rnd - 1), mxS1, 1);
    shwStat(win2, pts2, face2, static_cast<float>(sumS2)/(rnd - 1), mxS2, 2);
    cout << "Ties: " << tie << "\n";

    // Log final summary
    f << "\n=== FINAL SUMMARY ===\n";
    rptStks(f, win1, win2, pts1, pts2, face1, face2, static_cast<float>(sumS1)/(rnd - 1), static_cast<float>(sumS2)/(rnd - 1), mxS1, mxS2, p1, p2);

    // Log round-by-round history
    f << "\n=== ROUND HISTORY ===\n";
    for(int i = 0; i < rnd - 1; i++){
        f << "Round " << i + 1 << ": ";
        f << p1 << "=" << c1Hist[i] << su1His[i] << " (slap " << s1Hist[i] << "), ";
        f << p2 << "=" << c2Hist[i] << su2His[i] << " (slap " << s2Hist[i] << ") -> ";
        if(winHis[i] == '1') f << p1 << " wins\n";
        else if(winHis[i] == '2') f << p2 << " wins\n";
        else if(winHis[i] == 'T') f << "Tie\n";
        else f << "No match\n";
    }

    // Declare winner
    f << "\nOverall Winner: " << getWinner(p1, p2, pts1, pts2) << "\n";
    f << "END OF GAME\n";
    f.close();
    cout << "\nThanks for playing! Game log saved to ers_log.txt\n";
    return 0;
}

// Resets the entire deck usage array
void rstDeck(bool used[], int size) {
    for(int i = 0; i < size; i++) used[i] = false;
}

// Randomly selects an unused card index
int drawIdx(bool used[], int size) {
    int idx;
    do {
        idx = rand() % size;
    } while(used[idx]);
    used[idx] = true;
    return idx;
}

// Converts card value to its string representation
string crdName(int val) {
    switch(val) {
        case 1: return "Ace";
        case 11: return "Jack";
        case 12: return "Queen";
        case 13: return "King";
        default: return to_string(val);
    }
}

// Displays statistics for a player
void shwStat(int win, int pts, int face, float avg, int mxStrk, int ply) {
    cout << "Player " << ply << ": Wins = " << win
         << ", Points = " << pts
         << ", Face Cards = " << face
         << ", Avg Slap = " << avg
         << ", Max Streak = " << mxStrk << "\n";
}

// Logs the current system time to a file
void logTime(ofstream &f) {
    time_t now = time(0);
    f << "Game started at: " << ctime(&now) << endl;
}

// Outputs game stats to the file after the game ends
void rptStks(ofstream &f, int win1, int win2, int pts1, int pts2, int face1, int face2, float avg1, float avg2, int mxS1, int mxS2, string p1, string p2) {
    f << p1 << ": Wins = " << win1 << ", Points = " << pts1 << ", Face Cards = " << face1 << ", Avg Slap = " << avg1 << ", Max Streak = " << mxS1 << "\n";
    f << p2 << ": Wins = " << win2 << ", Points = " << pts2 << ", Face Cards = " << face2 << ", Avg Slap = " << avg2 << ", Max Streak = " << mxS2 << "\n";
    f << "Ties: " << (win1 + win2 + pts1 + pts2 + face1 + face2 == 0 ? 0 : (100 - win1 - win2)) << "\n";
}

// Shows O/X pattern of deck usage
void printDeckUsage(bool used[], int size) {
    cout << "Deck usage: ";
    for(int i = 0; i < size; i++) {
        cout << (used[i] ? "X" : "O");
        if ((i + 1) % 13 == 0) cout << " ";
    }
    cout << "\n";
}

// Computes win rate as a percentage
float calcWinRate(int wins, int rounds) {
    return rounds > 0 ? (static_cast<float>(wins) / rounds) * 100 : 0.0f;
}

// Returns the name of the player with more points, or "Tie"
string getWinner(string p1, string p2, int pts1, int pts2) {
    if(pts1 > pts2) return p1;
    else if(pts2 > pts1) return p2;
    else return "Tie";
}
