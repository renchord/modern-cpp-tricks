
FUNCTION(TestExample exampleName file)
    add_executable(${exampleName} ${file})
ENDFUNCTION()

TestExample(enable_if test_enable_if.cpp)
TestExample(crtp_base test_crtp_base.cpp)
TestExample(is_detect test_is_detect.cpp)