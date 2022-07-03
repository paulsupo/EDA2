#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <utility>
#include <cmath>
#include <algorithm>
#include <random>
using namespace std;
struct punto;

template<typename t>
bool ord_segundo(const pair<punto*, t>& a, const pair<punto*, t>& b) {
    return (a.second < b.second);
}

int numero_random(int minimo = 1, int maximo = 100){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(minimo, maximo);
    return distrib(gen);
}

struct punto{
    int dimension;
    vector<float> coordenadas;

    punto(){
        dimension = 0;
    }

    punto(int d, bool random_bool = false, int minimo = 1, int maximo = 100){
        dimension = d;
        if (random_bool)
            for (int i = 0; i < dimension; i++)
                coordenadas.push_back(numero_random(minimo, maximo));

        else
            for (int i = 0; i < dimension; i++)
                coordenadas.push_back(0);
    }

    punto(const punto& d) {
        dimension = d.dimension;
        coordenadas = d.coordenadas;
    }

    punto(int d, vector<float> coorde) {
        dimension = d;
        coordenadas = coorde;
    }

    punto(vector<float> coorde){
        dimension = coorde.size();
        coordenadas = coorde;
    }

    float& operator[](int index){
        return coordenadas[index];
    }

    punto& operator=(punto& d){
        d.dimension = dimension;
        d.coordenadas = coordenadas;
        return *this;
    }


    bool es_igual(punto d){
        if (d.dimension != dimension)
            return false;
        for (int i = 0; i < dimension; i++)
            if ((*this)[i] != d[i])
                return false;
        return true;
    }

    double diferencia_eucladiana(punto p){
        double temp = 0;
        for (int i = 0; i < dimension; i++){
            temp += pow(p.coordenadas[i] - coordenadas[i], 2);
        }
        return sqrt(temp);
    }

};

struct espacio_muldidimencional{
    vector<punto> puntos;
    int dimension;
    int capacidad;
    
    espacio_muldidimencional(int d, int c){
        capacidad = c;
        dimension = d;

        for (int i = 0; i < capacidad; i++)
            puntos.push_back(punto(dimension, true, 0, 10));
    }

    int unir_puntos_curvaz(punto* pptr, vector<pair<punto*, unsigned long long>> curvaz) {
        int i = 0;
        for (; i < capacidad; i++) {
            if (pptr == curvaz[i].first){
                return i;
            }
        }
    }
   
    vector<pair<punto*, unsigned long long>> crear_curva(){
        vector<pair<punto*, unsigned long long>> curvaz;

        for (int i = 0; i < capacidad; i++){
            vector<string> dim_binaria;
            for (int j = 0; j < dimension; j++){
                dim_binaria.push_back(bitset<4>(puntos[i].coordenadas[j]).to_string());
            }

            string stringz;
            for (int j = 0; j < 4; ++j){
                for (int k = 0; k < dimension; k++){
                    stringz += dim_binaria[k][j];
                }
            }
            curvaz.push_back(pair<punto*, unsigned long long>{&puntos[i], bitset<40>(stringz).to_ullong()});
        }
        sort(curvaz.begin(), curvaz.end(), ord_segundo<unsigned long long>);
        return curvaz;
    }

    vector<pair<punto*, float>> k_vecinos_cercanos(punto* pptr, int k) {
        vector<pair<punto*, float>> temp;

        for (int i = 0; i < capacidad; i++) {
            temp.push_back(pair<punto*, float>{&puntos[i], pptr->diferencia_eucladiana(puntos[i])});
        }

        sort(temp.begin(), temp.end(), ord_segundo<float>);
        temp.erase(temp.begin());
        temp.erase(temp.begin() + k, temp.end());

        return temp;
    }

    vector<pair<punto*, float>> curvaz_k_vecinos_cercanos(punto* pptr, int k, vector<pair<punto*, unsigned long long>> curvaz){
        int unir_indice = unir_puntos_curvaz(pptr, curvaz);
        vector<pair<punto*, float>> temp;

        for (int i = 0; i < capacidad; i++){
            unsigned long long distancia;
            if (curvaz[unir_indice].second > curvaz[i].second)
                distancia = curvaz[unir_indice].second - curvaz[i].second;
            else
                distancia = curvaz[i].second - curvaz[unir_indice].second;

            temp.push_back(pair<punto*, float>{curvaz[i].first, distancia});
        }

        sort(temp.begin(), temp.end(), ord_segundo<float>);
        temp.erase(temp.begin());
        temp.erase(temp.begin() + k, temp.end());

        return temp;
    }

    int conincidencias(vector<pair<punto*, float>> vec1, vector<pair<punto*, float>> vec2, int k){
        int temp = 0;
        for (int i = 0; i < k; i++){
            for (int j = 0; j < k; j++){
                if (vec1[i].first->es_igual(*vec2[j].first)){
                    temp++;
                    break;
                }
            }
        }
        return temp;
    }

    void prueba(int k, int cantidad){
        for (int i = 0; i < cantidad; i++){
            punto* random_punto = &puntos[numero_random(0, capacidad - 1)];
            vector<pair<punto*, unsigned long long>> curvaz = crear_curva();
            vector<pair<punto*, float>> knn = k_vecinos_cercanos(random_punto, k);
            vector<pair<punto*, float>> zcknn = curvaz_k_vecinos_cercanos(random_punto, k, curvaz);
            cout << float(conincidencias(knn, zcknn, k)) / k * 100 << " ";
        }
    }
};

int main(){
    int k = 10;
    int cantidad[] = {5, 10, 15, 20 };
    int dimension = 10;
    int capacidad = 5000;
    espacio_muldidimencional curvaz(dimension, capacidad);

    curvaz.prueba(k, cantidad[1]);
    cout << endl << "Termine" << endl;
}
