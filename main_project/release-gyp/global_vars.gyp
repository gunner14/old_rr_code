{
  'variables': {
    'protoc_path':'/usr/local/distcache-dev/bin/protoc',
    'ice_path':'<!(echo $ICE_HOME)',
    'slice2pp_path':'<(ice_path)/bin/slice2cpp',
    'boost_path':'/usr/local/xce',

    ###### 重要： main_project_path为项目主文件夹，相对于本文件的路径 项目中的其他路径都用该路径定义 ######

    'main_project_path':'..',

    
    'oce_slice_path':'<(main_project_path)/OceSlice/slice',
    'oce_proto_path':'<(main_project_path)/OceProto/proto',
		'socialgraph_proto_path':'<(main_project_path)/socialgraph/socialgraphproto',
    'util_slice_path':'<(main_project_path)/UtilSlice/slice',
    'adapter_path':'<(main_project_path)/OceCxxAdapter/src',
    'target_mode':'shared_library',
    #'target_mode':'executable',
   },
}
