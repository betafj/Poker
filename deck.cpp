#include "deck.h"
#include <iostream>
using namespace std;

void Deck::shuffle(int num_swaps){
	srand(time(NULL));
	int pos1, pos2;
	//
	for (int i =0; i < num_swaps; i++){
		pos1 = rand()%52;
		pos2 = rand()%52; 
		int tmp = cards[pos1];
		cards[pos1] = cards[pos2];
		cards[pos2] = tmp;
	}
	
}
