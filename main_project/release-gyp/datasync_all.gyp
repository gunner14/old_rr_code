{
  'targets':[ 
  {
    'target_name' : 'datasync_all',
    'type' : 'none',
    'dependencies' : [
      './datasync/local_producer_manager.gyp:*',
      './datasync/sync_manager.gyp:*',
      './datasync/remote_consumer_manager.gyp:*',
      './datasync/dispatch_manager.gyp:*',
    ],
  },

  ] #end targets
}
