boost_dep = dependency('boost', modules : ['regex'], required: true)
colorlog_files = [
                'utils.cc',
                'Rules.cc',
                'RuleBox.cc',
                'Config.cc',
                'CLHandler.cc',
                'PipedChild.cc',
              ]
colorlog_lib = static_library('colorlog', colorlog_files, dependencies: boost_dep)

static_library('ColorlogCAPI', 'colorlogCAPI.cc', link_with: colorlog_lib)
executable('colorlog', 'main.cc', link_with: colorlog_lib)
