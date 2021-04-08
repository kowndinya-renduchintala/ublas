#define BOOST_TEST_MODULE TEST_NEW_MATRIX
#define BOOST_TEST_DYN_LINK
#include<boost/numeric/ublas/matrix/matrix.hpp>
#include<boost/test/unit_test.hpp>
#include<iostream>
#include<vector>

using namespace boost::numeric::ublas::new_implementation;

BOOST_AUTO_TEST_SUITE(test_new_matrix)

BOOST_AUTO_TEST_CASE(matrix_initialize_with_STL_vectors){
    std::vector<double> v1{1, 4, 7};
    std::vector<double> v2{2, 5, 8};
    std::vector<double> v3{3, 6, 9};
    std::vector<std::vector<double>> mtrx;
    mtrx.push_back(v1);
    mtrx.push_back(v2);
    mtrx.push_back(v3);

    matrix<double> M1(mtrx);
    BOOST_CHECK_EQUAL(1, M1(0, 0));
    BOOST_CHECK_EQUAL(2, M1(0, 1));
    BOOST_CHECK_EQUAL(3, M1(0, 2));
    BOOST_CHECK_EQUAL(4, M1(1, 0));
    BOOST_CHECK_EQUAL(5, M1(1, 1));
    BOOST_CHECK_EQUAL(6, M1(1, 2));
    BOOST_CHECK_EQUAL(7, M1(2, 0));
    BOOST_CHECK_EQUAL(8, M1(2, 1));
    BOOST_CHECK_EQUAL(9, M1(2, 2));
    auto [m, n]=M1.size();
    BOOST_CHECK_EQUAL(m, 3);
    BOOST_CHECK_EQUAL(n, 3);
}

BOOST_AUTO_TEST_CASE(matrix_copy_constructor){
    std::vector<double> v1{1, 4, 7};
    std::vector<double> v2{2, 5, 8};
    std::vector<double> v3{3, 6, 9};
    std::vector<std::vector<double>> mtrx;
    mtrx.push_back(v1);
    mtrx.push_back(v2);
    mtrx.push_back(v3);

    matrix<double> M2(mtrx);
    matrix<double> M3=M2;
    BOOST_CHECK_EQUAL(1, M3(0, 0));
    BOOST_CHECK_EQUAL(2, M3(0, 1));
    BOOST_CHECK_EQUAL(3, M3(0, 2));
    BOOST_CHECK_EQUAL(4, M3(1, 0));
    BOOST_CHECK_EQUAL(5, M3(1, 1));
    BOOST_CHECK_EQUAL(6, M3(1, 2));
    BOOST_CHECK_EQUAL(7, M3(2, 0));
    BOOST_CHECK_EQUAL(8, M3(2, 1));
    BOOST_CHECK_EQUAL(9, M3(2, 2));
    auto [m, n]=M3.size();
    BOOST_CHECK_EQUAL(m, 3);
    BOOST_CHECK_EQUAL(n, 3);
}

BOOST_AUTO_TEST_CASE(generating_submatrix_instance){
    std::vector<double> v1{1, 4, 7};
    std::vector<double> v2{2, 5, 8};
    std::vector<double> v3{3, 6, 9};
    std::vector<std::vector<double>> mtrx;
    mtrx.push_back(v1);
    mtrx.push_back(v2);
    mtrx.push_back(v3);

    matrix<double> M4(mtrx);
    matrix<double> M5;
    M5=M4({0, 2}, {1, 2});
    
    auto [m, n]=M4.size();
    BOOST_CHECK_EQUAL(m, 3);
    BOOST_CHECK_EQUAL(n, 2);

    BOOST_CHECK_EQUAL(2, M5(0, 0));
    BOOST_CHECK_EQUAL(3, M5(0, 1));
    BOOST_CHECK_EQUAL(5, M5(1, 0));
    BOOST_CHECK_EQUAL(6, M5(1, 1));
    BOOST_CHECK_EQUAL(8, M5(2, 0));
    BOOST_CHECK_EQUAL(9, M5(2, 1));
}

BOOST_AUTO_TEST_CASE(modify_submatrix_modifies_parent_matrix){
    std::vector<double> v1{1, 4, 7};
    std::vector<double> v2{2, 5, 8};
    std::vector<double> v3{3, 6, 9};
    std::vector<std::vector<double>> mtrx;
    mtrx.push_back(v1);
    mtrx.push_back(v2);
    mtrx.push_back(v3);

    matrix<double> M6(mtrx);
    matrix<double> test(2, 2);
    test(0, 0)=1;
    test(1, 1)=2;
    print_matrix(test);

    M6({0, 1}, {0, 1})=test;
    BOOST_CHECK_EQUAL(2, M6(1, 1));
    BOOST_CHECK_EQUAL(0, M6(0, 1));
    BOOST_CHECK_EQUAL(0, M6(1, 0));
}

BOOST_AUTO_TEST_CASE(matrix_transpose){
    std::vector<double> v1{1, 4, 7};
    std::vector<double> v2{2, 5, 8};
    std::vector<double> v3{3, 6, 9};
    std::vector<std::vector<double>> mtrx;
    mtrx.push_back(v1);
    mtrx.push_back(v2);
    mtrx.push_back(v3);

    matrix<double> M7(mtrx);
    matrix<double> M8=M7.transpose();
    
    BOOST_CHECK_EQUAL(1, M8(0, 0));
    BOOST_CHECK_EQUAL(4, M8(0, 1));
    BOOST_CHECK_EQUAL(7, M8(0, 2));
    BOOST_CHECK_EQUAL(2, M8(1, 0));
    BOOST_CHECK_EQUAL(5, M8(1, 1));
    BOOST_CHECK_EQUAL(8, M8(1, 2));
    BOOST_CHECK_EQUAL(3, M8(2, 0));
    BOOST_CHECK_EQUAL(6, M8(2, 1));
    BOOST_CHECK_EQUAL(9, M8(2, 2));
}

BOOST_AUTO_TEST_CASE(matrix_multiplication){
    std::vector<double> v1{1, 4, 7};
    std::vector<double> v2{2, 5, 8};
    std::vector<double> v3{3, 6, 9};
    std::vector<std::vector<double>> mtrx;
    mtrx.push_back(v1);
    mtrx.push_back(v2);
    mtrx.push_back(v3);

    matrix<double> M9(mtrx);
    matrix<double> M10=M9.transpose();

    matrix<double> M11=M9*M10;
    BOOST_CHECK_EQUAL(14, M11(0, 0));
    BOOST_CHECK_EQUAL(32, M11(0, 1));
    BOOST_CHECK_EQUAL(50, M11(0, 2));
    BOOST_CHECK_EQUAL(32, M11(1, 0));
    BOOST_CHECK_EQUAL(77, M11(1, 1));
    BOOST_CHECK_EQUAL(122, M11(1, 2));
    BOOST_CHECK_EQUAL(50, M11(2, 0));
    BOOST_CHECK_EQUAL(122, M11(2, 1));
    BOOST_CHECK_EQUAL(194, M11(2, 2));

    matrix<double> M12(1, 3);
    matrix<double> M13(3, 1);
    M12(0, 0)=1;
    M12(0, 1)=2;
    M12(0, 2)=3;
    M13(0, 0)=1;
    M13(1, 0)=2;
    M13(2, 0)=3;
    matrix<double> M14=M12*M13;
    auto [m, n]=M14.size();
    BOOST_CHECK_EQUAL(m, 1);
    BOOST_CHECK_EQUAL(n, 1);
    BOOST_CHECK_EQUAL(M14(0, 0), 14);
}


BOOST_AUTO_TEST_SUITE_END()