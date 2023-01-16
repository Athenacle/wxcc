include(GTest)
enable_testing()

set(TEST_NAME ${PROJECT_NAME}_all_in_one)

add_library(test STATIC test.cc test_lib.cc)
target_link_libraries(test PRIVATE GTest::GTest GTest::GMock libs)
target_include_directories(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/includes)

set(LIBS)
macro(add_subdirectory_test)
    file(GLOB files ${ARGN}/*.cc)
    foreach(file ${files})
        get_filename_component(basename ${file} NAME_WE)
        set(TN test_${ARGN}_${basename})
        add_executable(${TN} ${file})
        list(APPEND LIBS ${file})
        target_link_libraries(${TN} PRIVATE GTest::GTest GTest::GMock libs test)
        target_include_directories(${TN} PRIVATE ${CMAKE_SOURCE_DIR}/lib)
        add_test(${TN} ${TN} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    endforeach()
endmacro()

add_subdirectory_test(lex)

add_executable(${TEST_NAME} ${LIBS})
add_test(${TEST_NAME} ${TEST_NAME})
target_link_libraries(${TEST_NAME} PRIVATE GTest::GTest GTest::GMock libs test)
target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib)