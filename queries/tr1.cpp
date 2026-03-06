
#include <fstream>
#include<bits/stdc++.h>
#include<ratio>
#include<chrono>
#include<ctime>

using namespace std::chrono;


#include "../src/KDTree.cpp"

high_resolution_clock::time_point start_select, stop_select;
double total_time_select = 0.0;       
duration<double> time_span_select;

#define AT_A 0
#define AT_B 1
#define AT_C 2

uint32_t potenciaDe2MayorOIgual(uint32_t n) {
    // Caso especial: si n es 0, retornamos 1 (2^0)
    // Matemáticamente 0 no tiene potencia, pero en computación suele requerirse al menos 1.
    if (n == 0) return 1;

    // 1. Restamos 1.
    // Esto es lo que permite que si entra 8 (1000), se trate como 7 (0111)
    // para que el resultado final vuelva a ser 8.
    n--;

    // 2. Propagación de bits (para 64 bits)
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;

    // 3. Sumamos 1 para obtener la potencia
    return n + 1;
}

std::vector<std::vector<uint32_t>>* read_relation(const std::string filename, uint8_t n_Atts)
{
    std::ifstream input_stream(filename); 
    uint32_t x;
    uint8_t i, j=0;
    
    std::vector<std::vector<uint32_t>>* relation;
    std::vector<uint32_t> tuple;

    relation = new std::vector<std::vector<uint32_t>>();

    input_stream >> x;
    while (!input_stream.eof()) {
        tuple.clear();         
        for (i = 0; i < n_Atts; i++) {       
            tuple.push_back(x);
            input_stream >> x;
        }
        relation->push_back(tuple);
    }
    
    return relation;
}


uint32_t maximum_in_table(std::vector<std::vector<uint32_t>> &table, uint8_t n_columns, uint32_t max_temp)
{
    uint32_t i, j;
    
    for (i = 0; i < table.size(); i++) 
        for (j = 0; j < n_columns; j++)
            if (table[i][j] > max_temp)
                max_temp = table[i][j];
    
    
    return max_temp;
}

void invertirTuplas(std::vector<std::vector<uint32_t>>* rel_U) {
    // Verificamos que el puntero no sea nulo por seguridad
    if (!rel_U) return;

    // Iteramos por referencia sobre el vector desreferenciado
    for (auto& tupla : *rel_U) {
        // Buena práctica: asegurarnos de que hay al menos 2 elementos
        if (tupla.size() >= 2) {
            std::swap(tupla[0], tupla[1]);
        }
    }
}

int main(int argc, char** argv)
{
    vector<uint8_t> att_R;
    vector<uint8_t> att_S;
    vector<uint8_t> att_T;

    att_R.push_back(AT_A); att_R.push_back(AT_B); 
    att_S.push_back(AT_B); att_S.push_back(AT_C); 
    att_T.push_back(AT_A); att_T.push_back(AT_C);
    
    std::string strRel_R(argv[1]), strRel_S(argv[2]), strRel_T(argv[3]); 
    
    std::vector<std::vector<uint32_t>>* rel_R = read_relation(strRel_R, att_R.size());
    std::vector<std::vector<uint32_t>>* rel_S = read_relation(strRel_S, att_S.size());
    std::vector<std::vector<uint32_t>>* rel_T = read_relation(strRel_T, att_T.size());

    invertirTuplas(rel_T);
    
    uint32_t grid_side = 0;
    
    grid_side = maximum_in_table(*rel_R, att_R.size(), grid_side);
    grid_side = maximum_in_table(*rel_S, att_S.size(), grid_side);
    grid_side = maximum_in_table(*rel_T, att_T.size(), grid_side);
    
    grid_side++;
  
    grid_side= potenciaDe2MayorOIgual(grid_side);
    //cout << "Grid side: " << grid_side << endl;

    KDTree qdag_rel_R(*rel_R, grid_side, 2, att_R);
    KDTree qdag_rel_S(*rel_S, grid_side, 2, att_S);
    KDTree qdag_rel_T(*rel_T, grid_side, 2, att_T);

    qdag_rel_R.build_tree();
    qdag_rel_S.build_tree();
    qdag_rel_T.build_tree();

    vector<vector<bit_vector>> Q(3);

    Q[0] = qdag_rel_R.bitvector;
    Q[1] = qdag_rel_S.bitvector;
    Q[2] = qdag_rel_T.bitvector;

    vector<vector<rank_support_v<1>>> ranks(3);

    ranks[0] = qdag_rel_R.ranks;
    ranks[1] = qdag_rel_S.ranks;
    ranks[2] = qdag_rel_T.ranks;

    vector<bit_vector> Join_Result;
  
    high_resolution_clock::time_point start, stop;
    double total_time = 0.0;       
    duration<double> time_span;

    uint8_t h = (uint8_t) std::round(log2(grid_side))*3;

    Join_Result=join(Q,ranks, {att_R,att_S,att_T},h, 3);
    
    start = high_resolution_clock::now();    
    
    Join_Result=join(Q,ranks, {att_R,att_S,att_T},h, 3);

    stop = high_resolution_clock::now();
    time_span = duration_cast<microseconds>(stop - start);
    total_time = time_span.count();    

    cout << /*"Multiway Join ended in " <<*/ total_time /*<< " seconds"*/ << endl;
    cout << /*"Multiway Join ended in " <<*/ total_time /*<< " seconds"*/ << endl;

    uint64_t bits = 0;
    for (auto& vector : Join_Result) {
        bits += vector.size();
    }

    cout << bits << endl;

    return 0;
}
