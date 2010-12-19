<?php

function pQgWhereOR($sql) {
	
	$queries = array();

	foreach($sql as $query) {

		if (PQG_LOG) pQgLog("pQgNotOR-loop: $query");
		
		$where = explode(' WHERE ', $query);
		if (PQG_CHECK and (! count($where) or count($where) > 2) )
			pQgError("pQgOR-Internal error: " . count($where));
			
		$or = explode(' || ', $where[1]); 	
		
		$always = '';
		foreach ($or as $key =>$or_part) {
			$and = explode(' && ', $or_part, 2); 	
			if ( count($and) > 1 ) {
				$or [$key] = str_replace(' && ' . $and[1], '', $or_part);
				$always .= ' && ' . $and[1] ;
			}
		}
		
		if ( count ($or) == 1) 

			$queries [] = $query;

		else {
			
			foreach ($or as $or_part) {
			$add = $where [0] . " WHERE " . $or_part . $always;
				if (PQG_LOG) pQgLog("pQgNotOR-add: $add");
				$queries [] = $add;
			}
			
		}

	}
		
	return $queries;
	
}

?>