/*
Name: Edward Garay
Date: 05/1/25
Purpose: Egyptian_V2
*/

//System Libraries
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cmath>
using namespace std;

// Function Prototypes
void rstDeck(bool used[], int size);
int drawIdx(bool used[], int size);
string crdName(int val);
void shwStat(int win, int pts, int face, float avg, int mxStrk, int ply);
void logTime(ofstream &f);

int main(int argc, char **argv){
    srand(static_cast<unsigned int>(time(0)));

    const int RNDMAX = 100;
    const int DEKSIZ = 52;

    bool used[DEKSIZ] = {false};

    int c1Hist[RNDMAX], c2Hist[RNDMAX];
    int s1Hist[RNDMAX], s2Hist[RNDMAX];
    char winHis[RNDMAX];
    string su1His[RNDMAX], su2His[RNDMAX];

    int face1 = 0, face2 = 0;
    int sumS1 = 0, sumS2 = 0;
    int strk1 = 0, strk2 = 0, mxS1 = 0, mxS2 = 0;

    string suit[4] = {"♣", "♦", "♥", "♠"};

    int val1 = 0, val2 = 0;
    int slap1 = 0, slap2 = 0;
    int win1 = 0, win2 = 0, tie = 0;
    int pts1 = 0, pts2 = 0;
    int rnd = 1;
    char cont = 'y';
    string crd1 = "", crd2 = "";

    string p1, p2;
    cout << "Enter name for Player 1: ";
    getline(cin, p1);
    cout << "Enter name for Player 2: ";
    getline(cin, p2);

    ofstream f;
    f.open("ers_log.txt");
    f << fixed;
    cout << fixed;

    logTime(f);

    cout << "===============================\n";
    cout << " Egyptian\n";
    cout << "===============================\n\n";
    cout << "Players draw cards and slap on matches.\n";
    cout << "Slap speed is simulated (1=fastest, 10=slowest).\n\n";
    f << "EGYPTIAN - GAME LOG\n\n";

    while(cont == 'y' || cont == 'Y'){
        int cnt = 0;
        for(int i = 0; i < DEKSIZ; i++) if(used[i]) cnt++;
        if(cnt >= DEKSIZ - 2) rstDeck(used, DEKSIZ);

        int idx1 = drawIdx(used, DEKSIZ);
        int idx2 = drawIdx(used, DEKSIZ);

        val1 = (idx1 % 13) + 1;
        val2 = (idx2 % 13) + 1;
        string s1 = suit[idx1 / 13];
        string s2 = suit[idx2 / 13];

        crd1 = crdName(val1);
        crd2 = crdName(val2);

        slap1 = 1 + rand() % 10;
        slap2 = 1 + rand() % 10;

        c1Hist[rnd - 1] = val1;
        c2Hist[rnd - 1] = val2;
        s1Hist[rnd - 1] = slap1;
        s2Hist[rnd - 1] = slap2;
        su1His[rnd - 1] = s1;
        su2His[rnd - 1] = s2;
        sumS1 += slap1;
        sumS2 += slap2;

        if(val1 >= 11) face1++;
        if(val2 >= 11) face2++;

        cout << "Round #" << rnd << "\n";
        cout << p1 << " draws: " << crd1 << s1 << " (slap: " << slap1 << ")\n";
        cout << p2 << " draws: " << crd2 << s2 << " (slap: " << slap2 << ")\n";

        f << "Round #" << rnd << ": ";
        f << p1 << "[" << crd1 << s1 << "," << slap1 << "] ";
        f << p2 << "[" << crd2 << s2 << "," << slap2 << "] ";

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

        if(rnd < RNDMAX){
            cout << "Play next round? (y/n): ";
            cin >> cont;
            cin.ignore();
        } else {
            cont = 'n';
        }

        char tmp;
        cout << "Press any key and Enter to continue...\n";
        cin >> tmp;
        cin.ignore();

        cout << "=============================\n";
        rnd++;
    }

    cout << "\n=== FINAL RESULTS ===\n";
    cout << "Rounds: " << rnd - 1 << "\n";
    shwStat(win1, pts1, face1, static_cast<float>(sumS1)/(rnd - 1), mxS1, 1);
    shwStat(win2, pts2, face2, static_cast<float>(sumS2)/(rnd - 1), mxS2, 2);
    cout << "Ties: " << tie << "\n";

    f << "\n=== FINAL SUMMARY ===\n";
    f << "Rounds: " << rnd - 1 << "\n";
    f << p1 << ": Wins = " << win1 << ", Points = " << pts1 << ", Face Cards = " << face1 << ", Avg Slap = " << static_cast<float>(sumS1)/(rnd - 1) << ", Max Streak = " << mxS1 << "\n";
    f << p2 << ": Wins = " << win2 << ", Points = " << pts2 << ", Face Cards = " << face2 << ", Avg Slap = " << static_cast<float>(sumS2)/(rnd - 1) << ", Max Streak = " << mxS2 << "\n";
    f << "Ties: " << tie << "\n";

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

    f << "END OF GAME\n";
    f.close();
    cout << "\nThanks for playing! Game log saved to ers_log.txt\n";
    return 0;
}

void rstDeck(bool used[], int size) {
    for(int i = 0; i < size; i++) used[i] = false;
}

int drawIdx(bool used[], int size) {
    int idx;
    do {
        idx = rand() % size;
    } while(used[idx]);
    used[idx] = true;
    return idx;
}

string crdName(int val) {
    switch(val) {
        case 1: return "Ace";
        case 11: return "Jack";
        case 12: return "Queen";
        case 13: return "King";
        default: return to_string(val);
    }
}

void shwStat(int win, int pts, int face, float avg, int mxStrk, int ply) {
    cout << "Player " << ply << ": Wins = " << win
         << ", Points = " << pts
         << ", Face Cards = " << face
         << ", Avg Slap = " << avg
         << ", Max Streak = " << mxStrk << "\n";
}

void logTime(ofstream &f) {
    time_t now = time(0);
    f << "Game started at: " << ctime(&now) << endl;
}
