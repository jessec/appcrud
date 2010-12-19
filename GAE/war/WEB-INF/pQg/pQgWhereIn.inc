<?php

function pQgWhereIn () {

	global  $pQgParse;
	
	$hit = FALSE;
	
	foreach( $pQgParse  as $key => $value ) {
  
		if ($value == 'in') {

			$hit = TRUE;
			
			$field = $pQgParse  [$key-1] ;
			
			$val = array();
			$len = count($pQgParse );
			
			for ($i=$key+2; $key<$len; $i++) {

				$pos = $pQgParse  [$i];
				
				if ( $pos <> ')' and $pos <> ',' )
					$val[] = $pos;
				
				unset ($pQgParse  [$i]) ;
				
				if ( $pos == ')' )
				
					break;
			}

			$in = "$field = " . implode ( " or $field == ", $val);

			if (PQG_LOG) pQgLog("pQgIn: $in");
			
			$pQgParse  [$key] = $in;
			
			unset ( $pQgParse  [$key-1] ) ;
			unset ( $pQgParse  [$key+1] ) ;
			
		}
		
	}

	if ($hit)
		pQgReParse();
		
}
	
?>