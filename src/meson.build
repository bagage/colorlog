boost_dep = dependency('boost', modules : ['regex'], required: true)
color_files = [
                'utils.cc',
                'Rules.cc',
                'RuleBox.cc',
                'Config.cc',
                'CLHandler.cc',
                'PipedChild.cc',
              ]
color_lib = static_library('color', color_files, dependencies: boost_dep)

static_library('ColorCAPI', 'colorCAPI.cc', link_with: color_lib)
executable('color', 'main.cc', link_with: color_lib)
