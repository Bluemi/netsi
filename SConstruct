#!/usr/bin/python3

import os
import shutil
import hashlib

DRY=False
TESTS=True

MODE = 'debug'
MODE = 'release'

SRC_DIRECTORY = 'src'
BUILD_DIRECTORY = os.path.join('build', MODE)
OBJ_DIRECTORY = os.path.join(BUILD_DIRECTORY, 'obj')
INCLUDE_DIRECTORY = os.path.join(BUILD_DIRECTORY, 'include')
INCLUDE_NETSI_DIRECTORY = os.path.join(INCLUDE_DIRECTORY, 'netsi')
LIB_DIRECTORY = os.path.join(BUILD_DIRECTORY, 'lib')
BIN_DIRECTORY = os.path.join(BUILD_DIRECTORY, 'bin')

TESTS_SRC_DIRECTORY = 'tests'
TESTS_BUILD_DIRECTORY = os.path.join(BUILD_DIRECTORY, 'tests')
TESTS_OBJ_DIRECTORY = os.path.join(TESTS_BUILD_DIRECTORY, 'obj')
TESTS_BIN_DIRECTORY = os.path.join(TESTS_BUILD_DIRECTORY, 'bin')

def md5(fname):
    hash_md5 = hashlib.md5()
    with open(fname, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash_md5.update(chunk)
    return hash_md5.digest()

def file_exists(src_file, dest_file):
    if not os.path.isfile(dest_file):
        return False

    return md5(src_file) == md5(dest_file)


def is_source_file(f):
    return os.path.splitext(f)[1] in ['.hpp', '.cpp', '.fs', '.vs']


def copy_tree(src_directory, destination_directory, dry=False, included=None):
    """
    Copies the given src directory to the destination directory.
    Files that already exists with the same version at the destination directory are ignored.
    """
    for src_root, directories, files in os.walk(src_directory):
        build_root = os.path.join(destination_directory, src_root[len(src_directory)+1:])
        if dry:
            if not os.path.isdir(build_root):
                print('mkdir {}'.format(build_root))
        else:
            if not os.path.isdir(build_root):
                os.makedirs(build_root)

        # create directories
        for d in directories:
            current_src_dir = os.path.join(src_root, d)
            current_dest_dir = os.path.join(build_root, d)

            if dry:
                if not os.path.isdir(current_dest_dir):
                    print('mkdir {}'.format(current_dest_dir))
            else:
                if not os.path.isdir(current_dest_dir):
                    os.makedirs(current_dest_dir)

        # copy files
        for f in files:
            current_src_file = os.path.join(src_root, f)
            current_dest_file = os.path.join(build_root, f)

            if not file_exists(current_src_file, current_dest_file):
                if dry:
                    print('cp {} -> {}'.format(current_src_file, current_dest_file))
                else:
                    if included is None:
                        shutil.copy2(current_src_file, current_dest_file)
                    elif os.path.splitext(current_src_file)[1] in included:
                        shutil.copy2(current_src_file, current_dest_file)


        # remove files
        build_files = [f for f in os.listdir(build_root) if os.path.isfile(os.path.join(build_root, f))]
        src_files = [f for f in os.listdir(src_root) if os.path.isfile(os.path.join(build_root, f))]
        for build_file in build_files:
            if not is_source_file(build_file):
                continue
            if build_file not in src_files:
                if dry:
                    print('rm {}'.format(os.path.join(build_root, build_file)))
                else:
                    os.remove(os.path.join(build_root, build_file))

copy_tree(SRC_DIRECTORY, OBJ_DIRECTORY, dry=DRY)
copy_tree(SRC_DIRECTORY, INCLUDE_NETSI_DIRECTORY, dry=DRY, included=['.hpp'])


def get_source_files(env, home, exclude=[]):
    dirs = []
    for root, _, _ in os.walk(home):
        dirs.append(root)

    source_files = []
    for d in dirs:
        excludes = [os.path.join(d, e) for e in exclude]
        source_files.extend(env.Glob(os.path.join(d, '*.cpp'), exclude=excludes))

    return source_files


env = Environment(parse_flags='-std=c++17 -pthread -I{}'.format('/usr/local/include/boost'))
env['CXXCOMSTR'] =  'compiling   $TARGET'
env['LINKCOMSTR'] = 'linking     $TARGET'
env['ENV']['TERM'] = os.environ['TERM']
env.Append(LIBS=['boost_thread', 'boost_system'])
env.Append(LIBPATH=['/usr/local/lib'])

if MODE == 'debug':
    env.Append(CCFLAGS='-g')
else:
    env.Append(CCFLAGS='-O3')

sources = get_source_files(env, OBJ_DIRECTORY)
sharedLib = env.SharedLibrary(target=os.path.join(LIB_DIRECTORY, 'netsi'), source=sources)
# staticLib = env.StaticLibrary(target=os.path.join(LIB_DIRECTORY, 'netsi'), source=sources)

if TESTS:
    copy_tree(TESTS_SRC_DIRECTORY, TESTS_OBJ_DIRECTORY, dry=DRY)

    test_env = Environment(parse_flags='-std=c++17 -pthread -I{} -I{}'.format(INCLUDE_DIRECTORY, '/usr/local/include/boost'))
    test_env.Append(LIBS=['netsi', 'boost_thread', 'boost_system'])
    test_env.Append(LIBPATH=[LIB_DIRECTORY, '/usr/local/lib'])
    test_env['ENV']['TERM'] = os.environ['TERM']
    if MODE == 'debug':
        test_env.Append(CCFLAGS='-g')
    else:
        test_env.Append(CCFLAGS='-O3')

    source_files = get_source_files(test_env, OBJ_DIRECTORY, exclude=['client.cpp', 'server.cpp'])
    test_source_files = get_source_files(test_env, TESTS_OBJ_DIRECTORY)
    
    for test_file in test_source_files:
        srcs = source_files+[test_file]
        name = os.path.splitext(test_file.name)[0]
        test_env.Program(os.path.join(TESTS_BIN_DIRECTORY, name), srcs)
