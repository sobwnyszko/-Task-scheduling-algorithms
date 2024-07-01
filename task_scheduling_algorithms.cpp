#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <cstdlib>

using namespace std;

struct Cart {
    int minute;
    int number;
    string type;
    int ilosc;
    int weight;
    int time;
    int status;  //1=zlamany, 0=normalny
    int time_b;
};

int r, rob=1, kwant=1;
vector <Cart> kolejka_blad;

void oddaj (vector <Cart> &kolejka, Cart robots[], int i)
{
    if (!kolejka.empty() && robots[i].time == 0 && kolejka.front().status == 0){
        robots[i].type = kolejka.front().type;
        robots[i].time = kolejka.front().time;
        kolejka.erase(kolejka.begin());
        r++;
    }
    else if (!kolejka.empty() && kolejka.front().status == 1){
        kolejka.front().status = 0;
        kolejka_blad.push_back(kolejka.front());
        kolejka.erase(kolejka.begin());
    }
}

void rr (vector <Cart> &kolejka, Cart robots[], int czasPracy[])
{
    for (int i=0; i<rob;++i){
        oddaj (kolejka, robots, i);
        if (robots[i].time > 0){
            cout << '[' << setw(12) << left << robots[i].type << setw(2)  << right << robots[i].time << ']';
            robots[i].time--;
            czasPracy[i]++;
            if (robots[i].time == 0){
                 r--;
                 czasPracy[i] = 0;
            }
        } else if (robots[i].time == 0){
            cout << "[              ]";
        }
    }
    for (int i=0; i<rob;++i){
        if (czasPracy[i] == kwant){
            czasPracy[i] = 0;
            kolejka.push_back(robots[i]);
            robots[i].time = 0;
            r--;
        }
    }
}

void fcfs (vector <Cart> &kolejka, Cart robots[])
{
    for (int i=0; i<rob;++i){
        oddaj (kolejka, robots, i);
        if (robots[i].time > 0){
            cout << '[' << setw(12) << left << robots[i].type << setw(2)  << right << robots[i].time << ']';
            robots[i].time--;
            if(robots[i].time == 0){r--;}
        }else if (robots[i].time == 0 ){
            cout << "[              ]";
        }
    }
}

void sjf (vector <Cart> &kolejka, Cart robots[])
{
    for (int i=0; i<kolejka.size(); ++i){
        for (int j=0; j<kolejka.size()-i-1; ++j){
            if (kolejka[j].time > kolejka[j+1].time){
                swap(kolejka[j], kolejka[j+1]);
            }
        }
    }
    for (int i=0; i<rob;++i){
        oddaj (kolejka, robots, i);
        if (robots[i].time > 0){
            cout << '[' << setw(12) << left << robots[i].type << setw(2)  << right << robots[i].time << ']';
            robots[i].time--;
            if(robots[i].time == 0){r--;}
        }else if (robots[i].time == 0){
            cout << "[              ]";
        }
    }
}

