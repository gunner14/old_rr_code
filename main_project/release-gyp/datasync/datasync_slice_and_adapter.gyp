{
  #包含公共设置
  'includes':[
    'datasync_common.gyp',
  ],

  'targets' : [

  ######################################################
  {
    'target_name' : 'datasync_slice_and_adapter',
    'type' : 'static_library',
    'sources' : [

      '<(oce_slice_path)/DataSync.cpp',     
      '<(oce_slice_path)/Monitor.cpp',

      '<(oce_proto_path)/DataSync.pb.cc',

      '<(adapter_path)/MonitorAdapter.cpp',
  
      '<!@(find <(main_project_path)/message_pipe/src/cpp -name "*.cpp")',
    ],
  }, #end datasync_slice_and_adapter 
  ] #end targets
}

