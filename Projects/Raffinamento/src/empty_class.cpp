#include "empty_class.hpp"
#include <queue>

namespace ProjectLibrary
{
void split2(vector<Triangle> &triangles, vector<Edge> &edges, vector<Vertex> &vertices, unsigned int m,
            unsigned int &k, bool &permissible, unsigned int &l)
{
    insertionSort(triangles[m].edges); // Sistemo i lati in ordine decrescente
    Vertex opposite =getOppositeVertex(triangles[m], triangles[m].edges[0]); // Lato opposto
    Vertex mid = set_mid(triangles[m].edges[0]);
    mid.id = vertices.size(); // Assegno nuovo id, +1 rispetto all'ultimo
    Pushback(vertices, mid);
    Edge bisection(opposite, mid,edges.size()); // Trovo il lato bisezione, gli id sono progressivi
    Edge newEdge1(triangles[m].edges[0].start, mid,edges.size() + 1); // Trovo primo lato nuovo, id progressivo
    Edge newEdge2(mid, triangles[m].edges[0].finish, edges.size() + 2); // Trovo secondo lato nuovo
    vector<Edge> newEdgesV1;
    vector<Edge> newEdgesV2;
    // nel modo seguente, creo i nuovi triangoli in modo corretto, assegnando il
    // lato giusto ai nuovi triangoli creati, il flag risparmia tempo, per evitare
    // di ricontrollare
    if (triangles[m].edges[1].start == triangles[m].edges[0].start || triangles[m].edges[1].finish ==triangles[m].edges[0].start) // Lato sinistro con edge1 o con edge2
    {
        newEdgesV1 = {bisection, newEdge1,triangles[m].edges[1]}; // Creo nuovi triangoli con i lati giusti
        newEdgesV2 = {bisection, newEdge2, triangles[m].edges[2]};
    }
    else
    {
        newEdgesV1 = {bisection, newEdge1,triangles[m].edges[2]}; // Creo nuovi triangoli con i lati giusti
        newEdgesV2 = {bisection, newEdge2, triangles[m].edges[1]};
    }
    Triangle newTriangle1(newEdgesV1, triangles.size());
    Triangle newTriangle2(newEdgesV2, triangles.size() + 1);
    // condizione di stop : se un triangolo ha il lato più lungo al bordo, raffino
    // una volta e mi fermo. La mesh è ammissibile
    triangles[m].active =false; // Disattivo il triangolo dal vettore di triangolo
    if (triangles[m].edges[0].border)
    {
        permissible = true;
        newTriangle1.edges[1].border=true;
        newTriangle2.edges[1].border=true;
    }
    else
    {
        for (Triangle &triangle : triangles)
        {
            if (triangle.active)
            {
                for (unsigned int i=0; i<3; i++)
                {
                    if (triangle.edges[i].start == triangles[m].edges[0].start && triangle.edges[i].finish == triangles[m].edges[0].finish)
                    {
                        k=triangle.id;
                        l=i;
                        break;
                    }
                    else if (triangle.edges[i].start == triangles[m].edges[0].finish && triangle.edges[i].finish == triangles[m].edges[0].start)
                    {
                        k=triangle.id;
                        l=i;
                        break;
                    }
                }
            }
        }
    }
    Pushback(edges, bisection);
    Pushback(edges, newEdge1);
    Pushback(edges, newEdge2);
    // inserisco i nuovi triangoli nel vettore
    Pushback(triangles, newTriangle1);
    Pushback(triangles, newTriangle2);
}

void split2again(vector<Triangle> &triangles, vector<Edge> &edges, vector<Vertex> &vertices, unsigned int &k, bool &permissible, unsigned int &l)
{
    Vertex newOpposite = getOppositeVertex(triangles[k], triangles[k].edges[l]);
    Edge newEdge(newOpposite, vertices[vertices.size() - 1],edges.size()); // Creo nuovo lato
    Edge EdgeV1(triangles[k].edges[l].start, vertices[vertices.size() - 1],edges.size() + 1); // Trovo primo lato nuovo, id progressivo
    Edge EdgeV2(vertices[vertices.size() - 1], triangles[k].edges[l].finish,edges.size() + 2); // Trovo secondo lato nuovo
    vector<Edge> EdgesV1;
    vector<Edge> EdgesV2;
    if (l==0)
    {
        if (triangles[k].edges[1].start == triangles[k].edges[l].start ||triangles[k].edges[1].finish ==triangles[k].edges[l].start)
        {
            EdgesV1 = {newEdge, EdgeV1,triangles[k].edges[1]}; // Creo nuovi triangoli con i lati giusti
            EdgesV2 = {newEdge, EdgeV2,triangles[k].edges[2]};
        }
        else
        {
            EdgesV1 = {newEdge, EdgeV1,triangles[k].edges[2]}; // Creo nuovi triangoli con i lati giusti
            EdgesV2 = {newEdge, EdgeV2,triangles[k].edges[1]};
        }
    }
    else if (l==1)
    {
        if (triangles[k].edges[2].start == triangles[k].edges[l].start ||triangles[k].edges[2].finish ==triangles[k].edges[l].start)
        {
            EdgesV1 = {newEdge, EdgeV1,triangles[k].edges[2]}; // Creo nuovi triangoli con i lati giusti
            EdgesV2 = {newEdge, EdgeV2,triangles[k].edges[0]};
        }
        else
        {
            EdgesV1 = {newEdge, EdgeV1,triangles[k].edges[0]}; // Creo nuovi triangoli con i lati giusti
            EdgesV2 = {newEdge, EdgeV2,triangles[k].edges[2]};
        }
    }
    else
    {
        if (triangles[k].edges[1].start == triangles[k].edges[l].start ||triangles[k].edges[1].finish ==triangles[k].edges[l].start)
        {
            EdgesV1 = {newEdge, EdgeV1,triangles[k].edges[1]}; // Creo nuovi triangoli con i lati giusti
            EdgesV2 = {newEdge, EdgeV2,triangles[k].edges[0]};
        }
        else
        {
            EdgesV1 = {newEdge, EdgeV1,triangles[k].edges[0]}; // Creo nuovi triangoli con i lati giusti
            EdgesV2 = {newEdge, EdgeV2,triangles[k].edges[1]};
        }
    }
    Triangle newTriangle(EdgesV1, triangles.size());
    Triangle newTriangle0(EdgesV2, triangles.size() + 1);
    Pushback(edges, newEdge);
    Pushback(edges, EdgeV1);
    Pushback(edges, EdgeV2);
    triangles[k].active = false; // Cancello il vecchio triangolo grande
    Pushback(triangles, newTriangle);
    Pushback(triangles, newTriangle0);
    permissible = true;
}

// raffina tutto l'array di triangoli, partendo da un vettore ordinato in base
// all'area
void Refine(vector<Triangle> &triangles, vector<Edge> &edges, vector<Vertex> &vertices, unsigned int &n)
{
    for (unsigned int i=0; i<n ; i++)
    {
        bool permissible = false;
        unsigned int m=massimoElementoAttivo(triangles);
        unsigned int k = 0,l=0;          // K-esimo triangolo da raffinare
        split2(triangles, edges, vertices, m, k, permissible, l);
        while (!permissible)
        {
            split2again(triangles, edges, vertices, k, permissible, l);
        }
    }
}
// assegno a ogni triangolo le sue proprietà chiamando le seguenti tre funzioni
bool ImportCell0Ds(vector<Vertex> &vertices, unsigned int n)
{
    ifstream file;
    string inFile = "C:/Users/Davide/Dropbox (Politecnico Di Torino Studenti)/PC/Desktop/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell0Ds.csv";
    file.open(inFile);

    if (file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);
    file.close();
    listLines.pop_front();
    vertices.reserve(listLines.size() * n);
    vertices.resize(listLines.size());
    if (vertices.size() == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }
    for (const string &line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2d coord;
        converter >> id >> marker >> coord(0) >> coord(1);
        vertices[id].id = id;
        vertices[id].x = coord(0);
        vertices[id].y = coord(1);
    }
    file.close();
    return true;
}
bool ImportCell1Ds(vector<Edge> &edges, vector<Vertex> &vertices, unsigned int n)
{
    ifstream file;
    string inFile = "C:/Users/Davide/Dropbox (Politecnico Di Torino Studenti)/PC/Desktop/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell1Ds.csv";
    file.open(inFile);

    if (file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);
    listLines.pop_front();
    edges.reserve(2 * listLines.size() * n);
    edges.resize(listLines.size());
    if (edges.size() == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }
    for (const string &line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        unsigned int start;
        unsigned int finish;

        converter >> id >> marker >> start >> finish;

        edges[id].id = id;
        edges[id].start = vertices[start];
        edges[id].finish = vertices[finish];
        edges[id].set_length();
    }

    file.close();
    return true;
}
bool ImportCell2Ds(vector<Triangle> &triangles, vector<Edge> &edges,
                   vector<Vertex> &vertices, unsigned int n)
{
    ifstream file;
    string inFile = "C:/Users/Davide/Dropbox (Politecnico Di Torino Studenti)/PC/Desktop/Progetto_PCS/Projects/Raffinamento/Dataset/Test1/Cell2Ds.csv";
    file.open(inFile);

    if (file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);
    listLines.pop_front();
    triangles.reserve(3 * listLines.size() * n);
    triangles.resize(listLines.size());
    if (triangles.size() == 0)
    {
        cerr<< "There is no cell 2D" << endl;
        return false;
    }
    vector<int> tempEdges;
    tempEdges.resize(3 * listLines.size());
    unsigned int z = 0;
    for (const string &line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        array<unsigned int, 3> tempVertices;

        converter >> id;
        for (unsigned int i = 0; i < 3; i++)
        {
            converter >> tempVertices[i];
        }
        for (unsigned int i = z; i < (z + 3); i++)
        {
            converter >> tempEdges[i];
        }

        triangles[id].id = id;
        Pushback(triangles[id].vertices, vertices[tempVertices[0]]);
        Pushback(triangles[id].vertices, vertices[tempVertices[1]]);
        Pushback(triangles[id].vertices, vertices[tempVertices[2]]);

        for (unsigned int j = z; j < (z + 3); j++)
        {
            edges[tempEdges[j]].adjTriangles.reserve(3);
            Pushback(edges[tempEdges[j]].adjTriangles, triangles[id].id);
        }
        z = z + 3;
    }
    z = 0;
    for (unsigned int i = 0; i < listLines.size(); i++)
    {
        for (unsigned int j = z; j < (z + 3); j++)
        {
            triangles[i].edges.erase(triangles[i].edges.begin()+j); // Se no levo l'altro
            Pushback(triangles[i].edges, edges[tempEdges[j]]);
            if (triangles[i].edges[j-z].adjTriangles.size()>0)
            {
                if (triangles[i].edges[j - z].adjTriangles[0] == triangles[i].id)
                    triangles[i].edges[j - z].adjTriangles.erase(triangles[i].edges[j - z].adjTriangles.begin()); // Se no levo l'altro// Levo il primo triangolo a cui punto
                else if (triangles[i].edges[j - z].adjTriangles[1] == triangles[i].id)
                    triangles[i].edges[j - z].adjTriangles.erase(triangles[i].edges[j - z].adjTriangles.begin()+1); // Se no levo l'altro
                if (triangles[i].edges[j-z].adjTriangles.size()==0)
                    triangles[i].edges[j-z].border=true;
            }
        }
        triangles[i].set_area();// assegno l'area al triangolo
        z = z + 3;
    }
    file.close();
    return true;
}
}


