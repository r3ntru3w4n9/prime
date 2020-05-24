#include <vector>
namespace doctry{
    template <class T>
    class heap<T>
    {
    public:
        heap(size_t s = 0,bool max = 1):_max(max) //s is the expected size. If max = 1 -> maxHeap. If max = 0 -> minHeap
        { if(s) _data.reserve(s); }
        ~heap() {}

        //functions
        void clear() { _data.clear(); }
        void heapify(T& t)
        {}
    private:
        std::vector<T>  _data;
        bool            _max;
        bool comp(const T& a,const T& b) { if(_max) return a>b; else return a<b; }//the compare function
    }
};