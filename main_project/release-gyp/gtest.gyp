# Copyright (c) 2009 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'includes':[
    'global_vars.gyp',
  ],
  'targets': [
    {
      'target_name': 'gtest',
      'type': '<(library)',
      'msvs_guid': 'BFE8E2A7-3B3B-43B0-A994-3058B852DB8B',
      'sources': [
        '<(main_project_path)/third-party/gtest/include/gtest/gtest-death-test.h',
        '<(main_project_path)/third-party/gtest/include/gtest/gtest-message.h',
        '<(main_project_path)/third-party/gtest/include/gtest/gtest-param-test.h',
        '<(main_project_path)/third-party/gtest/include/gtest/gtest-printers.h',
        '<(main_project_path)/third-party/gtest/include/gtest/gtest-spi.h',
        '<(main_project_path)/third-party/gtest/include/gtest/gtest-test-part.h',
        '<(main_project_path)/third-party/gtest/include/gtest/gtest-typed-test.h',
        '<(main_project_path)/third-party/gtest/include/gtest/gtest.h',
        '<(main_project_path)/third-party/gtest/include/gtest/gtest_pred_impl.h',
        '<(main_project_path)/third-party/gtest/include/gtest/gtest_prod.h',
        '<(main_project_path)/third-party/gtest/include/gtest/internal/gtest-death-test-internal.h',
        '<(main_project_path)/third-party/gtest/include/gtest/internal/gtest-filepath.h',
        '<(main_project_path)/third-party/gtest/include/gtest/internal/gtest-internal.h',
        '<(main_project_path)/third-party/gtest/include/gtest/internal/gtest-linked_ptr.h',
        '<(main_project_path)/third-party/gtest/include/gtest/internal/gtest-param-util-generated.h',
        '<(main_project_path)/third-party/gtest/include/gtest/internal/gtest-param-util.h',
        '<(main_project_path)/third-party/gtest/include/gtest/internal/gtest-port.h',
        '<(main_project_path)/third-party/gtest/include/gtest/internal/gtest-string.h',
        '<(main_project_path)/third-party/gtest/include/gtest/internal/gtest-tuple.h',
        '<(main_project_path)/third-party/gtest/include/gtest/internal/gtest-type-util.h',
        '<(main_project_path)/third-party/gtest/src/gtest-all.cc',
        '<(main_project_path)/third-party/gtest/src/gtest-death-test.cc',
        '<(main_project_path)/third-party/gtest/src/gtest-filepath.cc',
        '<(main_project_path)/third-party/gtest/src/gtest-internal-inl.h',
        '<(main_project_path)/third-party/gtest/src/gtest-port.cc',
        '<(main_project_path)/third-party/gtest/src/gtest-printers.cc',
        '<(main_project_path)/third-party/gtest/src/gtest-test-part.cc',
        '<(main_project_path)/third-party/gtest/src/gtest-typed-test.cc',
        '<(main_project_path)/third-party/gtest/src/gtest.cc',
      ],
      'sources!': [
        '<(main_project_path)/third-party/gtest/src/gtest-all.cc',  # Not needed by our build.
      ],
      'include_dirs': [
        '<(main_project_path)/third-party/gtest',
        '<(main_project_path)/third-party/gtest/include',
      ],
      'conditions': [
        ['OS == "mac" or OS == "linux" or OS == "freebsd" or OS == "openbsd"', {
          'defines': [
            # gtest isn't able to figure out when RTTI is disabled for gcc
            # versions older than 4.3.2, and assumes it's enabled.  Our Mac
            # and Linux builds disable RTTI, and cannot guarantee that the
            # compiler will be 4.3.2. or newer.  The Mac, for example, uses
            # 4.2.1 as that is the latest available on that platform.  gtest
            # must be instructed that RTTI is disabled here, and for any
            # direct dependents that might include gtest headers.
            'GTEST_HAS_RTTI=0',
          ],
          'direct_dependent_settings': {
            'defines': [
              'GTEST_HAS_RTTI=0',
            ],
          },
        }],
      ],
      'direct_dependent_settings': {
        'defines': [
          'UNIT_TEST',
        ],
        'include_dirs': [
          '<(main_project_path)/third-party/gtest/include',  # So that gtest headers can find themselves.
        ],
        'target_conditions': [
          ['_type=="executable"', {
            'test': 1,
            'conditions': [
              ['OS=="mac"', {
                'run_as': {
                  'action????': ['${BUILT_PRODUCTS_DIR}/${PRODUCT_NAME}'],
                },
              }],
              ['OS=="win"', {
                'run_as': {
                  'action????': ['$(TargetPath)', '--gtest_print_time'],
                },
              }],
            ],
          }],
        ],
        'msvs_disabled_warnings': [4800],
      },
    },
    {
      'target_name': 'gtest_main',
      'type': '<(library)',
      'dependencies': [
        'gtest',
      ],
      'direct_dependent_settings': {
        'target_conditions': [
          ['_type=="executable"', {
            'conditions':[
              ['OS=="linux"',{
                'libraries': ['-lpthread',], 
              }],
            ],
          }],
        ],
      },
      'sources': [
        '<(main_project_path)/third-party/gtest/src/gtest_main.cc',
      ],
    },
  ],
}

# Local Variables:
# tab-width:2
# indent-tabs-mode:nil
# End:
# vim: set expandtab tabstop=2 shiftwidth=2:
