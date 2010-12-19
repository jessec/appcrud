<?php

function pQgWhereNE($sql) {
	
	$queries = array();

	foreach($sql as $query) {

		if (PQG_LOG) pQgLog("pQgNotEqual-loop: $query");
		
		$pos = strpos($query, ' != ');	
		while ($pos) {
		
			$before = substr ( $query, 0, $pos  );
			$after  = substr ( $query, $pos + 4 );

			$add1 = $before . ' < ' . $after;
			$add2 = $before . ' > ' . $after;
			
			if (PQG_LOG) pQgLog("pQgNotEqual-add-1: $add1");
			if (PQG_LOG) pQgLog("pQgNotEqual-add-2: $add2");
			
			$queries [] = $add1;
			$query      = $add2;
			
			$pos = strpos($query, ' != ');	
		
		}
	
		$queries [] = $query;

	}
		
	return $queries;
	
}

?>