#ifndef _BOOST_UBLAS_NEW_MATRIX
#define _BOOST_UBLAS_NEW_MATRIX

#include<boost/numeric/ublas/matrix/vector.hpp>
#include<vector>
#include<utility>
#include<initializer_list>

namespace boost::numeric::ublas::new_implementation{
    
    template<typename T>
    struct matrix_storage{
        vector<T>* column_vectors;
        int m, n;
    };

    template<typename T>
    class matrix{
    public:
        matrix():data{nullptr}{}
        matrix(int m, int n);
        matrix(std::vector< std::vector<T> >& mtrx);
        matrix(matrix_storage<T>* ptr, int, int, int, int);
        ~matrix();
        matrix(const matrix& M);            //copy constructor
        matrix(matrix&& M);                 //move constructor
        matrix& operator=(const matrix& M); //copy assingment
        matrix& operator=(matrix&& M);      //move assignment
        matrix operator()(std::pair<int, int>, std::pair<int, int>);
        T& operator()(int, int);
        const T operator()(int, int) const;
        matrix operator()(std::pair<int, int>, int);
        matrix operator()(int, std::pair<int, int>);
        std::pair<int, int> size() const;
        matrix transpose();
        bool operator==(const matrix& M);
        bool operator!=(const matrix& M);
    private:
        bool is_submatrix=false;
        matrix_storage<T>* data;
        int i, j, k, l;
    };

    template<typename T>
    matrix<T>::matrix(int m, int n){
        data=new matrix_storage<T>;
        data->column_vectors=new vector<T>[n];
        data->m=m;
        data->n=n;
        i=0; j=m-1;
        k=0; l=n-1;
        for(int p=0; p<n; p++){
            data->column_vectors[p]=vector<T>(m, (T)0);
        }
    }

    template<typename T>
    matrix<T>::matrix(std::vector< std::vector<T> >& mtrx){
        data=new matrix_storage<T>;
        data->column_vectors=new vector<T>[mtrx.size()];
        data->m=mtrx[0].size();
        data->n=mtrx.size();
        i=0; j=(data->m)-1;
        k=0; l=(data->n)-1;
        for(int p=0; p<(data->n); p++){
            data->column_vectors[p]=vector(mtrx[p].begin(), mtrx[p].end());
        }
    }

    template<typename T>
    matrix<T>::matrix(matrix_storage<T>* ptr, int a, int b, int c, int d){
        data=ptr;
        i=a; j=b;
        k=c; l=d;
        is_submatrix=true;
    }

    template<typename T>
    matrix<T>::~matrix(){
        if(!is_submatrix && data!=nullptr){
            //std::cout << "Deleting Matrix..." << "at " << data <<  std::endl;
            delete[] data->column_vectors;
            delete data;
            data=nullptr;
        }
    }

    template<typename T>
    matrix<T>::matrix(const matrix& M){
        //std::cout << "Copy constructor of Matrix" << std::endl;
        auto [m, n]=M.size();
        data=new matrix_storage<T>;
        data->column_vectors=new vector<T>[n];
        data->m=m;
        data->n=n;
        i=0; j=m-1;
        k=0; l=n-1;
        is_submatrix=M.is_submatrix;
        for(int p=M.k, q=0; p<=M.l; p++, q++){
            data->column_vectors[q]=vector(M.data->column_vectors[p].begin()+M.i, M.data->column_vectors[p].begin()+M.j+1);
        }
    }

    template<typename T>
    matrix<T>::matrix(matrix&& M){
        //std::cout << "Move constructor of matrix" << std::endl;
        data=M.data;
        M.data=nullptr;
        i=M.i;
        j=M.j;
        k=M.k;
        l=M.l;
        is_submatrix=M.is_submatrix;
    }

    template<typename T>
    matrix<T>& matrix<T>::operator=(const matrix& M){
        //std::cout << "Copy assignment of matrix" << std::endl;
        if(is_submatrix){
            auto [m, n]=M.size();
            for(int p=0; p<m; p++){
                for(int q=0; q<n; q++){
                    (*this)(p, q)=M(p, q);
                }
            }
        }
        else{
            if(data!=nullptr){
                delete[] data->column_vectors;
                delete data;
            }
            auto [m, n]=M.size();
            data=new matrix_storage<T>;
            data->column_vectors=new vector<T>[n];
            data->m=m;
            data->n=n;
            i=0; j=m-1;
            k=0; l=n-1;
            for(int p=M.k, q=0; p<=M.l; p++, q++){
                data->column_vectors[q]=vector(M.data->column_vectors[p].begin()+M.i, M.data->column_vectors[p].end()+M.j+1);
            }
    }
        return (*this);
    }

    template<typename T>
    matrix<T>& matrix<T>::operator=(matrix&& M){
        //std::cout << "Move assignment of Matrix" << std::endl;
        if(is_submatrix){
            auto [m, n]=M.size();
            for(int p=0; p<m; p++){
                for(int q=0; q<n; q++){
                    (*this)(p, q)=M(p, q);
                }
            }
        }
        else{
            if(data!=nullptr){
                    delete[] data->column_vectors;
                    delete data;
            }
            data=M.data;
            M.data=nullptr;
            i=M.i;
            j=M.j;
            k=M.k;
            l=M.l;
            is_submatrix=M.is_submatrix;
        }
        
        return (*this);
    }

