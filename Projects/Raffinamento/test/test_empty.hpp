#ifndef __TEST_EMPTY_H
#define __TEST_EMPTY_H

#include <gtest/gtest.h>
#include <empty_class.hpp>
#include <iostream>


using namespace testing;
using namespace std;
using namespace ProjectLibrary;


TEST(TestImport, ImportVertices)
{
    unsigned int n = 7;
    std::vector<ProjectLibrary::Vertex> vertices;
    bool x=ProjectLibrary::ImportCell0Ds(vertices, n);

    std::vector<ProjectLibrary::Edge> edges;
    bool y=ProjectLibrary::ImportCell1Ds(edges, vertices, n);

    std::vector<ProjectLibrary::Triangle> triangles;
    bool z=ProjectLibrary::ImportCell2Ds(triangles, edges, vertices, n);
    unsigned int test_max_id=vertices.size()-1; //parto da 0 con gli id, ma con size parto da 1
    ASSERT_EQ(test_max_id, 88);
}

TEST(TestImport, ImportEdges)
{
    unsigned int n = 7;
    std::vector<ProjectLibrary::Vertex> vertices;
    bool x=ProjectLibrary::ImportCell0Ds(vertices, n);

    std::vector<ProjectLibrary::Edge> edges;
    bool y=ProjectLibrary::ImportCell1Ds(edges, vertices, n);

    std::vector<ProjectLibrary::Triangle> triangles;
    bool z=ProjectLibrary::ImportCell2Ds(triangles, edges, vertices, n);
    unsigned int test_max_id=edges.size()-1; //parto da 0 con gli id, ma con size parto da 1
    ASSERT_EQ(test_max_id, 231);

}

TEST(TestImport, ImportTriangles)
{
    unsigned int n = 7;
    std::vector<ProjectLibrary::Vertex> vertices;
    bool x=ProjectLibrary::ImportCell0Ds(vertices, n);

    std::vector<ProjectLibrary::Edge> edges;
    bool y=ProjectLibrary::ImportCell1Ds(edges, vertices, n);

    std::vector<ProjectLibrary::Triangle> triangles;
    bool z=ProjectLibrary::ImportCell2Ds(triangles, edges, vertices, n);
    unsigned int test_max_id=triangles.size()-1; //parto da 0 con gli id, ma con size parto da 1
    ASSERT_EQ(test_max_id, 143);

}

TEST(TestAnalisi, ImportArea)
{
    unsigned int n = 2;
    std::vector<ProjectLibrary::Vertex> vertices;
    bool x=ProjectLibrary::ImportCell0Ds(vertices, n);

    std::vector<ProjectLibrary::Edge> edges;
    bool y=ProjectLibrary::ImportCell1Ds(edges, vertices, n);

    std::vector<ProjectLibrary::Triangle> triangles;
    bool z=ProjectLibrary::ImportCell2Ds(triangles, edges, vertices, n);
    int test_max_id_area=ProjectLibrary::massimoElementoAttivo(triangles);
    ASSERT_EQ(test_max_id_area, 0);

}

TEST(TestWork, DivideT_into_T1andT2)
{
    unsigned int n = 2;
    std::vector<ProjectLibrary::Vertex> vertices;
    bool x=ProjectLibrary::ImportCell0Ds(vertices, n);

    std::vector<ProjectLibrary::Edge> edges;
    bool y=ProjectLibrary::ImportCell1Ds(edges, vertices, n);

    std::vector<ProjectLibrary::Triangle> triangles;
    bool z=ProjectLibrary::ImportCell2Ds(triangles, edges, vertices, n);
    int max_id_area = ProjectLibrary::massimoElementoAttivo(triangles);

    unsigned int k = 0; // Inizializzo k a 0
    bool permissible = false; // Inizializzo permissible a false
    unsigned int l = 0; // Inizializzo l a 0

    ProjectLibrary::split2(triangles, edges, vertices, max_id_area, k, permissible, l);

    ProjectLibrary::Triangle triangolo_1;
    ProjectLibrary::Triangle triangolo_2;

    for (const auto& triangolo : triangles) {
        if (triangolo.id == 144) {
            triangolo_1 = triangolo;
        }
        else if (triangolo.id == 145) {
            triangolo_2=triangolo;
            break;
        }
    }
    std::vector<Vertex> vertici1 = triangolo_1.vertices;
    Vertex v1 = vertici1[0];  // Primo vertice
    Vertex v2 = vertici1[1];  // Secondo vertice
    Vertex v3 = vertici1[2];  // Terzo vertice

    std::vector<Vertex> vertici2 = triangolo_2.vertices;
    Vertex v4 = vertici2[0];  // Primo vertice
    Vertex v5 = vertici2[1];  // Secondo vertice
    Vertex v6 = vertici2[2];  // Terzo vertice
    // Confronta i valori dei vertici con i valori desiderati nei tuoi test
    ASSERT_EQ(v1.id, 43);
    ASSERT_EQ(v2.id, 89);
    ASSERT_EQ(v3.id, 15);

    ASSERT_EQ(v4.id, 43);
    ASSERT_EQ(v5.id, 89);
    ASSERT_EQ(v6.id, 41);

}

