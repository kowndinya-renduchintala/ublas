#include<boost/numeric/ublas/matrix/matrix.hpp>
#include<boost/numeric/ublas/matrix/vector.hpp>
#include<iostream>
#include<utility>
#include<vector>

using namespace boost::numeric::ublas::new_implementation;

auto qr(matrix<double>& A){
    auto [m, n]=A.size();
    auto Q=zeros(m, n);
    auto R=zeros(n, n);
    R(0, 0)=norm(A({0, m-1}, 0));
    Q({0, m-1}, 0)=A({0, m-1}, 0)/R(0, 0);
    for(int k=1; k<n; k++){
        R({0, k-1}, k) = Q({0, m-1}, {0, k-1}).transpose() * A({0, m-1}, k);
        auto v=A({0, m-1}, k) - Q({0, m-1}, {0, k-1})*R({0, k-1}, k);
        R(k, k)=norm(v);
        Q({0, m-1}, k)=v/R(k, k);
    }
    return std::pair{Q, R};
}

int main(){
    std::vector<double> v1{1, 4, 0};
    std::vector<double> v2{2, 5, 0};
    std::vector<double> v3{9, 6, 0};
    std::vector<std::vector<double>> mtrx;
    mtrx.push_back(v1);
    mtrx.push_back(v2);
    mtrx.push_back(v3);

    matrix<double> A(mtrx);
    auto [Q, R]=qr(A);
    print_matrix(Q);
    print_matrix(R);
    print_matrix(Q*R);
    if(A==Q*R){
        std::cout << "QR Decomposition Successful..." << std::endl;
    }
    else{
        std::cout << "Wrong QR decomposition..." << std::endl;
    }
}