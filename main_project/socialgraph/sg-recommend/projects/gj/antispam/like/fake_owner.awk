BEGIN{
	bound = 10;
}
{
	cnt[$2] += $3;
}
END{
	for( i in cnt ){
		if( cnt[i] >= bound) {
		print i"\t"cnt[i]
		}
	}
}
