#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <iostream>
using namespace std;

class Product
{
    int id;

    double price;

    string productName;

public:
    static const string class_name; // Cada clase tiene un static llamado : class_name

    Product(int _key, double _price, string _product) : id(_key), price(_price), productName(_product) {}

    Product()
    {
        id = 0;

        price = 0;

        productName = "";
    };

    void print()
    {
        cout << " Product Object : " << id << ", " << price << ", " << productName << endl;
    };

    ~Product(){};

    // Definición de la función amiga sobrecargada <<
    friend ostream &operator<<(ostream &os, const Product &product)
    {
        os << "object :" << product.class_name << "{ ID: " << product.id << ", price: " << product.price << ", product name: " << product.productName << "}";
        return os;
    }

    int get_id()
    {
        return id;
    }

    double get_price()
    {
        return price;
    }

    string get_product_name()
    {
        return productName;
    }

    void set_id(int id)
    {

        this->id = id;
    }

    void set_price(double price)
    {

        this->price = price;
    }

    void set_product_name(string name)
    {

        this->productName = name;
    }
};

// Definición de la variable class_name
const std::string Product::class_name = "ProductClass";

#endif