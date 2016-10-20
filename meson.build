project('colorlog', 'cpp', default_options : ['cpp_std=c++11'])
if meson.get_compiler('cpp').get_id() != 'msvc'
    add_global_arguments('-std=c++11', language: 'cpp')
endif
if meson.get_compiler('cpp').get_id() == 'clang'
    add_global_arguments('-fdiagnostics-color', language: 'cpp')
endif

boost_dep = dependency('boost', modules : ['regex'], required: true)

subdir('src')
if get_option('build_tests')
    subdir('tests')
endif

install_man('data/colorlog.1')
install_data('data/colorlog.conf', install_dir: '/etc')
install_data('data/_colorlog', install_dir: get_option('datadir') + '/zsh/functions/Completion/Zsh')
