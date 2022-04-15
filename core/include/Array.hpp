#include <string.h>
#include <iostream>

namespace Basalt{

    template<typename T>
    struct Array
    {
        using val_t = T;
        using ref_t = T&;
        using ptr_t = T*;
        /* A local view other the Array object */
        struct View
        {
        private:
            const ptr_t _start, _end;
        public:
            View(const ptr_t start, const ptr_t end): _start(start), _end(end){}
            const ref_t operator[] (size_t i) const { return _start[i]; }
            ref_t operator[] (size_t i)  = delete;
            const ptr_t begin() const { return _start; }
            ptr_t begin()  = delete;
            const ptr_t end() const { return _end; }
            ptr_t end()  = delete;
            size_t size() const { return _end - _start; }
            const ptr_t at(size_t n) const { return _start + n; }
            ptr_t at(size_t n) = delete;
            View sub(size_t start) const { return View(_start+start, _end); }
            View sub(size_t start, size_t size) const { return View(_start+start, _start+size); }
        };
    protected:
        ptr_t _data = nullptr;
        ptr_t _end = nullptr;
    public:
        Array() {}
        Array(size_t size){
            _data = new val_t[size];
            _end = _data + size;
        }
        Array(size_t size, ptr_t data){
            _data = new val_t[size];
            _end = _data + size;
            for(size_t i=0; i<size; ++i)
                _data[i] = data[i];
        }
        Array(size_t size, const val_t& val){
            _data = new val_t[size];
            _end = _data + size;
            for(ptr_t p=_data; p<_end; ++p)
                *p = val;
        }
        Array(const Array<T>& other){
            size_t size = other._end - other._data;
            _data = new val_t[size];
            _end = _data + size;
            for(size_t i=0; i<size; ++i)
                _data[i] = other._data[i];
        }
        Array(Array<T>&& other): _data(other._data), _end(other._end){
            other._data = other._end = nullptr;
        }
        void operator=(const Array<T>& other){
            delete[] _data;
            size_t size = other._end - other._data;
            _data = new val_t[size];
            _end = _data + size;
            for(size_t i=0; i<size; ++i)
                _data[i] = other._data[i];
        }
        void operator=(Array<T>&& other){
            std::swap(this->_data, other._data);
            std::swap(this->_end, other._end);
        }
        template<typename... Arg_t>
        val_t& emplace(size_t index, Arg_t... args)
        {
            _data[index] = val_t(std::forward<Arg_t>(args)...);
            return _data[index];
        }
        ref_t operator[](size_t i){ return _data[i]; }
        const ref_t operator[] (size_t i) const { return _data[i]; }
        ptr_t begin() { return _data; }
        ptr_t end() { return _end; }
        const ptr_t begin() const { return _data; }
        const ptr_t end() const { return _end; }
        size_t size() const { return _end - _data; }
        ptr_t at(size_t n) { return _data + n; }
        View view() const { return View(_data, _end); }
        View sub(size_t start, size_t size) const { return View(_data+start, _data+start+size); }
        View sub(size_t start) const { return View(_data+start, _end); }
        virtual ~Array() { delete[] _data;  }
    };
    
    
}
