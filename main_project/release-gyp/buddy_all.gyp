{
  'targets':[ 
  {
    'target_name' : 'buddy_all',
    'type' : 'none',
    'dependencies' : [
      './buddy/buddy_apply_service.gyp:*',
      './buddy/buddy_by_applicant_service.gyp:*',
      './buddy/buddy_apply_service.gyp:*',
      './buddy/buddy_by_addtime_service.gyp:*',
      './buddy/buddy_by_applicant_service.gyp:*',
      './buddy/buddy_by_id_service.gyp:*',
      './buddy/buddy_by_name_service.gyp:*',
      './buddy/buddy_by_onlinetime_service.gyp:*',
      './buddy/buddy_core_logic_service.gyp:*',
      './buddy/buddy_core_service.gyp:*',
      './buddy/buddy_count_service.gyp:*',
      './buddy/buddy_friends_service.gyp:*',
      './buddy/buddy_group_service.gyp:*',
      './buddy/buddy_relation_service.gyp:*',
      './buddy/follow_relation_service.gyp:*',
#      './buddy/friend_count_cache.gyp:*',
      './buddy/journey_logic.gyp:*',
    ],
  },

  ] #end targets
}