void srtf (vector <Cart> &kolejka, Cart robots[])
{
    for (int i=0; i<kolejka.size(); ++i){
        for (int j=0; j<kolejka.size()-i-1; ++j){
            if (kolejka[j].time > kolejka[j+1].time){
                swap(kolejka[j], kolejka[j+1]);
            }
        }
    }
    for (int i=0; i<rob;++i){
        oddaj (kolejka, robots, i);
        if (robots[i].time > 0){
            if (robots[i].time > kolejka.front().time){
                kolejka.push_back(robots[i]);
                robots[i].type = kolejka.front().type;
                robots[i].time = kolejka.front().time;
                kolejka.erase(kolejka.begin());
            }
            cout << '[' << setw(12) << left << robots[i].type << setw(2)  << right << robots[i].time << ']';
            robots[i].time--;
            if(robots[i].time == 0){r--;}
        }else if (robots[i].time == 0){
            cout << "[              ]";
        }
    }
}
int main(int argc, char*argv[]) {
    //ilość robotów, kwant czasu pracy, nazwa pliku, algorytm szeregowania
    if (argc != 5){
        cerr << "Uzycie: " << argv[0]
        << "ilość robotów(>0), kwant czasu pracy(>0), nazwa pliku, algorytm szeregowania(1=RR, 2=FCFS, 3=SJF, 4=SRTF)" << endl;
        return 1;
    }

    srand(time(NULL));

    int algorytm = 1, czyt = 1;
    string file = "dane.txt";
    vector <Cart> kolejka;
    Cart cart;
    int n=1, lista=0, lista_chwil[3];
    lista_chwil[0]=1;lista_chwil[1]=2;lista_chwil[2]=4;
    int kol_or_rob = 0, wybor = 0;
    string line, minut, numb, typ, ile, weigth;

    rob = stoi(argv[1]);
    if (rob < 1){
        cerr << "Ilosc robotow nie moze byc mniej niz 1" << endl;
        return 1;
    }
    kwant = stoi(argv[2]);
    if (kwant < 1){
        cerr << "Kwant czasu nie moze byc mniej niz 1" << endl;
        return 1;
    }

    if (argv[3][0] == '-'){
        czyt = 1;
    } else {
        czyt = 2;
        file = argv[3];
    }
    algorytm = stoi(argv[4]);

    Cart robots[rob];
    int czasPracy[rob];

    cout << "Ilosc robotow: " << rob << endl
         << "Kwant czasu: " << kwant << endl
         << "Plik(2) lub cin(1): " << czyt << endl
         << "Algorytm szeregowania(1=RR, 2=FCFS, 3=SJF, 4=SRTF): " << algorytm << endl << endl;

    ifstream dane(file);

    if (!dane.is_open() && czyt == 2){
        cerr << "Nie mozna otworzyc pliku" << endl;
        return 1;
    }

    for (int i=0; i<rob; i++){
        robots[i].time = 0;
        czasPracy[i] = 0;
    }

    while(1) {
        if (czyt == 2){
            getline(dane, line);
        } else{
            getline(cin, line);
        }
        stringstream ss(line);
        getline(ss, minut, ' ');

        cout << "Moment " << n << endl; ++n;

        while (getline(ss, numb, ' ')){
            if(!getline(ss, typ, ' ')){break;}
            if(!getline(ss, ile, ' ')){break;}
            if(!getline(ss, weigth, ' ')){break;}

            cart.minute = stoi(minut);
            cart.number = stoi(numb);
            cart.type = typ;
            cart.ilosc = stoi(ile);
            cart.weight = stoi(weigth);
            cart.time = cart.ilosc * cart.weight;
            cart.status = 0;
            cart.time_b = 0;

            kolejka.push_back(cart);

            cout << "        wheelbarrow arrived <" << kolejka.back().number << ' ' << kolejka.back().type << ' ' << kolejka.back().ilosc
                 << ' ' << kolejka.back().weight << " [" << kolejka.back().time << "]>" << endl;
        }
        if (lista_chwil[lista] == n-1 && lista < 3){
            ++lista;
            //kol_or_rob = rand()%2;
            if (kol_or_rob == 0){
                wybor = rand()%kolejka.size();
                kolejka[wybor].status = 1;
                kolejka[wybor].time_b = rand()%2+1;
cout << wybor << ' ' << kolejka[wybor].type << " " << kolejka[wybor].time_b << endl;
            }
            else {
                wybor = rand()%rob;
                robots[wybor].status = 1;
            }
        }

        cout << "             ";

        switch (algorytm){
            case 2:{
                fcfs(kolejka, robots);
                break;
            }

            case 1:{
                rr(kolejka, robots, czasPracy);
                break;
            }

            case 3:{
                sjf(kolejka, robots);
                break;
            }

            case 4:{
                srtf(kolejka, robots);
                break;
            }

            default:{
                cerr << "Nie poprawny metod" << endl;
                return 1;
            }
        }

        for (int i=0; i<kolejka_blad.size();++i){
            if (kolejka_blad[i].time_b > 0){
                kolejka_blad[i].time_b--;
            }
            if (kolejka_blad[i].time_b == 0){
                kolejka_blad[i].status = 0;
                kolejka.push_back(kolejka_blad[i]);
                kolejka_blad.erase(kolejka_blad.begin()+i);
            }
        }

        cout << endl;
        if(kolejka.empty() && r == 0 && kolejka_blad.empty()){
            return 0;
        }
    }
    return 0;
}
