#include "Parser.h"

Parser::Parser(string filename){
    parseFile(filename);
}

Parser::~Parser() {
//	Tools::viderVector(clients);
//	Tools::viderVector(produits);
}

void Parser::parseFile(string name){

	ifstream stream;
	stream.open (name.c_str());

	if(stream.is_open()){

		char* buf[256];
		string line;
		string desc; // la description de la donnée
		string data; // la donnée en elle même
		cout <<"Fichier ouvert, demarrage du parsage.\n";
        m = 0;
        list<int> tempJobList;
        list<double> tempDatesList; // la liste des jobs et de leur date due est créée séparément pour le moment, mais sera reconstitutée à la fin

		while(std::getline(stream,line)){ // stream.eof() deprecated

			desc = line.substr(0,line.find(":"));
			removeSpaces(desc);

			/* le comportement du parseur sera différent en fonction de la description de la ligne */
			if(strcmp(desc.c_str(),"NBR_PRODUCT") == 0){

				data = line.substr(line.find(":"),line.find("\n\n"));
                data.erase(data.begin()); // suppression du ':' dans la chaine
                n = atoi(data.c_str());
				cout << "Nombre de produits : " << n <<"\n\n";

			} else if(strcmp(desc.c_str(),"TRANSPORTER_CAPACITY") == 0){

				data = line.substr(line.find(":"),line.find("\n\n"));
                data.erase(data.begin());
                c = atoi(data.c_str());
				cout << "Capacite du transporteur : " << c <<"\n\n";

			} else if(strcmp(desc.c_str(),"TRANSPORTER_DELIVERY_COST_ETA") == 0){

				data = line.substr(line.find(":"),line.find("\n"));
                data.erase(data.begin());
                eta = atof(data.c_str());
				cout << "Cout de transport (eta) : " << eta <<"\n\n";

			} else if(strcmp(desc.c_str(),"CUSTOMER") == 0){
			/* si c'est un client, on regarde immédiatement les données le concernant après */
                int tempIndex;
                double tempCout;
                float tempDist;

                data = line.substr(line.find(":"),line.find("\n"));
                data.erase(data.begin());
                tempIndex = atoi(data.c_str());

                getline(stream,line); // ligne coûts de stockage

                data = line.substr(line.find(":"),line.find("\n"));
                data.erase(data.begin());
                tempCout = atof(data.c_str());

                getline(stream,line); // ligne distance jusqu'à l'entrepot

                data = line.substr(line.find(":"),line.find("\n"));
                data.erase(data.begin());
                tempDist = atof(data.c_str());

                cout<<"Nouveau Client. ID : "<< tempIndex <<"\n\tCout stockage : "<< tempCout <<"\n\tDistance client-entrepot : "<< tempDist <<"\n";

                clients.push_back(new Client(tempIndex,tempDist,tempCout));
                ++m;

			} else if(strcmp(desc.c_str(),"JOB_CUSTOMER") == 0){

                data = line.substr(line.find(":"),line.find("\n"));
                data.erase(data.begin());
                char* buff = (char *) malloc(sizeof(char)*data.size());
                strcpy(buff,data.c_str());
                char* temp;

                temp = strtok (buff,";");
                while (temp != NULL)
                {
                    tempJobList.push_back(atoi(temp));
                    temp = strtok (NULL, ";");
                }

                cout << "Liste des jobs obtenue : ";
                list<int>::iterator itjobs;
                for(itjobs = tempJobList.begin();itjobs != tempJobList.end();++itjobs){
                    cout<<*itjobs<<" ";
                }
                cout<<"\n\n";

                free(buff);
				// free(temp);

			} else if(strcmp(desc.c_str(),"JOB_DUE_DATES") == 0){
                data = line.substr(line.find(":"),line.find("\n"));
                data.erase(data.begin());
                char* buff = (char *) malloc(sizeof(char)*data.size());
                strcpy(buff,data.c_str());
                char* temp;

                temp = strtok (buff,";");
                while (temp != NULL)
                {
                    tempDatesList.push_back(atof(temp));
                    temp = strtok (NULL, ";");
                }

                cout << "Liste des dates des jobs obtenue : ";
                list<double>::iterator itdates;
                for(itdates = tempDatesList.begin();itdates != tempDatesList.end();++itdates){
                    cout<<*itdates<<", ";
                }
                cout<<"\n\n";

                free(buff);
				// free(temp);
			}
		}

		cout<<"Construction de la liste de jobs...\n";
        cout<<"Nb dates trouvees : "<<tempDatesList.size()<<"\n";
        cout<<"Nb jobs trouvees : "<<tempJobList.size()<<"\n";


		if(tempJobList.size() != n){ // simple vérification, pour savoir si le parsage s'est bien déroulé
            cout<<"Erreur de parsage (nb produits/liste des commandes incompatibles)\n";
            exit(-1);
		}

        int i;
        vector<Client*>::iterator itClients;
        list<int>::iterator itjob2 = tempJobList.begin();
        list<double>::iterator itDate2 = tempDatesList.begin();
        Client* tempCli;
        double tempDate;
        int tempJob;

        for(i = 0;i<n;++i){
            tempJob = *itjob2;
            tempDate = *itDate2;

            itClients = clients.begin();
            while(itClients != clients.end()){

                if((*itClients)->getNum() == tempJob){
                    break;
                }
                ++itClients;
            }

            produits.push_back(new Produit(i+1,tempDate,*itClients));

            ++itjob2;
            ++itDate2;
        }

		cout<<"Parsage fini.\n";
		stream.close();

		printProduits();

	} else {

		cout<<"Impossible d'ouvrir le fichier.\n";
		exit(-1);
	}
}

/* Supprime tous les espaces "parasites" dans la string str */

void Parser::removeSpaces(string &input){
	input.erase(std::remove(input.begin(),input.end(),' '),input.end());
}

void Parser::printProduits(){

    vector<Produit*>::iterator it;

    cout<<"\nListe des jobs :\n\n";

    for(it = produits.begin();it != produits.end(); ++it){
        cout<<"\tNum : "<<(*it)->getNum()<<"\n";
        cout<<"\tClient : "<<(*it)->getClient()->getNum()<<"\n";
        cout<< setprecision(12) << setiosflags(ios::fixed) << setiosflags(ios::showpoint) << "\tDate due : "<<(*it)->dateDue()<<"\n";
        cout<<"\n";
    }
}

float Parser::getEta() {
    return eta;
}

int Parser::getCapa() {
    return c;
}

vector<Client*> Parser::getClients() {
    return clients;
}

vector<Produit*> Parser::getProduits() {
    return produits;
}

