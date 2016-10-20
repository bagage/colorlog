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
