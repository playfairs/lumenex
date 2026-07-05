if get_option('tests')
  test_exe = executable(
    'lumenex_language_test',
    'language_test.c',
    include_directories: include_directories('../include'),
    link_with: lumenex_lib,
  )
  test('language-dispatch', test_exe)
endif
