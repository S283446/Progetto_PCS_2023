#include "empty_class.hpp"
#include <chrono>
using namespace std;
using namespace ProjectLibrary;
int main()
{
    auto start = chrono::high_resolution_clock::now();
    vector<Vertex> ciro;
    vector<Edge> marco;
    vector<Triangle> cosimo;
    unsigned int n= 1000;
    ImportCell0Ds(ciro, n);
    ImportCell1Ds(marco, ciro, n);
    ImportCell2Ds(cosimo, marco, ciro, n);
    Refine(cosimo, marco, ciro, n);
    ofstream outputFile("C:/Users/Davide/Dropbox (Politecnico Di Torino Studenti)/PC/Desktop/Progetto_PCS/Projects/esporto_dati\file_lati.cvs.xlsx");
    for (unsigned int i=0; i<ciro.size(); i++)
    {
        outputFile<<ciro[i].x<<";"<<ciro[i].y<<"\n";
    }
    outputFile.close();

    ofstream outputFile1("C:/Users/Davide/Dropbox (Politecnico Di Torino Studenti)/PC/Desktop/Progetto_PCS/Projects/esporto_dati\file_vertici.cvs.xlsx");
    for (unsigned int i=0; i<marco.size(); i++)
    {
        if (marco[i].active)
            outputFile1<<marco[i].start.x<<";"<<marco[i].start.y<<";"<<marco[i].finish.x<<";"<<marco[i].finish.y<<"\n";
    }

    auto end = std::chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Tempo di esecuzione: " << duration.count() << "s" << endl;
    cout << "Numero di iterazioni: " << n << endl;
    return 0;
}
