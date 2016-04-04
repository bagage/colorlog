include(UseGoogleMock)

include_directories(${BASE_SRC_DIR} ${GMOCK_INCLUDE_DIRS})
link_directories(${GMOCK_LIBRARY_DIRS})

add_executable (RulesTest RulesUnitTest.cc IntermediateResultUnitTest.cc RuleBoxUnitTest.cc )
add_executable (ConfigTest ConfigUnitTest.cc )
add_executable (CLHandlerTest CLHanlderUnitTest.cc )

target_compile_options(RulesTest PUBLIC -Wno-overloaded-virtual)
target_compile_options(ConfigTest PUBLIC -Wno-overloaded-virtual)
target_compile_options(CLHandlerTest PUBLIC -Wno-overloaded-virtual)

add_dependencies(RulesTest GMockExternal ${CMAKE_PROJECT_NAME})
add_dependencies(ConfigTest GMockExternal ${CMAKE_PROJECT_NAME})
add_dependencies(CLHandlerTest GMockExternal ${CMAKE_PROJECT_NAME})

target_link_libraries(RulesTest Rules Config ${GMOCK_LIBS} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(ConfigTest Config Rules ${GMOCK_LIBS} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(CLHandlerTest Config Rules CLHandler ${GMOCK_LIBS} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

#every test has its own valgrind version
#so it is easy to turn off time-consuming valgrind tests in ctest while
#debugging
add_test(RulesTest ${EXECUTABLE_OUTPUT_PATH}/RulesTest)
add_test(RulesTest|Valgrind valgrind --leak-check=full
	--undef-value-errors=no -v --error-exitcode=1 --suppressions=${CMAKE_SOURCE_DIR}/tests/valgrind_supp/emutls.supp ${EXECUTABLE_OUTPUT_PATH}/RulesTest --gtest_filter=-*NonVal*)
add_test(ConfigTest ${EXECUTABLE_OUTPUT_PATH}/ConfigTest)
add_test(ConfigTest|Valgrind valgrind --leak-check=full
	--undef-value-errors=no -v --error-exitcode=1 --suppressions=${CMAKE_SOURCE_DIR}/tests/valgrind_supp/emutls.supp ${EXECUTABLE_OUTPUT_PATH}/RulesTest --gtest_filter=-*NonVal*)
add_test(CLHandlerTest ${EXECUTABLE_OUTPUT_PATH}/CLHandlerTest)
add_test(CLHandlerTest|Valgrind valgrind --leak-check=full
	--undef-value-errors=no -v --error-exitcode=1 --suppressions=${CMAKE_SOURCE_DIR}/tests/valgrind_supp/emutls.supp ${EXECUTABLE_OUTPUT_PATH}/CLHandlerTest )
#cppcheck
add_test(cppcheckInfo  cppcheck ${BASE_SRC_DIR} --error-exitcode=1 --enable=information -q )
# add_test(cppcheckUnusedFunctions  cppcheck ${BASE_SRC_DIR} --error-exitcode=1 --enable=unusedFunction -q )
add_test(cppcheckStyle  cppcheck ${BASE_SRC_DIR} --error-exitcode=1 --enable=style -q )
add_test(cppcheckAll  cppcheck ${BASE_SRC_DIR} --error-exitcode=1 --enable=all -q )
