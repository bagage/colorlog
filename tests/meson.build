incdir = include_directories('../src')

test_deps = [ dependency('gtest', main: true), dependency('gmock'), boost_dep, dependency('threads') ]

extra_flags = ['-Wno-overloaded-virtual', '-w' ]

rulestest_exe = executable('RulesTest', 'RulesUnitTest.cc', 'IntermediateResultUnitTest.cc', 'RuleBoxUnitTest.cc',
        include_directories: incdir,
        dependencies: test_deps,
        link_with: colorlog_lib,
        cpp_args: extra_flags)
configtest_exe = executable('ConfigTest', 'ConfigUnitTest.cc',
        include_directories: incdir,
        dependencies: test_deps,
        link_with: colorlog_lib,
        cpp_args: extra_flags)
clhandlertest_exe = executable('CLHandlerTest', 'CLHanlderUnitTest.cc',
        include_directories: incdir,
        dependencies: test_deps,
        link_with: colorlog_lib,
        cpp_args: extra_flags)

valgrind_supp = join_paths(meson.current_source_dir(), 'valgrind_supp/emutls.supp')
valgrind_args = ['--undef-value-errors=no', '-v', '--suppressions=' + valgrind_supp]
test('RulesTest', rulestest_exe, valgrind_args: valgrind_args)
test('ConfigTest', configtest_exe, valgrind_args: valgrind_args)
test('CLHandlerTest', clhandlertest_exe, valgrind_args: valgrind_args)
