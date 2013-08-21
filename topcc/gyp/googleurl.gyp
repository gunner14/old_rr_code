# Copyright (c) 2009 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

# TODO(mark): Upstream this file to googleurl.
{
  'variables': {
    'chromium_code': 1,
    'chromeos': 0,
  },
  'conditions': [
    ['OS=="win"', {
      'target_defaults': {
        'defines': ['WIN32'],
        'sources!':['../third_party/googleurl/base/string16.cc',],
      },
    },],
  ],
  'targets': [
    {
      'target_name': 'googleurl',
      'type': '<(component)',
      'msvs_guid': 'EF5E94AB-B646-4E5B-A058-52EF07B8351C',
      'dependencies': [
        #'../../base/base.gyp:base',
        '..//third_party/icu/icu.gyp:icudata',
        '..//third_party/icu/icu.gyp:icui18n',
        '..//third_party/icu/icu.gyp:icuuc',
      ],
      'include_dirs': [
          '../third_party',
          '..',
          '../third_party/googleurl',
          '..//third_party/icu/public/common',
          '..//third_party/icu/public/i18n',
      ],
      'sources': [
        '../third_party/googleurl/base/string16.cc',
        '../third_party/googleurl/src/gurl.cc',
        '../third_party/googleurl/src/gurl.h',
        '../third_party/googleurl/src/url_canon.h',
        '../third_party/googleurl/src/url_canon_etc.cc',
        '../third_party/googleurl/src/url_canon_fileurl.cc',
        '../third_party/googleurl/src/url_canon_host.cc',
        '../third_party/googleurl/src/url_canon_icu.cc',
        '../third_party/googleurl/src/url_canon_icu.h',
        '../third_party/googleurl/src/url_canon_internal.cc',
        '../third_party/googleurl/src/url_canon_internal.h',
        '../third_party/googleurl/src/url_canon_internal_file.h',
        '../third_party/googleurl/src/url_canon_ip.cc',
        '../third_party/googleurl/src/url_canon_ip.h',
        '../third_party/googleurl/src/url_canon_mailtourl.cc',
        '../third_party/googleurl/src/url_canon_path.cc',
        '../third_party/googleurl/src/url_canon_pathurl.cc',
        '../third_party/googleurl/src/url_canon_query.cc',
        '../third_party/googleurl/src/url_canon_relative.cc',
        '../third_party/googleurl/src/url_canon_stdstring.h',
        '../third_party/googleurl/src/url_canon_stdurl.cc',
        '../third_party/googleurl/src/url_file.h',
        '../third_party/googleurl/src/url_parse.cc',
        '../third_party/googleurl/src/url_parse.h',
        '../third_party/googleurl/src/url_parse_file.cc',
        '../third_party/googleurl/src/url_parse_internal.h',
        '../third_party/googleurl/src/url_util.cc',
        '../third_party/googleurl/src/url_util.h',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '../../',
        ],
      },
      'conditions': [
        ['OS=="win" and component=="shared_library"', {
          'defines': [
            'GURL_DLL',
            'GURL_IMPLEMENTATION=1',
          ],
          'direct_dependent_settings': {
            'defines': [
              'GURL_DLL',
            ],
          },
        }],
      ],
    },
    {
      'target_name': 'googleurl_unittests',
      'dependencies': [
        'googleurl',
        '../testing/gtest.gyp:gtest',
        '..//third_party/icu/icu.gyp:icuuc',
      ],
      'include_dirs': [
          '../third_party',
          '..',
          '../third_party/googleurl',
      ],
      'sources': [
        '../third_party/googleurl/src/gurl_unittest.cc',
        '../third_party/googleurl/src/url_canon_unittest.cc',
        '../third_party/googleurl/src/url_parse_unittest.cc',
        '../third_party/googleurl/src/url_test_utils.h',
        '../third_party/googleurl/src/url_util_unittest.cc',
        '../third_party/googleurl/src/gurl_test_main.cc',
      ],
      'conditions': [
        ['OS=="linux" or OS=="freebsd"', {
          'conditions': [
            ['linux_use_tcmalloc==1', {
              'dependencies': [
                '../../base/allocator/allocator.gyp:allocator',
              ],
            }],
          ],
        }],
        # TODO(victorw): The unittest code uses inline functions that access
        # global variables, it also uses internal functions that we may not want
        # to export, so skip building unittests for windows multi dll build.
        # The googleurl functions are tested by the static library build.
        ['OS=="win" and component=="shared_library"', {
          'type': 'none',
        }, {
          'type': 'executable',
        }],
      ],
    },
  ],
}

# Local Variables:
# tab-width:2
# indent-tabs-mode:nil
# End:
# vim: set expandtab tabstop=2 shiftwidth=2:
