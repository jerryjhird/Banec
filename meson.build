# basic necessities (banec)
project('banec', 'c', version : '2.0.0',
    default_options : ['c_std=c99', 'warning_level=3'])

add_project_arguments(
  '-D_POSIX_C_SOURCE=200809L',
  '-D_XOPEN_SOURCE=700',
  language: 'c'
)

inc = include_directories('include')

common_cflags = [
  '-Wall',
  '-Wextra',
  '-O3',
  '-march=native',
  '-flto'
]

link_args = ['-nostdlib', '-static']

# check DEBUG option to dictate weather to use rpath($ORIGIN/../lib)
debug_mode = get_option('debug_mode')

if debug_mode
    message('Debug Build : RPATH=enabled')
else
    message('Release Build : RPATH=disabled')
endif

# all to be compiled binaries that will end up in fs/bin and are also located in src/bin
utils = {
    'src/coreutils/ls.c': 'ls',
    'src/coreutils/pwd.c': 'pwd',
    'src/coreutils/echo.c': 'echo',
    'src/coreutils/mkdir.c': 'mkdir',
    'src/coreutils/testutil.c': 'testutil',  # "test" is reserved by meson so make will rename "testutil" binary to "test"
    'src/coreutils/cat.c': 'cat',
    'src/coreutils/clear.c': 'clear',
    'src/coreutils/true.c': 'true',
    'src/coreutils/false.c': 'false',
    'src/coreutils/uname.c': 'uname',
    'src/coreutils/kill.c': 'kill',
    'src/coreutils/pkill.c': 'pkill',
    'src/coreutils/whoami.c': 'whoami',
}

blibc_src = [
    'src/blibc/special/start.S', # stage 1 entry point
    'src/blibc/special/crt0.c', # stage 2 entry point
    'src/blibc/unistd.c',
    'src/blibc/syscall.c',
    'src/blibc/errno.c',
    'src/blibc/string.c',
    'src/blibc/stat.c',
    'src/blibc/dirent.c',
    'src/blibc/stdlib/malloc.c',
    'src/blibc/stdlib/qsort.c',
    'src/blibc/stdlib/atoi.c',
    'src/blibc/stdlib/printf.c',
    'src/blibc/stdlib/fileio.c',
    'src/blibc/time.c'
]

blibc_lib = static_library(
    'blibc',
    blibc_src,
    include_directories: inc,
    c_args: common_cflags,
    name_prefix : '',
    install: true,
    install_dir: 'lib'
)

foreach src_path, bin_name : utils
    if debug_mode
        executable(bin_name, src_path,
            include_directories: inc,
            c_args: common_cflags,
            link_with: blibc_lib,
            link_args: link_args,
            install: true,
            install_dir: 'bin',
            install_rpath: '$ORIGIN/../lib'
        )
    else
        executable(bin_name, src_path,
            include_directories: inc,
            c_args: common_cflags,
            link_with: blibc_lib,
            link_args: link_args,
            install: true,
            install_dir: 'bin'
        )
    endif
endforeach