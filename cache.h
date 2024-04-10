#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <string>
#include <vector>

using namespace std;

template <class T>
class CacheManager
{
    // members ( private )
    // cache capacity
    int capacity;

    // lleva el conteo de los pair existentes en el map de cache,
    int cacheCounter = 0;

    // lleva el conteo MRU,
    int MRU = 0;

    map<string, pair<T, int>> cache_data; // << Clave , < Obj ,Indice de Uso >>

    // productos totales,actualizados
    // son los datos que se guardan en file
    map<string, pair<double, string>> file_data;

    bool write_file(string, T);

    // se divide la funcionalidad de cada mètodo,(en helper functions)
    // para facilitar la implementaciòn y legibilidad del mismo,
    bool search_key_in_file(string);

    void update_data_in_file();

    void write(string, double, string);

    void delete_all_data_in_file();

    T get_object_from_file(string);

    bool search_key_in_cache(string);

    void insert_data_in_cache(string, T);

    void update_data_in_cache(string, T);

    void delete_LRU_in_cache();

public:
    CacheManager(int cap)
    {
        capacity = cap;
    }; // recibe la capacidad en el int

    ~CacheManager(){};

    void insert(string, T);

    T get(string);

    // Agregar todas las funciones necesarias
    map<string, pair<T, int>> get_cache_data()
    {
        return this->cache_data;
    }

    void show_cache_data();
};

//
//
template <class T>
bool CacheManager<T>::write_file(string key, T obj)
{

    double price = obj.get_price();

    string name = obj.get_product_name();

    // actualiza la data en el map de file,si ya existe esa clave
    if (file_data.find(key) != file_data.end())
    {
        file_data.find(key)->second.first = price;

        file_data.find(key)->second.second = name;

        delete_all_data_in_file();

        update_data_in_file();

        return true;
    }

    file_data.insert(make_pair(key, make_pair(price, name)));

    write(key, price, name);

    return true;
}

//
//
template <class T>
void CacheManager<T>::update_data_in_file()
{
    cout << "sobreescribiendo clave" << endl;

    int id = 1;

    double price;

    string name;

    // escribe en file,la lista actulizada de productos
    auto pair = file_data.begin();

    while (pair != file_data.end())
    {
        // construye nuevo id,para cada dato en file,
        string str = to_string(id);

        price = pair->second.first;

        name = pair->second.second;

        write(str, price, name);

        id++;

        pair++;
    }
}

//
//
template <class T>
void CacheManager<T>::write(string key, double price, string name)
{
    // apertura escritura
    ofstream o_file{"./file.txt", std::ios::out | std::ios::app};

    if (!o_file)
    {
        cerr << "error opening file" << endl;
    }

    o_file << key << " " << price << " " << name << "\n";

    o_file.close();
}

//
// borrar contenido del file, (para luego actualizar con nuevos datos)
template <class T>
void CacheManager<T>::delete_all_data_in_file()
{
    ofstream o_file{"./file.txt", std::ios::out | std::ios::trunc};

    if (!o_file)
    {
        cerr << "error opening file" << endl;
    }

    o_file.close();
}

//
template <class T>
bool CacheManager<T>::search_key_in_file(string key)
{

    if (file_data.find(key) != file_data.end())
    {
        return true;
    }

    return false;
}

//
template <class T>
T CacheManager<T>::get_object_from_file(string key)
{
    int id = stoi(key);

    double price = file_data.find(key)->second.first;

    string name = file_data.find(key)->second.second;

    T p(id, price, name);

    return p;
}

//
// INSERT , se ingresa la data en la cache y en files,
// si existe en cache o files,se actualiza,
// se actualiza el MRU ,pasa a ser el mas alto,
template <class T>
void CacheManager<T>::insert(string key, T obj)
{
    insert_data_in_cache(key, obj);

    // escribe la data en file,
    write_file(key, obj);
}

template <class T>
void CacheManager<T>::insert_data_in_cache(string key, T obj)
{
    if (search_key_in_cache(key))
    {
        cout << "actualizando key en cache : " << key << endl;

        update_data_in_cache(key, obj);

        return;
    }

    if (cacheCounter == capacity)
    {
        delete_LRU_in_cache();
    }

    cache_data.insert(make_pair(key, make_pair(obj, ++MRU)));

    cacheCounter++;
}

//
template <class T>
void CacheManager<T>::update_data_in_cache(string key, T obj)
{
    // actualizar objeto y MRU
    cache_data.find(key)->second.first = obj;

    cache_data.find(key)->second.second = ++MRU;
}

//
// busca en cache o files ,el objeto asociado a la key,y lo retorna e inserta en cache(si esta en file)
template <class T>
T CacheManager<T>::get(string key)
{
    T p;

    if (search_key_in_cache(key))
    {
        // actualizar MRU.
        cache_data.find(key)->second.second = ++MRU;

        // se toma el objeto con esa key
        p = cache_data.find(key)->second.first;
    }
    else if (search_key_in_file(key))
    {
        cout << "clave encontrada en file : " << key << endl;

        delete_LRU_in_cache();

        p = get_object_from_file(key);

        // se inserta en cache,
        cache_data.insert(make_pair(key, make_pair(p, ++MRU)));
    }
    else
    {
        cout << "clave no encontrada : " << key << endl;
    }

    // si no es encontrada la key, se retorna un objeto vacio,
    return p;
}

//
template <class T>
bool CacheManager<T>::search_key_in_cache(string key)
{
    if (cache_data.find(key) != cache_data.end())
    {
        return true;
    }

    return false;
}

//
template <class T>
void CacheManager<T>::delete_LRU_in_cache()
{
    // buscar el menor MRU
    // toma el primer pair
    auto pair = cache_data.begin();

    // iterador temporal,guarda el pair con mru mas bajo,se inicializa al 1er pair,
    auto pair_tmp = cache_data.begin();

    // guarda el mru de cada pair,
    int mru;

    // mru temp , guarda el mru temporal mas bajo,
    // se inicializa al mru del 1er pair,
    int mru_tmp = pair->second.second;

    while (pair != cache_data.end())
    {
        mru = pair->second.second;

        if (mru < mru_tmp)
        {
            mru_tmp = mru;

            // se apunta al pair con mru temporal mas bajo,
            pair_tmp = pair;
        }

        pair++;
    }

    // borra de la cache, el pair con MRU mas bajo,
    cache_data.erase(pair_tmp);

    cacheCounter--;
}

//
template <class T>
void CacheManager<T>::show_cache_data()
{

    cout << "\nCache Data " << endl;

    cout << endl;

    auto pair = cache_data.begin();

    while (pair != cache_data.end())
    {
        cout << " key : " << pair->first << endl;
        cout << " content : " << pair->second.first << endl;
        cout << " MRU : " << pair->second.second << endl;
        cout << endl;

        pair++;
    }
}

#endif