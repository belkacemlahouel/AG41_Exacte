#ifndef __BATCH__
#define __BATCH__

class Batch{

	private:
		vector<Produit*> batch;
		
	public:
		Batch();
		~Batch(){}
		void addProduit(Produit*);
		int dateDueBatch();
		


};

#endif // BATCH