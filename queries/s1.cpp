
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

#define AT_X1 0
#define AT_X2 1
#define AT_X3 2
#define AT_X4 3

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

std::vector<std::vector<uint32_t>>* read_relation_inverted(const std::string filename, uint8_t n_Atts)
{
    std::ifstream input_stream(filename);
    if (!input_stream) return nullptr; // Seguridad: archivo no encontrado

    uint32_t x;
    auto* relation = new std::vector<std::vector<uint32_t>>();

    while (input_stream >> x) {
        // Pre-reservamos el tamaño para evitar reasignaciones
        std::vector<uint32_t> tuple(n_Atts);

        // Insertamos desde la última posición (n_Atts - 1) hasta la 0
        for (int i = n_Atts - 1; i >= 0; i--) {
            tuple[i] = x;
            if (i > 0) { // Solo leemos el siguiente si no es el último del ciclo for
                input_stream >> x;
            }
        }
        relation->push_back(std::move(tuple)); // use move para evitar copiar el vector interno
    }

    return relation;
}

int main(int argc, char** argv)
{
    vector<uint8_t> att_R;
    vector<uint8_t> att_S;
    vector<uint8_t> att_T;
    vector<uint8_t> att_U;
    
    att_R.push_back(AT_X1); att_R.push_back(AT_X2); 
    att_S.push_back(AT_X2); att_S.push_back(AT_X3); 
    att_T.push_back(AT_X3); att_T.push_back(AT_X4); 
    att_U.push_back(AT_X1); att_U.push_back(AT_X4);
    
    std::string strRel_R(argv[1]), strRel_S(argv[2]), strRel_T(argv[3]), strRel_U(argv[4]); 
    
    std::vector<std::vector<uint32_t>>* rel_R = read_relation(strRel_R, att_R.size());
    std::vector<std::vector<uint32_t>>* rel_S = read_relation(strRel_S, att_S.size());
    std::vector<std::vector<uint32_t>>* rel_T = read_relation(strRel_T, att_T.size());
    std::vector<std::vector<uint32_t>>* rel_U = read_relation_inverted(strRel_U, att_U.size());


    uint32_t grid_side = 0;
    
    grid_side = maximum_in_table(*rel_R, att_R.size(), grid_side);
    grid_side = maximum_in_table(*rel_S, att_S.size(), grid_side);
    grid_side = maximum_in_table(*rel_T, att_T.size(), grid_side);
    grid_side = maximum_in_table(*rel_U, att_U.size(), grid_side);
    
    grid_side++;
    grid_side= potenciaDe2MayorOIgual(grid_side);


    KDTree qdag_rel_R(*rel_R, grid_side, 2, att_R);
    KDTree qdag_rel_S(*rel_S, grid_side, 2, att_S);
    KDTree qdag_rel_T(*rel_T, grid_side, 2, att_T);
    KDTree qdag_rel_U(*rel_U, grid_side, 2, att_U);

    qdag_rel_R.build_tree();
    qdag_rel_S.build_tree();
    qdag_rel_T.build_tree();
    qdag_rel_U.build_tree();

    vector<vector<bit_vector>> Q(4);

    Q[0] = qdag_rel_R.bitvector;
    Q[1] = qdag_rel_S.bitvector;
    Q[2] = qdag_rel_T.bitvector;
    Q[3] = qdag_rel_U.bitvector;

    vector<vector<rank_support_v<1>>> ranks(4);

    ranks[0] = qdag_rel_R.ranks;
    ranks[1] = qdag_rel_S.ranks;
    ranks[2] = qdag_rel_T.ranks;
    ranks[3] = qdag_rel_U.ranks;

    vector<bit_vector> Join_Result;
  
    high_resolution_clock::time_point start, stop;
    double total_time = 0.0;       
    duration<double> time_span;

    uint8_t h = (uint8_t) std::round(log2(grid_side))*4;

    Join_Result=join(Q,ranks, {att_R,att_S,att_T,att_U},h, 4);
    
    start = high_resolution_clock::now();    
    
    Join_Result=join(Q,ranks, {att_R,att_S,att_T,att_U},h, 4);

    stop = high_resolution_clock::now();
    time_span = duration_cast<microseconds>(stop - start);
    total_time = time_span.count();    

    cout << /*"Multiway Join ended in " <<*/ total_time /*<< " seconds"*/ << endl;
    uint64_t bits = 0;
    for (auto& vector : Join_Result) {
        bits += vector.size();
    }

    cout << bits << endl;

    return 0;
}