    template<typename T>
    matrix<T> matrix<T>::operator()(std::pair<int, int> row,std::pair<int, int> col){
        return matrix(data, row.first, row.second, col.first, col.second);
    }

    template<typename T>
    matrix<T> matrix<T>::operator()(int a, std::pair<int, int> P){
        matrix<double> res=matrix(data, a, a, P.first, P.second);
        return res;
    }

    template<typename T>
    matrix<T> matrix<T>::operator()(std::pair<int, int> P, int a){
        return matrix(data, P.first, P.second, a, a);
    }

    template<typename T>
    T& matrix<T>::operator()(int row, int col){
        return data->column_vectors[k+col][i+row];
    }

    template<typename T>
    const T matrix<T>::operator()(int row, int col) const{
        return data->column_vectors[k+col][i+row];
    }

    template<typename T>
    matrix<T> matrix<T>::transpose(){
        auto [m, n]=size();         //m=j-i+1(#rows), n=l-k+1(#columns)
        matrix<T> transposed(n, m);
        for(int p=i, r=0; p<=j; p++, r++){
            std::vector<T> temp;
            for(int q=k; q<=l; q++){
                temp.push_back(data->column_vectors[q][p]);
            }
            transposed.data->column_vectors[r]=vector(temp.begin(), temp.end());
        }
        return transposed;
    }

    template<typename T>
    matrix<T> operator+(const matrix<T>& M1, const matrix<T>& M2){
        auto [m, p]=M1.size();
        auto [q, n]=M2.size();
        if(m!=q || p!=n)throw "Matrices are incompatible for subtraction";
        matrix<T> res(m, n);
        for(int a=0; a<m; a++){
            for(int b=0; b<n; b++){
                res(a, b)=M1(a, b)+M2(a, b);
            }
        }
        return res;
    }

    template<typename T>
    matrix<T> operator-(const matrix<T>& M1, const matrix<T>& M2){
        auto [m, p]=M1.size();
        auto [q, n]=M2.size();
        if(m!=q || p!=n)throw "Matrices are incompatible for subtraction";
        matrix<T> res(m, n);
        for(int a=0; a<m; a++){
            for(int b=0; b<n; b++){
                res(a, b)=M1(a, b)-M2(a, b);
            }
        }
        return res;
    }

    template<typename T>
    matrix<T> operator*(const matrix<T>& M1, const matrix<T>& M2){
        auto [m, p]=M1.size();
        auto [q, n]=M2.size();
        if(p!=q)throw "Matrices are incompatible to multiplication";
        matrix<T> res(m, n);
        for(int i=0; i<m; i++){
            for(int j=0; j<n; j++){
                for(int k=0; k<p; k++){
                    res(i, j)+=M1(i, k)*M2(k, j);
                }
            }
        }
        return res;
    }

    template<typename T>
    matrix<T> operator*(const matrix<T>& M, T elem){
        matrix<T>res=M;
        auto [m, n]=M.size();
        for(int i=0; i<m; i++){
            for(int j=0; j<n; j++){
                res(i, j)=M(i, j)*elem;
            }
        }
        return res;
    }

    template<typename T>
    matrix<T> operator*(T elem, const matrix<T>& M){
        matrix<T>res=M;
        auto [m, n]=M.size();
        for(int i=0; i<m; i++){
            for(int j=0; j<n; j++){
                res(i, j)=M(i, j)*elem;
            }
        }
        return res;
    }

    template<typename T>
    matrix<T> operator/(const matrix<T>&M, T elem){
        matrix<T> res=M;
        auto [m, n]=M.size();
        for(int i=0; i<m; i++){
            for(int j=0; j<n; j++){
                res(i, j)=M(i, j)/elem;
            }
        }
        return res;
    }

    template<typename T>
    std::pair<int, int> matrix<T>::size() const{
        return {j-i+1, l-k+1};
    }

    template<typename T>
    bool matrix<T>::operator==(const matrix<T>& M){
        auto [m, n]=M.size();
        for(int p=0; p<m; p++){
            for(int q=0; q<n; q++){
                if((*this)(i, j)!=M(i, j))return false;
            }
        }
        return true;
    }

    template<typename T>
    bool matrix<T>::operator!=(const matrix<T>& M){
        return !((*this)==M);
    }

    matrix<double> zeros(int m, int n){
        matrix<double> res(m, n);
        return res;
    }

    template<typename T>
    auto norm(const matrix<T>& M){
        if(M.i!=M.j && M.k!=M.l){
            throw "Not row vector or column vector";
        }
        else{
            auto [m, n]=M.size();
            auto res=(T)0;
            for(int i=0; i<m; i++){
                for(int j=0; j<n; j++){
                    res+=(M(i, j)*M(i, j));
                }
            }
            return std::sqrt(res);
        }
    }

    template<typename T>
    void print_matrix(const matrix<T>& M){
        auto [m, n]=M.size();
        std::cout << "Size " << m << "x" << n << std::endl;
        for(int i=0; i<m; i++){
            for(int j=0; j<n; j++){
                std::cout << M(i, j) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }   
}

#endif