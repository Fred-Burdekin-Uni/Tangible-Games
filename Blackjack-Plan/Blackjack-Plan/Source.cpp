#include <iostream> 
#include <vector>

using namespace std;


void setup(bool& gamePlaying, vector<int>& cards, int& player1Hand, int& player2Hand, int& player3Hand) {
	cards = { 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };
	gamePlaying = true; // Set to true to start the game 
	player1Hand = 0;
	player2Hand = 0;
	player3Hand = 0;

}


int main() {

	// Defining the variables
	bool dealing, player1Drawing = true, player2Drawing = true, player3Drawing = true, player1Bust = false, player2Bust = false, player3Bust = false;
	vector<int> cards;
	int player1Hand, player2Hand, player3Hand, userInput, draw;

	// call setup function
	setup(dealing, cards, player1Hand, player2Hand, player3Hand);

	// Initialize random seed 
	srand(static_cast<unsigned>(time(0)));



    if (dealing) {
        for (int i = 0; i < 2; i++) {
            draw = rand() % cards.size();
            player1Hand += cards[draw];
            cards.erase(cards.begin() + draw);

            draw = rand() % cards.size();
            player2Hand += cards[draw];
            cards.erase(cards.begin() + draw);

            draw = rand() % cards.size();
            player3Hand += cards[draw];
            cards.erase(cards.begin() + draw);
        }

        cout << "Starting Hand for player 1: " << player1Hand << endl;
        cout << "Starting Hand for player 2: " << player2Hand << endl;
        cout << "Starting Hand for player 3: " << player3Hand << endl;

        dealing = false;
    }
    while (player1Drawing) {
        if (player1Hand <= 21) {
            cout << "Player 1 Would you like to draw 1 = yes 0 = no" << endl;
            cin >> userInput;

            if (userInput == 1) {
                draw = rand() % cards.size();
                player1Hand += cards[draw];
                cards.erase(cards.begin() + draw);
                cout << player1Hand << endl;
            }
            else if (userInput == 0) {
                player1Drawing = false;
            }
            else {
                cout << "invalid input" << endl;
            }
        }
        else {
            cout << "PLayer went bust" << endl;
            player1Drawing = false;
            player1Bust = true;

            break;
        }
    }


    while (player2Drawing) {
        if (player2Hand <= 21) {
            cout << "Player 2 Would you like to draw 1 = yes 0 = no" << endl;
            cin >> userInput;

            if (userInput == 1) {
                draw = rand() % cards.size();
                player2Hand += cards[draw];
                cards.erase(cards.begin() + draw);
                cout << player2Hand << endl;
            }
            else if (userInput == 0) {
                player2Drawing = false;
            }
            else {
                cout << "invalid input" << endl;
            }
        }
        else {
            cout << "Player went bust" << endl;
            player2Drawing = false;
            player2Bust = true;

            break;
        }
    }



    while (player3Drawing) {
        if (player3Hand <= 21) {
            cout << "Player 3 Would you like to draw 1 = yes 0 = no" << endl;
            cin >> userInput;

            if (userInput == 1) {
                draw = rand() % cards.size();
                player3Hand += cards[draw];
                cards.erase(cards.begin() + draw);
                cout << player3Hand << endl;
            }
            else if (userInput == 0) {
                player3Drawing = false;
            }
            else {
                cout << "invalid input" << endl;
            }
        }
        else {
            cout << "Player went bust" << endl;
            player3Drawing = false;
            player3Bust = true;

            break;
        }
    }


    if (!player1Drawing && !player2Drawing && !player3Drawing) {

        if (player1Hand <= 21 && player1Hand > player2Hand && player1Hand > player3Hand || !player1Bust && player2Bust && player3Bust) {
            cout << "Player 1 wins" << endl;
        }

        if (player2Hand <= 21 && player2Hand > player1Hand && player2Hand > player3Hand || player1Bust && !player2Bust && player3Bust) {
            cout << "Player 2 wins" << endl;
        }

        if (player3Hand <= 21 && player3Hand > player1Hand && player3Hand > player2Hand || player1Bust && player2Bust && !player3Bust) {
            cout << "Player 3 wins" << endl;
        }

        if (player1Hand == player2Hand == player3Hand) {
            cout << "All players draw";
        }

        if (player1Hand == player2Hand) {
            cout << "player 1 and 2 draw";
        }

        if (player2Hand == player3Hand) {
            cout << "player 2 and 3 draw";
        }

        if (player1Hand == player3Hand) {
            cout << "player 1 and 3 draw";
        }
    }

	return 0;
}
