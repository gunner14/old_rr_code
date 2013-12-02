{
  'targets':[ 
  {
    'target_name' : 'user_all',
    'type' : 'none',
    'dependencies' : [
      './user/complete_user_info.gyp:*',
      './user/id_cache.gyp:*',
      './user/personal_info.gyp:*',
      './user/user_ads_cache.gyp:*',
      './user/user_api_cache.gyp:*',
      './user/user_api_count.gyp:*',
      './user/user_base.gyp:*',
      './user/user_cache.gyp:*',
      './user/user_config.gyp:*',
      './user/user_count.gyp:*',
      './user/user_danger.gyp:*',
      './user/user_desc.gyp:*',
      './user/user_logic.gyp:*',
      './user/user_network.gyp:*',
      './user/vip_info.gyp:*',
      './user/user_score.gyp:*',
      './user/account_filter.gyp:*',
      './user/account_connect.gyp:*',
      './user/ticket.gyp:*',
    ],
  },

  ] #end targets
}
