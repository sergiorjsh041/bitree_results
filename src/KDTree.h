#ifndef KDTREE_H
#define KDTREE_H

#include <vector>
#include <set>
#include <queue>
#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

// Estructura que representa una grilla
struct Grid {
    vector<pair<uint32_t, uint32_t>> ranges;  // Rangos en cada dimension [start, end]
    set<vector<uint32_t>> points;        // Conjunto de puntos en esta grilla
    uint8_t next;                       // Eje en el cual se realiza el proceso de construcción (para kdtree bi)
    uint8_t level;
    Grid(uint8_t d, uint32_t S, uint8_t level);
    Grid(); // Declaracion del constructor
};

// Estructura que representa la respuesta de la grilla
struct Response {
    bit_vector bv;                 // Bitvector con la respuesta de la grilla
    vector<Grid> subgrids;
    uint8_t level;// Subgrillas resultantes con puntos

    Response(uint8_t d, uint8_t level);               // Declaracion del constructor
};

class KDTree {
public:
    vector<vector<uint32_t>> points;  // Lista de puntos
    uint32_t S;                       // Tamaño de la grilla
    uint8_t d;                       // Dimensiones del espacio
    queue<Grid> grids;          // Arreglo de grillas
    vector<uint8_t> attr_set;
    vector<bit_vector> bitvector;
    vector<rank_support_v<1>> ranks;


    Response (KDTree::*get_response)(Grid&);  // Mtodo de creacion de las subgrillas
    
    KDTree(const vector<vector<uint32_t>>& pts, uint32_t S, uint8_t d, const vector<uint8_t> &attr_set); // Declaración del constructor
    
    bit_vector representation; // Vector de bits que representa al arbol
    
    void build_tree(); // Constructor del vector de bits

    Response get_resp(Grid& g);

};

vector<bit_vector> join(const vector<vector<bit_vector>>& bitrees,const vector<vector<rank_support_v<1>>>& ranks,const vector<vector<uint8_t>>& attr, const uint8_t& h, const uint8_t& d);
uint8_t intersect(vector<bit_vector>* bitvectors,const vector<vector<bit_vector>>& bitrees,const vector<vector<rank_support_v<1>>>& ranks, const vector<pair<uint8_t, uint32_t>>& posiciones, const vector<vector<uint8_t>>& attr_set,const uint8_t& att, const uint8_t& p, const uint8_t& h);

#endif

