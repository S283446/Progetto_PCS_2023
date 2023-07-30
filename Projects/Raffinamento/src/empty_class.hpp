#ifndef __EMPTY_H
#define __EMPTY_H
#include "Eigen/Eigen"
#include <iostream>
#include <fstream>
#include <vector>


using namespace std;
using namespace Eigen;
namespace ProjectLibrary
{
// il vertice è formato da due punti nel piano: x e y
class Vertex
{
public:
    double x;
    double y;
    unsigned int id;

    Vertex():
        x(0), y(0), id(0)
    {}

    inline bool operator>(const Vertex& other) const
    {return id > other.id;}
    inline bool operator<(const Vertex& other) const
    {return id < other.id;}
    inline bool operator==(const Vertex& other) const
    {return id == other.id;}
    inline bool operator!=(const Vertex& other) const
    {return !(id == other.id);}

};

class Edge
{
public:
    bool active = true;
    Vertex start;
    Vertex finish ;
    vector<unsigned int> adjTriangles;
    bool border=false;
    unsigned int id;
    double length;

    Edge(Vertex start, Vertex finish, unsigned int id):
        start(start), finish(finish), id(id)
    {
        length = sqrt(pow(finish.y-start.y, 2) + pow(finish.x-start.x, 2));
    }

    Edge():
        id(0)
    {}

    void set_length()
    {
        length = sqrt(pow(finish.y-start.y, 2) + pow(finish.x-start.x, 2));
    }

    inline bool operator>(const Edge& other)
    {return length > other.length;}

    inline bool operator<(const Edge& other)
    {return length  < other.length;}

    inline bool operator==(const Edge& other)
    {return id == other.id;}

    inline bool operator!= (const Edge& other)
    {return !(id == other.id);}

};

// il triangolo è formato da area, id e lati
class Triangle
{
public:
    bool active = true;
    double area;
    unsigned int id;
    vector<Edge> edges;
    vector<Vertex> vertices;
    Triangle (vector<Edge> edge, unsigned int id):
        id(id), edges(edge)
    {
        vertices.push_back(edges[0].start);
        vertices.push_back(edges[0].finish);
        if (edges[1].start != edges[0].start && edges[1].start != edges[0].finish)
            vertices.push_back(edges[1].start);
        else
            vertices.push_back(edges[1].finish);
        area = abs(vertices[0].x*(vertices[1].y-vertices[2].y) +
                   vertices[1].x*(vertices[2].y-vertices[0].y)  +
                   vertices[2].x*(vertices[0].y-vertices[1].y)) / 2;
    }

    Triangle():
        id(0)
    {
        edges = vector<Edge>(3, Edge(Vertex(), Vertex(), 0)); // Create a vector of 3 edges, each initialized with default constructor of Edge
        vertices = vector<Vertex>(3, Vertex()); // Create a vector of 3 vertices, each initialized with default constructor of Vertex
    }

    // quando confronto due triangoli, uno è maggiore dell'altro in base alle loro aree
    inline bool operator>(const Triangle& other)
    {return area  > other.area;}

    inline bool operator<(const Triangle& other)
    {return area < other.area;}

    inline bool operator==(const Triangle& other)
    {return id == other.id;}

    inline bool operator!=(const Triangle& other)
    {return !(id == other.id);}

    void set_area()
    {
        area = abs(vertices[0].x*(vertices[1].y-vertices[2].y) +
                   vertices[1].x*(vertices[2].y-vertices[0].y)  +
                   vertices[2].x*(vertices[0].y-vertices[1].y)) / 2;
    }
};

void Refine(vector<Triangle>& triangles, vector<Edge>& edges, vector<Vertex>& vertices, unsigned int &n);
//importa i vertici della mesh triangolare
bool ImportCell0Ds(vector<Vertex>& vertices, unsigned int n);
//importa i lati della mesh triangolare
bool ImportCell1Ds(vector<Edge>& edges, vector<Vertex>& vertices, unsigned int n);
//importa i triangoli della mesh
bool ImportCell2Ds(vector<Triangle>& triangles, vector<Edge>& edges, vector<Vertex>& vertices, unsigned int n);
//divido triangolo in 2
void split2(vector<Triangle> &triangles, vector<Edge> &edges, vector<Vertex> &vertices, unsigned int m,
            unsigned int &k, bool &permissible, unsigned int &l);
void split2again (vector<Triangle> &triangles, vector<Edge> &edges,vector<Vertex> &vertices,
                 unsigned int &k, bool &permissible, unsigned int &l);

template <typename T>
void Pushback(std::vector<T>& vec, const T& element)
{
    vec.push_back(element);
}


inline void insertionSort(vector<Edge> &edge)
{
    unsigned int n= edge.size();
    for (unsigned int i = 0; i < n; i++)
    {
        Edge key = edge[i];
        int j = i - 1;
        while (j >= 0 && edge[j] < key)
        {
            edge[j + 1] = edge[j];
            j = j - 1;
        }
        edge[j + 1] = key;
    }
}


inline unsigned int massimoElementoAttivo(vector<Triangle> &vettore)
{
    Triangle massimo;
    massimo.area = -1;

    for (unsigned int i=0; i<vettore.size(); i++)
    {
        if (vettore[i].active)
        {
            if (vettore[i] > massimo)
                massimo = vettore[i];
        }
    }
    unsigned int m = massimo.id;
    return m;
}

inline Vertex getOppositeVertex(Triangle &triangle, Edge &edge)
{
    for (unsigned int i = 0; i < 3; i++)
    {
        if (triangle.edges[i].start != (edge.start) &&
            triangle.edges[i].start != (edge.finish))
            return triangle.edges[i].start;
        else if (triangle.edges[i].finish != (edge.start) &&
                 triangle.edges[i].finish != (edge.finish))
            return triangle.edges[i].finish;
    }
    throw(runtime_error("Something went wrong getting the opposite vertex"));
}

// Prendo il punto medio di un lato
inline Vertex set_mid(Edge edge)
{
    Vertex mid;
    mid.x = (edge.finish.x + edge.start.x) / 2;
    mid.y = (edge.finish.y + edge.start.y) / 2;
    return mid; // Assegnare id e aggiornare vettore di vertici
}
}

#endif// __EMPTY_H
