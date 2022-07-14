#include <vector>
#include <map>
template<typename T>
std::vector<T> vecsum(std::vector<T> a, std::vector<T> b) {
    std::vector<T> c(a.size());
    for (int i = 0; i < a.size(); ++i) 
        c[i] = a[i] + b[i];
    return c;
}

template<typename K, typename V>
void print_map(std::map<K, V> const &m)
{
    for (auto const &pair : m) {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}

template <typename T> 
void print_vector(std::vector<T> vec) {
    for (auto &elem : vec)
        std::cout << elem << ", ";
}
    