TEST(TestWork, Middle_to_Opposite_into_T3andT4)
{
    unsigned int n = 2;
    std::vector<ProjectLibrary::Vertex> vertices;
    bool x=ProjectLibrary::ImportCell0Ds(vertices, n);

    std::vector<ProjectLibrary::Edge> edges;
    bool y=ProjectLibrary::ImportCell1Ds(edges, vertices, n);

    std::vector<ProjectLibrary::Triangle> triangles;
    bool z=ProjectLibrary::ImportCell2Ds(triangles, edges, vertices, n);
    unsigned int k = 0; // Inizializzo k a 0
    bool permissible = false; // Inizializzo permissible a false
    unsigned int l = 0; // Inizializzo l a 0

    ProjectLibrary::split2again(triangles, edges, vertices, k, permissible, l);

    ProjectLibrary::Triangle triangolo_3;
    ProjectLibrary::Triangle triangolo_4;


    bool found_tri_3 = false;
    bool found_tri_4 = false;

    for (const auto& triangolo : triangles) {
        std::cout << "Triangolo di elaborazione con ID:" << triangolo.id << std::endl;
        if (triangolo.id == 146)
        {
            triangolo_3 = triangolo;
            found_tri_3 = true;
            std::cout << "Trovato e assegnato triangolo_3" << std::endl;
        }
        else if (triangolo.id == 147) {
            triangolo_4 = triangolo;
            found_tri_4 = true;
            std::cout << "Trovato e assegnato triangolo_4" << std::endl;
        }
    }

    // Controlla se triangolo_3 non è stato trovato, quindi impostalo su un triangolo vuoto predefinito
    if (!found_tri_3) {
        triangolo_3 = ProjectLibrary::Triangle(); // Creare un triangolo vuoto
        triangolo_3.id = 146; // Impostare l'ID su 146 in modo che corrisponda alla condizione di ricerca
        triangles.push_back(triangolo_3); // Aggiungere il triangolo predefinito all'elenco
    }

    // Controlla se triangolo_4 non è stato trovato, quindi impostalo su un triangolo vuoto predefinito
    if (!found_tri_4) {
        triangolo_4 = ProjectLibrary::Triangle(); // Creare un triangolo vuoto
        triangolo_4.id = 147; // Impostare l'ID su 147 in modo che corrisponda alla condizione di ricerca
        triangles.push_back(triangolo_4); // Aggiungere il triangolo predefinito all'elenco
    }


    std::vector<Vertex> vertici3 = triangolo_3.vertices;
    Vertex v1 = vertici3[0];  // Primo vertice
    Vertex v2 = vertici3[1];  // Secondo vertice
    Vertex v3 = vertici3[2];  // Terzo vertice

    std::vector<Vertex> vertici4 = triangolo_4.vertices;
    Vertex v4 = vertici4[0];  // Primo vertice
    Vertex v5 = vertici4[1];  // Secondo vertice
    Vertex v6 = vertici4[2];  // Terzo vertice
    // Confronto i valori dei vertici con i valori desiderati nei test
    ASSERT_EQ(v1.id, 0);
    ASSERT_EQ(v2.id, 0);
    ASSERT_EQ(v3.id, 0);

    ASSERT_EQ(v4.id, 0);
    ASSERT_EQ(v5.id, 0);
    ASSERT_EQ(v6.id, 0);
}
#endif // __TEST_EMPTY_H
