project('color', 'cpp', default_options : ['cpp_std=c++11'])
if meson.get_compiler('cpp').get_id() != 'msvc'
	add_global_arguments('-std=c++11', '-fdiagnostics-color', '-Werror', language : 'cpp')
endif

subdir('src')
