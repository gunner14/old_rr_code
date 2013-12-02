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
    'buddy_common.gyp',
  ],
  'variables':{
    'service_name'            : 'BuddyApply',

    'cache_name'              : '<(service_name)Cache',
    'loader_name'             : '<(service_name)Loader',
    'preloader_name'          : '<(service_name)Preloader',

    'cache_src_path'          : '<(main_project_path)/buddy/<(cache_name)/cache',
    'loader_src_path'         : '<(main_project_path)/buddy/<(cache_name)/loader',
    'preloader_src_path'      : '<(main_project_path)/buddy/<(cache_name)/preloader',
  },

  # 编译目标
  'targets' : [

  ######################################################
  {
    'target_name' : '<(cache_name)',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(cache_src_path) -name "*.cpp")',
    ],
    'dependencies' : [
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : '<(loader_name)',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(loader_src_path) -name "*.cpp")',
    ],
    'dependencies' : [
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, 
  ######################################################
  {
    'target_name' : '<(preloader_name)',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(preloader_src_path) -name "*.cpp")',
    ],
    'dependencies' : [
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, 

  ] #end targets
}

