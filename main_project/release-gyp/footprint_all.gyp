{
  'targets':[ 
  {
    'target_name' : 'footprint_all',
    'type' : 'none',
    'dependencies' : [
      './footprint/footprint_unite.gyp:*',
      './footprint/footprint_new.gyp:*',
      './footprint/footprint_ugc.gyp:*',
    ],
  },

  ] #end targets
}
