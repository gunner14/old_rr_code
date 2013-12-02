{
  #包含公共设置
  'includes':[
    'global_vars.gyp',
  ],

  'targets' : [

  ######################################################
  {
    'target_name' : 'oce_proto',
    'type' : 'none',
    'direct_dependent_settings' : {
      'include_dirs':[
        '/usr/local/distcache-dev/include',
      ],
    },
    'sources' : [
      '<!@(find <(oce_proto_path) -name "*.proto")',
    ],
    'rules' : [
    {
     'rule_name':'genproto',
     'extension':'proto',
     'inputs': [],
     'variables': {
        # The protoc compiler requires a proto_path argument with the
        # directory containing the .proto file.
        # There's no generator variable that corresponds to this, so fake it.
       'rule_input_relpath':'<(main_project_path)/OceProto/proto',
      },
     'outputs': [
       '<(rule_input_relpath)/<(RULE_INPUT_ROOT).pb.h',
       '<(rule_input_relpath)/<(RULE_INPUT_ROOT).pb.cc',
      ],
     'action': [
       '<(protoc_path)',
       '--proto_path=/usr/local/distcache-dev/include',
       '--proto_path=<(rule_input_relpath)',
       '<(rule_input_relpath)/<(RULE_INPUT_NAME)',
       '--cpp_out=<(rule_input_relpath)',
      ],
     'message':'Generating C++ code from <(RULE_INPUT_PATH)',
    },
    ],
  }, 

  ] #end targets
}

