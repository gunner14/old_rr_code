{
  'targets':[ 
  {
    'target_name' : 'socialgraph_all',
    'type' : 'none',
    'dependencies' : [
      './socialgraph/config_parser.gyp:*',
      './socialgraph/data_parser.gyp:*',
      './socialgraph/recommendation_manager.gyp:*',
      './socialgraph/socialgraphrcdmessageservice.gyp:*',
			'./socialgraph/socialgraph_core_logic.gyp:*',
      './socialgraph/socialgraph_test.gyp:*'
    ],
  },

  ] #end targets
}
