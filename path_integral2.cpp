#include<iostream>
#include<array>
#include<vector>
#include<numeric>
#include<cmath>
#include<algorithm>
#include<random>
#include <fstream>
#include <string>
using namespace std;

double V(double x, double m = 1)
{
    double potencial = 5000;
    if (x > -1 and x < 1)
    {
        potencial = 0;
    }

    return potencial;
    
}



double E(std::vector<double> x, double m=1, double epsilon=1)
{
    int N = x.size();
    double sums = 0;
    for(int i=1; i < N; i++)
    {
        sums+= (m/2)*((x[i]-x[i-1])/epsilon)*((x[i]-x[i-1])/epsilon); //+ V((x[i]+x[i-1])/2, m);
    }
    //ezzel az összefüggéssel kapjuk meg egy adott pályának az energiáját. 
    //lsd ...képlet
    return sums;
}
double generate_random_number(double x1, double x2)
{
    std::random_device rd;  
    std::mt19937 gen(rd());                        //random számot generál egy [x1, x2] intervallumban
    std::uniform_real_distribution<> dis(x1, x2);
    return dis(gen);
}
int main() {
    int steps = 3000000;
    int N = 1000;
    double m = 1; 
    double epsilon = 1;                 //inicializálás
    std::vector<double> path(N, 0.0);
    std::vector<double> prob(N, 0.0);
    double w = generate_random_number(0,1);
    double oldE = E(path,m, epsilon );   //az energia inicializálása
    int counter = 1;   //ciklushoz számlálás

    while (counter < steps)
    {
       
        double r = generate_random_number(0,1);
        int element = (int)N*r;   //egyenletes eloszlásból véletlenszerűen választjuk ki, hogy a pálya
                                  //melyik elemét variáljuk meg
        double w = generate_random_number(0,1);
        double change = generate_random_number(-1,1);
        //mindegyik iterálásnál maximum +-1-et változhat a pozíció értéke
        path[element] += change;  //itt variáljuk meg az út egy véletlenszerűen választott elemét
        double newE = E(path,m, epsilon);
        //a megvariált útra kiszámítjuk az energiát
        
        if (newE > oldE and exp(oldE*epsilon-newE*epsilon) <= w)
        //ha az új energia nagyobb mint ez előző, illetve a megadott exponenciális kifejezés
        //(valószínűség) kisebb egy random számnál 0 és 1 között, akkor nem fogadjuk el a 
        //változtatást
            {
                path[element] -= change;
            }
        int elem = int(path[element]*160 + 500);
        //ez egy lineáris transzformáció, hogy a pálya elemeit tudjuk majd szétkenni egy 0-tól N-ig
        //tartó intervallumon egész számos egységekben, tehát az x = 0 legyen középen, illetve egész
        //számok legyenek 
        if (elem < 0){elem = 0;}                           //mivel index, ezért le kell ellenőrizni, hogy 
        if (elem > N){elem = N;}                          //nem csordul-e túl
        prob[elem] += 1;
        oldE = newE;
         //frissítjük az energiaértéket
        counter = counter + 1;
    }

    fstream file;    // kiiratjuk a szükséges vektort egy fájlba
    file.open("szabad.txt",ios_base::out);
 
    for(int i = 0; i < prob.size(); i++)
    {
        file<<prob[i]<<endl;
    }
    file.close();
    return 0;
}