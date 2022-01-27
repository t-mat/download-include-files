#!/usr/bin/env python3
import argparse
import re
import urllib.request
import glob
import os

program_name = 'download-include-files.py'
description_text = 'Download #include files in C/C++ code'
default_include_dir = 'third_party/include'

usage_text = f'''
Special comment in C/C++ code
=============================
{program_name} recognizes the following #include and special comment pair.

    #include "(FILENAME)" // --download-from--:(URL)
    #include "(FILENAME)" /* --download-from--:(URL) */

These comments indicate to download (URL) as (FILENAME).

The following example downloads xxhash.h from github.

    #include "xxhash.h" // --download-from--:https://github.com/Cyan4973/xxHash/blob/dev/xxhash.h

And the local file name of it is

    {default_include_dir}/xxhash.h

You can change {default_include_dir} by --include (see below).

Special #include "github.com/..." in C/C++ code
===============================================
{program_name} recognizes the following special form of #include.

    #include "github.com/(FILENAME)"

This #include line indicates to download (FILENAME) from GitHub.

For example, the following #include downloads stb_image.h from github.

    #include "github.com/nothings/stb/blob/master/stb_image.h"

And the local file name of it is

    {default_include_dir}/github.com/nothings/stb/blob/master/stb_image.h

You can change {default_include_dir} by --include (see below).

--include (or -I)
=================
The root directory of (FILENAME) can be specified by

    --include (DIR)

when you specify --include, actual downloaded file path becomes

    (DIR)/(FILENAME)

Default (DIR) is "{default_include_dir}".

The following example download files under external/include/

    {program_name} *.c *.h *.cpp *.hpp --include external/include

And above examples are downloaded as

    external/include/xxhash.h
    external/include/github.com/nothings/stb/blob/master/stb_image.h

Download only form
==================
When you just want to download a file but avoid to be compiled, you can
put them within comment line.

    // #include "github.com/..."

Glob
====
Since this program uses standard Python glob module,
you can use usual glob notations.  (Even on Windows Command Prompt)
'''

includeRegex = re.compile(r'#[ \t]*include[ \t]+["<]([^">]+)[">]')
downloadFromUrlRegex = re.compile(r'--download-from--:([^ \t\r\n]*)')
githubIncludeRegex = re.compile(r'#[ \t]*include[ \t]+["<](github.com/[^">]+)[">]')
rawGithubIncludeRegex = re.compile(r'#[ \t]*include[ \t]+["<](raw.githubusercontent.com/[^">]+)[">]')


def get_download_filenames_from_file(filename, include_root_dir):
    filepath_url_pairs = {}   # filepath_url_pairs[filepath] = URL

    with open(filename, 'r', encoding='utf8') as f:
        lines = f.readlines()

    line_number = 0
    for line in lines:
        line_number += 1

        if '--download-from--:' in line:
            # --download-from--:(URL)
            m = re.search(downloadFromUrlRegex, line)
            if not m:
                print(f'{filename}({line_number}): Error.  Please specify URL')
                continue
            url = m.groups()[0]
            url = translate_special_url(url)

            # #include "(filename)"
            m = re.search(includeRegex, line)
            if not m:
                print(f'{filename}({line_number}): Error.  Failed to find #include')
                continue
            filename = m.groups()[0]

            # canonical filename
            filename = os.path.join(include_root_dir, filename)
            filename = os.path.realpath(filename)
            filepath_url_pairs[filename] = url

        # #include "github.com/..."
        m = re.search(githubIncludeRegex, line)
        if m:
            filename = m.groups()[0]
            url = make_github_url_from_filename(filename)
            output = os.path.join(include_root_dir, filename)
            output = os.path.realpath(output)
            filepath_url_pairs[output] = url

        # #include "raw.githubusercontent.com/..."
        m = re.search(rawGithubIncludeRegex, line)
        if m:
            filename = m.groups()[0]
            url = make_rawgithub_url_from_filename(filename)
            output = os.path.join(include_root_dir, filename)
            output = os.path.realpath(output)
            filepath_url_pairs[output] = url

    return filepath_url_pairs


# #include "github.com/..."
def make_github_url_from_filename(filename):
    url = 'https://' + filename
    url = translate_special_url(url)
    return url


# #include "raw.githubusercontent.com/..."
def make_rawgithub_url_from_filename(filename):
    url = 'https://' + filename
    return url


# 
def translate_special_url(url):
    # github.com/... -> https://github.com/...
    if url.startswith('github.com/'):
        url = url.replace('github.com/', 'https://github.com/')

    # http://github.com/... -> https://github.com/...
    if url.startswith('http://github.com/'):
        url = url.replace('http://github.com/', 'https://github.com/')

    # https://github.com/user/repo/blob/branch/file -> https://raw.githubusercontent.com/user/repo/branch/file
    if url.startswith('https://github.com/'):
        url = url.replace('https://github.com/', 'https://raw.githubusercontent.com/')
        url = url.replace('/blob/', '/')
    return url


def download(filepath, url, dry_run):
    print(f'Downloading {url}\n  as {filepath} ... ', end='')

    # makedir if directory doesn't exist
    dir_part = os.path.dirname(filepath)
    if not os.path.exists(dir_part):
        os.makedirs(dir_part, exist_ok=True)

    # Download a file from URL.  Save it as filepath.
    error = False
    if dry_run:
        print(f'-- DRY-RUN -- ... ', end='')
    else:
        if os.path.exists(filepath):
            print(f'already exists ... ', end='')
        else:
            try:
                urllib.request.urlretrieve(url, filepath)
            except urllib.error.HTTPError as e:
                print(f'{e} (url={url}) ... ', end='')
                error = True
    if error:
        print(f'Error')
    else:
        print(f'Done')


def main():
    parser = argparse.ArgumentParser(description=description_text,
                                     epilog=usage_text,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('filenames', nargs='+')
    parser.add_argument('--include', '-I', help='root directory of #include "..."', default=default_include_dir)
    parser.add_argument('--dry-run', dest='dry_run', action='store_true', help='run without downloading')
    parser.set_defaults(dry_run=False)
    args = parser.parse_args()

    dry_run = args.dry_run
    include_dir = args.include

    # Expand all glob patterns
    source_files = []
    for pattern in args.filenames:
        source_files += glob.glob(pattern)

    # Read all { include_filename, URL } pair
    filepath_url_pairs = {}   # filepath_url_pairs[filepath] = URL
    for filename in source_files:
        if not os.path.exists(include_dir):
            os.makedirs(include_dir, exist_ok=True)
        fup = get_download_filenames_from_file(filename, include_dir)
        filepath_url_pairs = {**filepath_url_pairs, **fup}  # we will be able to use '|' operator in future.

    # Download all { include_filename, URL } pair
    for filepath, url in filepath_url_pairs.items():
        download(filepath, url, dry_run)


if __name__ == '__main__':
    main()
