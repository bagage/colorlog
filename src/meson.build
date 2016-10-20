colorlog_files = [
                'utils.cc',
                'Rules.cc',
                'RuleBox.cc',
                'Config.cc',
                'CLHandler.cc',
                'PipedChild.cc',
              ]
colorlog_lib = static_library('colorlog', colorlog_files, dependencies: boost_dep)

colorlog_c_lib = static_library('ColorlogCAPI', 'colorlogCAPI.cc', link_with: colorlog_lib)
colorlog_exe = executable('colorlog', 'main.cc', link_with: colorlog_lib, install: true)

#cppcheck
run_target('cppcheckInfo', command : [ 'cppcheck', '.', '--suppress=missingIncludeSystem', '--error-exitcode=1', '--enable=information'])
run_target('cppcheckUnused', command : [ 'cppcheck', '.', '--suppress=missingIncludeSystem', '--error-exitcode=1', '--enable=unusedFunction'])
run_target('cppcheckStyle', command : [ 'cppcheck', '.', '--suppress=missingIncludeSystem', '--error-exitcode=1', '--enable=style'])
run_target('cppcheckAll', command : [ 'cppcheck', '.', '--suppress=missingIncludeSystem', '--error-exitcode=1', '--enable=all'])
