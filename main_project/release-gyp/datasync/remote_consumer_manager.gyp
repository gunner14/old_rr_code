########################################################
##  需要在其他文件中加入的内容：
##
##  一、在buddy_slice_and_adapter中加入以下源文件：
##    1. ICE 或 PROTO 生成的cpp或cc文件 
##    2. 用到的adapter的源文件
##
##  二、在buddy_common.gyp中加入include路径和link的参数
##
########################################################

{
  # 包含公共设置
  'includes':[
    'datasync_common.gyp',
  ],
  'variables':{
    'service_name'            : 'RemoteConsumerManager',

    'service_src_path'              : '<(main_project_path)/DataSync-Service/remoteconsumermanager',

  },

  # 编译目标
  'targets' : [

  ######################################################
  {
    'target_name' : '<(service_name)',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path) -name "*.cpp")',
    ],
    'dependencies' : [
      './datasync_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################

  ] #end targets
}

