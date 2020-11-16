#include <vector>
#include <algorithm>

using namespace std;

template<typename T>
class CircularVector {
public:
    CircularVector(int max_size): idx(0), max_size(max_size) {
        vec = vector<T>(max_size);
    }

    void push_back(T& elt) {
        idx = idx++ % max_size;
        vec[idx] = elt;
    }

    bool contains(const T& e) {
        return std::find(vec.begin(), vec.end(), e) != vec.end();
    }

private:
    int max_size;
    int idx;
    vector<T> vec;
};