boost_dep = dependency('boost', modules : ['regex'], required: true)
color_lib = static_library('color', 'utils.cc', 'Rules.cc', 'RuleBox.cc', 'Config.cc', 'CLHandler.cc', dependencies: boost_dep)

static_library('ColorCAPI', 'colorCAPI.cc', link_with: color_lib)
executable('color', 'main.cc', link_with: color_lib)
