project(
  'lumenex',
  'c',
  version: '0.1.0',
  meson_version: '>=1.3.0',
  default_options: ['c_std=c11', 'warning_level=2']
)

cc = meson.get_compiler('c')

incdir = include_directories('include')

pkg = import('pkgconfig')

sources = [
  'src/language.c',
  'src/lexer.c',
  'src/token.c',
  'src/languages/c/lexer.c',
  'src/languages/cpp/lexer.c',
  'src/languages/rust/lexer.c',
  'src/languages/python/lexer.c',
  'src/languages/javascript/lexer.c',
  'src/languages/ada/lexer.c',
  'src/languages/go/lexer.c',
  'src/languages/java/lexer.c',
  'src/languages/typescript/lexer.c',
  'src/languages/cs/lexer.c',
  'src/languages/zig/lexer.c',
]

lumenex_lib = both_libraries(
  'lumenex',
  sources,
  include_directories: incdir,
  install: true,
  version: meson.project_version(),
  soversion: '0',
  c_args: ['-DLUMENEX_BUILDING']
)

install_headers(
  'include/lumenex/export.h',
  'include/lumenex/version.h',
  'include/lumenex/language.h',
  'include/lumenex/token.h',
  'include/lumenex/lexer.h',
  subdir: 'lumenex'
)

pkg.generate(
  lumenex_lib.get_shared_lib(),
  name: 'lumenex',
  filebase: 'lumenex',
  description: 'A production-quality C lexer library',
  subdirs: 'lumenex'
)

if get_option('tests')
  subdir('tests')
endif
