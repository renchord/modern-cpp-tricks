
FUNCTION(TestExample exampleName file)
    add_executable(${exampleName} ${file})
ENDFUNCTION()

TestExample(enable_if test_enable_if.cpp)
TestExample(crtp_base test_crtp_base.cpp)
TestExample(is_detect test_is_detect.cpp)
TestExample(static_comb static_combinations.cpp)
TestExample(static_comb2 static_combinations_v2.cpp)
TestExample(static_comb3 static_combinations_v3.cpp)

TestExample(cpp_tmp_2_3 cpp_tmp_chapter2_3.cpp)