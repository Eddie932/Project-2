/*
Name: Edward Garay
Date: 05/1/25
Purpose: Egyptian_V1
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

int main(int argv, char **argc){
    //Random seed
    srand(static_cast<unsigned int>(time(0)));

    //Variables
    int card1 = 0, card2 = 0;
    char suit1 = ' ', suit2 = ' ';
    int slap1 = 0, slap2 = 0;
    int win1 = 0, win2 = 0, ties = 0;
    int pts1 = 0, pts2 = 0;
    int rnd = 1;
    char cont = 'y';
    string crd1 = "", crd2 = "";

    ofstream file;
    file.open("ers_log.txt");
    file << fixed;
    cout << fixed;

    //Intro
    cout << "===============================\n";
    cout << " Egyptian\n";
    cout << "===============================\n\n";
    cout << "Players draw cards and slap on matches.\n";
    cout << "Slap speed is simulated (1=fastest, 10=slowest).\n\n";
    file << "EGYPTIAN - GAME LOG\n\n";

    while(cont == 'y' || cont == 'Y'){
        //Draw card values (1–13 for Ace to King)
        card1 = 1 + rand() % 13;
        card2 = 1 + rand() % 13;

        //Convert to string names
        crd1 = (card1 == 1) ? "Ace" :
               (card1 == 11) ? "Jack" :
               (card1 == 12) ? "Queen" :
               (card1 == 13) ? "King" :
               to_string(card1);

        crd2 = (card2 == 1) ? "Ace" :
               (card2 == 11) ? "Jack" :
               (card2 == 12) ? "Queen" :
               (card2 == 13) ? "King" :
               to_string(card2);

        //Draw suits
        suit1 = 3 + rand() % 4;
        suit2 = 3 + rand() % 4;

        //Simulate slap speeds (1-10)
        slap1 = 1 + rand() % 10;
        slap2 = 1 + rand() % 10;

        //Display round
        cout << "Round #" << rnd << "\n";
        cout << "Player 1 draws: " << crd1 << char(suit1) << " (slap: " << slap1 << ")\n";
        cout << "Player 2 draws: " << crd2 << char(suit2) << " (slap: " << slap2 << ")\n";

        //Log round
        file << "Round #" << rnd << ": ";
        file << "P1[" << crd1 << "," << slap1 << "] ";
        file << "P2[" << crd2 << "," << slap2 << "] ";

        //Match check
        if(card1 == card2){
            cout << "It's a MATCH! Slap!!\n";
            if(slap1 < slap2){
                cout << "Player 1 slapped faster!\n";
                win1++;
                pts1 += 2;
                file << "-> P1 wins\n";
            } else if(slap2 < slap1){
                cout << "Player 2 slapped faster!\n";
                win2++;
                pts2 += 2;
                file << "-> P2 wins\n";
            } else if(abs(slap1 - slap2) <= 1){
                cout << "Tie! Slap speeds were close.\n";
                ties++;
                file << "-> Tie\n";
            } else {
                cout << "Unclear result.\n";
                ties++;
                file << "-> Tie\n";
            }
        } else {
            cout << "No match. Round ends.\n";
            file << "-> No match\n";
        }

        //Flavor output
        if(card1 >= 11 && card1 <= 13){
            cout << "Player 1 drew a face card!\n";
        }
        if(card2 >= 11 && card2 <= 13){
            cout << "Player 2 drew a face card!\n";
        }

        if(card1 == 1 && slap1 < 4){
            cout << "Ace slap bonus for Player 1!\n";
        }
        if(card2 == 1 && slap2 < 4){
            cout << "Ace slap bonus for Player 2!\n";
        }

        if(slap1 > 8){
            cout << "Player 1 hesitated...\n";
        }
        if(slap2 > 8){
            cout << "Player 2 hesitated...\n";
        }

        //Live score
        cout << "-----------------------------\n";
        cout << "Score:\n";
        cout << "Player 1: " << pts1 << " pts, Wins: " << win1 << "\n";
        cout << "Player 2: " << pts2 << " pts, Wins: " << win2 << "\n";
        cout << "Ties: " << ties << "\n";

        file << "Score: P1=" << pts1 << ", P2=" << pts2 << ", Ties=" << ties << "\n";

        //Commentary
        int talk = rand() % 5;
        if(talk == 0) cout << "That round was wild!\n";
        else if(talk == 1) cout << "Slap speed was close!\n";
        else if(talk == 2) cout << "They’re giving it their all!\n";
        else if(talk == 3) cout << "Nice reflexes!\n";
        else cout << "Who's going to win this?\n";

        //Continue
        if(rnd < 15){
            cout << "Play next round? (y/n): ";
            cin >> cont;
        } else {
            cont = 'n';
        }

        //Pause
        char dummy;
        cout << "Press any key and Enter to continue...\n";
        cin >> dummy;

        cout << "=============================\n";
        rnd++;
    }

    //Final results
    cout << "\n=== FINAL RESULTS ===\n";
    cout << "Rounds: " << rnd - 1 << "\n";
    cout << "Player 1: Wins = " << win1 << ", Points = " << pts1 << "\n";
    cout << "Player 2: Wins = " << win2 << ", Points = " << pts2 << "\n";
    cout << "Ties: " << ties << "\n";

    file << "\n=== FINAL SUMMARY ===\n";
    file << "Rounds: " << rnd - 1 << "\n";
    file << "Player 1: Wins = " << win1 << ", Points = " << pts1 << "\n";
    file << "Player 2: Wins = " << win2 << ", Points = " << pts2 << "\n";
    file << "Ties: " << ties << "\n";
    file << "END OF GAME\n";

    file.close();
    cout << "\nThanks for playing! Game log saved to ers_log.txt\n";
    return 0;
}
