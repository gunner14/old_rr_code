BEGIN{
	bound = 5;
}
{
    if( $3 >= bound ) {
	   innode[$2] += $3 ;
	   count[$2] ++ ;
       outnode[$1] ++ ;
	}
}
END{
	for( i in innode ) {
		#if( outnode[$1] == 0 ) 
		print i"\t"count[i]"\t"innode[i]
	}
}
