<?php

function pQgWhereBetween () {

	global  $pQgParse;
	
	$hit = FALSE;
	
	foreach( $pQgParse  as $key => $value ) {
  
		if ($value == 'between') {

			$field = $pQgParse  [$key-1] ;
			$low   = $pQgParse  [$key+1] ;
			$and   = $pQgParse  [$key+2] ;
			$high  = $pQgParse  [$key+3] ;
			
			$new = "$field >= $low && $field <= $high";

			if (PQG_LOG) pQgLog("pQgBetween: $add");
			
			$pQgParse  [$key-1] = $new;
			
			unset ( $pQgParse  [$key]   ) ;
			unset ( $pQgParse  [$key+1] ) ;
			unset ( $pQgParse  [$key+2] ) ;
			unset ( $pQgParse  [$key+3] ) ;
						
			$hit = TRUE;
			
		}
		
	}

	if ($hit)
		pQgReParse();
		
}

?>