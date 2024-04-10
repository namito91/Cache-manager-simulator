#include "cache.h"
#include "product.h"

int main()
{
  CacheManager<Product> my_cache(3);

  my_cache.insert("1", Product(0, 14.5, "amd-ryzen-7-5800h"));

  my_cache.insert("2", Product(1, 15.99, "intel-i5-2500k-2nd-gen"));

  my_cache.insert("3", Product(3, 5.99, "intel-i3-1550h-1st-gen"));

  my_cache.insert("1", Product(4, 12.99, "intel-5500"));

  my_cache.insert("1", Product(2, 5.99, "intel-i7-3770k-3rd-gen"));

  my_cache.insert("3", Product(2, 115.99, "xeon-3rd-gen"));

  my_cache.insert("4", Product(2, 25.99, "mips-r5000"));

  my_cache.insert("7", Product(2, 50.99, "amd-ryzen5"));

  my_cache.insert("1", Product(2, 5.99, "rtx-3080ti"));

  my_cache.insert("8", Product(2, 1.99, "asus-rog"));

  my_cache.insert("1", Product(4, 4.99, "M3-APPL"));

  my_cache.show_cache_data();

  // IMPORTANTE
  // se deben buscar los datos atraves de las claves insertadas en main,ya que los datos en file poseen otras claves.
  //  Al actualizar su contenido,sus claves de reinician,

  Product p = my_cache.get("2"); // insertar cualquier clave,el sistema informa si no se encuentra

  cout << "product found : " << p.get_product_name() << endl;

  my_cache.show_cache_data();

  Product p2 = my_cache.get("4");

  my_cache.show_cache_data();

  Product p3 = my_cache.get("2");

  my_cache.show_cache_data();

  Product p4 = my_cache.get("4");

  my_cache.show_cache_data();

  Product p5 = my_cache.get("8");

  my_cache.show_cache_data();

  return 0;
}
