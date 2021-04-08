#define BOOST_TEST_MODULE  TEST_NEW_VECTOR
#define BOOST_TEST_DYN_LINK
#include<boost/numeric/ublas/matrix/vector.hpp>
#include <boost/test/unit_test.hpp>
#include<iostream>
#include<vector>

using namespace boost::numeric::ublas::new_implementation;

BOOST_AUTO_TEST_SUITE(test_new_vector)

BOOST_AUTO_TEST_CASE(vector_intialization_using_initializer_list){
    vector<int> v1{1, 2, 3, 4, 5};
    int len=v1.size();
    BOOST_CHECK_EQUAL(5, len);
    for(int i=0; i<len; i++){
        BOOST_CHECK_EQUAL(i+1, v1[i]);
    }
}

BOOST_AUTO_TEST_CASE(vector_initialization_using_iterators){
    double elem[]{45, 23.5, 25.89};
    vector v2(elem, elem+3);
    int len=v2.size();
    BOOST_CHECK_EQUAL(3, len);
    BOOST_CHECK_EQUAL(45, v2[0]);
    BOOST_CHECK_EQUAL(23.5, v2[1]);
    BOOST_CHECK_EQUAL(25.89, v2[2]);
}

BOOST_AUTO_TEST_CASE(vector_initialization_with_same_element){
    vector v3(5, 23.65);
    int len=v3.size();
    BOOST_CHECK_EQUAL(5, len);
    for(int i=0; i<v3.size(); i++){
        BOOST_CHECK_EQUAL(23.65, v3[i]);
    }
}

BOOST_AUTO_TEST_CASE(vector_copy_constructor){
    vector v3(5, 23.65);
    vector v4=v3;
    int len=v4.size();
    BOOST_CHECK_EQUAL(5, len);
    for(int i=0; i<v4.size(); i++){
        BOOST_CHECK_EQUAL(23.65, v4[i]);
    }
}

BOOST_AUTO_TEST_CASE(vector_addition_operation){
    vector v5{0, 1, 2, 3, 4};
    vector v6(5, 1);
    v5+=v6;
    for(int i=0; i<5; i++){
        BOOST_CHECK_EQUAL(i+1, v5[i]);
    }
}

BOOST_AUTO_TEST_CASE(vector_equality_operator){
    vector v5{0, 1, 2, 3, 4};
    vector v6{0, 1, 2, 3, 4};
    BOOST_CHECK_EQUAL(v5==v6);
}

BOOST_AUTO_TEST_CASE(vector_norm){
    vector v7{3, 4};
    BOOST_CHECK_EQUAL(v7.norm(), 5)
}

BOOST_AUTO_TEST_CASE(vector_iterators_begin_end){
    vector v8{0, 1, 2, 3, 4};
    int i=0;
    for(auto x: v8){
        BOOST_CHECK_EQUAL(x, v8[i]);
        i++;
    }
    BOOST_CHECK_EQUAL(v8[3], *(v8.begin()+3));
}

BOOST_AUTO_TEST_CASE(vector_scalar_multiplication_division){
    vector v9{0, 1, 2, 3, 4};
    v9=3*v9;
    for(int i=0; i<v9.size(); i++){
        BOOST_CHECK_EQUAL(2*i, v9[i]);
    }
    v9=v9/3;
    for(int i=0; i<v9.size(); i++){
        BOOST_CHECK_EQUAL(i, v9);
    }
}

BOOST_AUTO_TEST_SUITE_END()