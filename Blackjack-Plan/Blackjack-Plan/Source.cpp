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
	return 0;
}
