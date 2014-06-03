void Parser::parseFile(string name){

	ifstream stream;
	stream.open (name.c_str());

	if(stream.is_open()){

		char* buf[256];
		string line;
		string desc; // la description de la donnée
		string data; // la donnée en elle même
		cout <<"Fichier ouvert, demarrage du parsage.\n";

		while(std::getline(stream,line)){ // stream.eof() deprecated

			desc = line.substr(0,line.find(":"));
			remove_spaces(desc);
			
			/* le comportement du parseur sera différent en fonction de la description de la ligne */
			if(strcmp(desc.c_str(),"NBR_PRODUCT") == 0){

				data = line.substr(line.find(":"),line.find("\n"));

				cout << "Nombre de produits : " << data <<"\n";

				// assigner n au problème

			} else if(strcmp(desc.c_str(),"TRANSPORTER_CAPACITY") == 0){

				data = line.substr(line.find(":"),line.find("\n"));

				cout << "Capacite du transporteur : " << data <<"\n";

			} else if(strcmp(desc.c_str(),"TRANSPORTER_DELIVERY_COST_ETA") == 0){

				data = line.substr(line.find(":"),line.find("\n"));

				cout << "Cout de transport (eta) : " << data <<"\n";

			} else if(strcmp(desc.c_str(),"CUSTOMER") == 0){
			/* si c'est un client, on regarde immédiatement les données le concernant après */


			} else if(strcmp(desc.c_str(),"JOB_CUSTOMER") == 0){

			} else if(strcmp(desc.c_str(),"JOB_DUE_DATES") == 0){

			}
		}

		cout<<"Parsage fini.\n";
		stream.close();
	} else {

		cout<<"Impossible d'ouvrir le fichier.\n";
		exit(-1);
	}
}

/* Supprime tous les espaces "parasites" dans la string str */

void Parser::remove_spaces(string &input){
	input.erase(std::remove(input.begin(),input.end(),' '),input.end());
}