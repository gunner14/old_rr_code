BEGIN{
  FS=","
}
{
  cnt[$1"\t"$3] ++ ;
}END{
	for( i in cnt ) 
		print i"\t"cnt[i]
}

