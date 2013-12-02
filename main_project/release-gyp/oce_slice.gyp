{
  #包含公共设置
  'includes':[
    'global_vars.gyp',
  ],

  'targets' : [

  ######################################################
  {
    'target_name' : 'oce_slice',
    'type' : 'none',
    #'direct_dependent_settings' : {
    #  'include_dirs':[
    #    '<(ice_path)/include',
    #  ],
    #},
    'sources' : [
      '<!@(find <(oce_slice_path) -name "*.ice")',
    ],
    'rules' : [
      {
        'rule_name' : 'genslice', 
        'extension':'ice',
        'inputs': [],
        'variables': {
          'rule_input_relpath':'<(oce_slice_path)',
         },
        'outputs': [
          '<(rule_input_relpath)/<(RULE_INPUT_ROOT).h',
          '<(rule_input_relpath)/<(RULE_INPUT_ROOT).cpp',
         ],
        'action': [
          '<(slice2pp_path)',
          '-I<(ice_path)/slice',
          '-I<(main_project_path)/UtilSlice/slice',
          '-I<(main_project_path)/OceSlice/slice',
          '<(RULE_INPUT_PATH)',
          '--output-dir=<(rule_input_relpath)',
         ],
        'message':'Generating C++ code from <(RULE_INPUT_PATH)',
      },
    ],
  }, 

  ] #end targets
}

