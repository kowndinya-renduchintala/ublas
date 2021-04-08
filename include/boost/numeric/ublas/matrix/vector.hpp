#ifndef _BOOST_UBLAS_NEW_VECTOR
#define _BOOST_UBLAS_NEW_VECTOR

#include<initializer_list>
#include<cmath>
#include<algorithm>
#include<stdexcept>
#include<iterator>

namespace boost::numeric::ublas::new_implementation{

    template<typename T>
    class vector{
        T* elem;            //pointer to the first element in the heap
        int m;              //number of elements in the vector
    public:
        vector()=default;                   //default constructor
        vector(int s):elem{new T[s]}, m{s}{}    
        vector(std::initializer_list<T> lst):elem{new T[lst.size()]}, m{static_cast<int>(lst.size())}{ //from initializer_list
            std::copy(lst.begin(), lst.end(), elem);
        }; 
        template<typename Iter>
            vector(Iter b, Iter e){
                m=std::distance(b, e);
                elem=new T[m];
                for(auto it=b, i=0; it!=e; it++, i++){
                    elem[i]=(*it);
                }
            }
        vector(int s, T element):elem{new T[s]}, m{s}{
            for(auto &x: *this)
                x=element;
        }
        ~vector(){
            delete[] elem;
        }                       //destructor
        vector(const vector& v);                        //copy constructor
        vector& operator=(vector& v);                   //copy assignment: clean up target and copy 
        T& operator[](int i);                           
        const T& operator[](int i) const;               
        vector(vector&& a);                             //move constructor
        vector& operator=(vector&& v);                  //move assignment: clean up target and copy
        int size() const; 
        template<typename T1>
            vector& operator+=(vector<T1>& v); 
        template<typename T1>
            vector& operator-=(vector<T1>& v);
        template<typename T1>
            vector& operator*=(vector<T1>& v);
        template<typename T1>
            vector& operator/=(vector<T1>& v);
        bool operator==(const vector& v);
        bool operator!=(const vector& v);
        auto norm();
        T* begin(){return size()? elem: nullptr;}
        T* end(){return size()? elem+size(): nullptr;}
    };

    template<typename Iter>
        vector(Iter b, Iter e)->vector<typename std::iterator_traits<Iter>::value_type>;

    template<typename T>
    vector<T>::vector(const vector& v){
        elem=new T[v.size()];
        m=v.size();
        for(int i=0; i<v.size(); i++){
            elem[i]=v[i];
        }
    }    

    template<typename T>
    vector<T>& vector<T>::operator=(vector& v){
        delete[] elem;
        elem=new T[v.size()];
        m=v.size();
        for(int i=0; i<v.size(); i++){
            elem[i]=v[i];
        }
        return *this;
    }

    template<typename T>
    T& vector<T>::operator[](int i){
        if(i<0 || i>=size()){
            throw std::out_of_range{"Vector::operator[]"};
        }
        else return elem[i];
    }

    template<typename T>
    const T& vector<T>::operator[](int i) const{
        if(i<0 || i>=size()){
            throw std::out_of_range{"Vector::operator[]"};
        }
        else return elem[i];
    }

    template<typename T>
    vector<T>::vector(vector&& v):elem{v.elem}, m{v.m}{
        v.elem=nullptr;
        v.m=0;
    }

    template<typename T>
    vector<T>& vector<T>::operator=(vector&& v){
        elem=v.elem;
        m=v.m;
        v.elem=nullptr;
        v.m=0;
        return *this;
    }

    template<typename T>
    int vector<T>::size() const{
        return m;
    }

    template<typename T>
        template<typename T1>
            vector<T>& vector<T>::operator+=(vector<T1>& v){
                if(m!=v.size())throw "Cannot add vectors of different sizes";
                else{
                    for(int i=0; i<v.size(); i++){
                        elem[i]+=v[i];
                    }
                }
                return *this;
            }

    template<typename T>
        template<typename T1>
            vector<T>& vector<T>::operator-=(vector<T1>&v){
                if(m!=v.size())throw "Cannot subtract vectors of different sizes";
                else{
                    for(int i=0; i<v.size(); i++){
                        elem[i]-=v[i];
                    }
                }
                return *this;
            }


    template<typename T>
        template<typename T1>
            vector<T>& vector<T>::operator*=(vector<T1>&v){
                if(m!=v.size())throw "Cannot element-wise multiply vectors of different sizes";
                else{
                    for(int i=0; i<v.size(); i++){
                        elem[i]*=v[i];
                    }
                }
                return *this;
            }

    template<typename T>
        template<typename T1>
            vector<T>& vector<T>::operator/=(vector<T1>&v){
                if(m!=v.size())throw "Cannot element-wise divide vectors of different sizes";
                else{
                    for(int i=0; i<v.size(); i++){
                        elem[i]/=v[i];
                    }
                }
                return *this;
            }

    template<typename T>
    vector<T> operator*(T elem, vector<T>& v){
        vector<T> res=v;
        for(int i=0; i<v.size(); i++){
            res[i]=v[i]*elem;
        }
    }

    template<typename T>
    vector<T> operator*(vector<T>& v, T elem){
        vector<T> res=v;
        for(int i=0; i<v.size(); i++){
            res[i]=v[i]*elem;
        }
        return res;
    }

    template<typename T>
    vector<T> operator/(vector<T>& v, T elem){
        vector<T> res=v;
        for(int i=0; i<v.size(); i++){
            res[i]=v[i]/elem;
        }
        return res;
    }

    template<typename T>
    bool vector<T>::operator==(const vector& v){
        for(int i=0; i<v.size(); i++){
            if(elem[i]!=v[i])return false;
        }
        return true;
    }

    template<typename T>
    bool vector<T>::operator!=(const vector& v){
        return !(*this==v);
    }

    template<typename T>
    auto vector<T>::norm(){
        T res=0;
        for(int p=0; p<m; p++){
            res+=(elem[p]*elem[p]);
        }
        return std::sqrt(res);
    }

    template<typename T>
    T* begin(vector<T>& x){
        return x.size()? &x[0] : nullptr;
    }

    template<typename T>
    T* end(vector<T>& x){
        return x.size()? &x[0] + x.size() : nullptr;
    }

    template<typename T>
    auto dot(vector<T>& v1, vector<T>& v2){
        vector<T> res=v1;
        res*=v2;
        auto result=0;
        for(auto x: res)
            result+=x;
        return result;
    }
}

#endif