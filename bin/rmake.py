#!/usr/local/bin/python3

import argparse
import os
import subprocess

if __name__ == '__main__':
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter,
                                    description = 'Recursively run make targets')
    parser.add_argument('dir',
                        type = str,
                        help = 'directory to start with'
                       )
    parser.add_argument('target',
                        type = str,
                        default = '',
                        help = 'the make target to run recursively'
                       )
    parser.add_argument('--jobs', '-j',
                        type = int,
                        default = 1,
                        help = 'make -j'
                       )
    args = parser.parse_args()

    cwd = os.getcwd()
    root_dir = os.path.realpath(args.dir)

    if not os.path.exists(root_dir):
        raise RuntimeError(root_dir + ' does not exist!')
    if not os.path.isdir(root_dir):
        raise RuntimeError(root_dir + ' is not a directory!')

    for root, dirs, files in os.walk(root_dir):
        for dir in dirs:
            dirname = os.path.join(root, dir)
            os.chdir(dirname)
            makefile = os.path.join(dirname, 'Makefile')
            if os.path.exists(makefile) and os.path.isfile(makefile):
                cmdlist = [ 'make' ]
                if args.target:
                    cmdlist.append(args.target)

                if args.jobs > 1:
                    cmdlist.append('-j' + str(args.jobs))
                subprocess.call(cmdlist)

    os.chdir(cwd)
