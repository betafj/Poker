
class Deck{
	public:
		Deck(){
			for (int i =0; i < 52; i++) 
				cards[i]=i;
			top = 0; 
		}
		void shuffle(int num_swaps);
		int get_top(){
			int r = cards[top];
			top = (top +1 ) %52;
			return r;
		}
		
		void reset(){top = 0;}

	private:
		int cards[52];
		int top;
	
	


};
