
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
TestExample(cpp_tmp_2_4 cpp_tmp_chapter2_4.cpp)
TestExample(cpp_tmp_2_5 cpp_tmp_chapter2_5.cpp)

TestExample(test_reference_wrapper test_reference_wrapper.cpp)

TestExample(template_basic_features template_basic_features.cpp)
TestExample(variadic_template variadic_template.cpp)
TestExample(template_param_deduction template_param_deduction.cpp)
TestExample(template_action template_action.cpp)
TestExample(my_shared_ptr my_shared_ptr.cpp)
TestExample(advanced_topic_2 advanced_topic_2.cpp)

TestExample(odd_even odd_even.cpp)
TestExample(quick_sort quick_sort.cpp)
TestExample(producer_consumer producer_consumer.cpp)
TestExample(cache_estimate cache_estimate.cpp)
TestExample(queue queue.cpp)