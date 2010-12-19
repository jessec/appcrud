<?php

error_reporting(E_ALL); 
//error_reporting(E_ALL ^ E_NOTICE);

import pQg.*;

if ( ! isset($pQgLocation) ) {
	$error = '"$pQgLocation" is not set, set it to the location where pQg is installed.';
	echo $error;
	logSevere($error);
	exit;
}

defined('PQG_CHECK') or define('PQG_CHECK', TRUE  );
defined('PQG_LOG'  ) or define('PQG_LOG',   TRUE  );
defined('PQG_JDO'  ) or define('PQG_JDO',   TRUE  );

if (PQG_CHECK) include "$pQgLocation/pQgCheck.inc";
if (PQG_LOG)   include "$pQgLocation/pQgLog.inc";

$pQgInternal  = array('Command','Class','Table','TableFields','Strings','StringsKey','Close','Parts','Parse','Words','Keys','OnlyKey','Alias');
$pQgDouble    = array('group' => 'by' , 'order' => 'by' , 'insert' => 'into' , 'delete' => 'from' , 'group' => 'by', 'not' => 'exists');
$pQgClauses   = array('insert into','select','update','delete from','truncate','from','values','set','where','order by','group by','having','limit');
$pQgOptions   = array('as','between','in','like','exist','not','and','or');
$pQgCombined  = array_merge($pQgClauses, $pQgOptions);

function sql($sql) {
	
	global $pQgLocation, $pQgCommand, $pQgTable;
	
	if (PQG_CHECK)	set_error_handler     ("pQgErrorHandler");
	if (PQG_CHECK)	set_exception_handler ("pQgExceptionHandler");
	if (PQG_LOG) 	pQgLog("pQg-Start: " . pQgLogLayout($sql));
	
	pQgParse($sql);
	
	pQgOpen();

		switch ($pQgCommand) {
    		case "insert into":
				include_once "$pQgLocation/pQgInsert.inc";
				include_once "$pQgLocation/pQgInsertUpdate.inc";
				$return = pQgInsert ();
    			break;
    		case "select":
				include_once "$pQgLocation/pQgSelect.inc";
				include_once "$pQgLocation/pQgQuery.inc";
				$return = pQgSelect ();
    			break;
    		case "update":
				include_once "$pQgLocation/pQgUpdate.inc";
				include_once "$pQgLocation/pQgQuery.inc";
				include_once "$pQgLocation/pQgInsertUpdate.inc";
				$return = pQgUpdate ();
    			break;
    		case "delete from":
				include_once "$pQgLocation/pQgDelete.inc";
				include_once "$pQgLocation/pQgQuery.inc";
				$return = pQgDelete ();
    			break;	
    		case "truncate":
				include_once "$pQgLocation/pQgTruncate.inc";
				$return = pQgTruncate ();
    			break;	
    		default:
				if (PQG_CHECK)                     
					pQgError("Invallid SQL statement: $pQgCommand");
    			return $item;  			
		}	
			
	if (PQG_LOG) 	pQgLog("pQg-End: |$pQgTable|$return");
	if (PQG_CHECK)	restore_error_handler();
	if (PQG_CHECK)	restore_exception_handler();

	return $return;

}

function sqlExists($sql) {

	global $pQgOnlyKey;
	
	$pQgOnlyKey = TRUE;
	
	$array = sql("$sql limit 0,1");
	
	$pQgOnlyKey = FALSE;
	
	if (count($array))
		return TRUE;
	else 
		return FALSE;

}

function sqlCount($sql) {

	global $pQgOnlyKey;
	
	$pQgOnlyKey = TRUE;
	
	$array = sql($sql);

	$pQgOnlyKey = FALSE;
			
	return count($array);

}

function sqlField($sql) {
	
	$array = sql("$sql limit 0,1");

	foreach ($array as $record)
		foreach($record as $field)
			return $field;

	return FALSE;

}

function sqlRecord($sql) {
	
	$array = sql("$sql limit 0,1");

	foreach ($array as $record)
		return $record;

	return $array;

}

function sqlRecords($sql) {
	
	return sql($sql);

}

function pQgOpen() {

	if ( isset($GLOBALS['pQgPMF']) )
		return;
		
	if ( PQG_JDO) $GLOBALS['pQgPMF'] = database::get()->getPersistenceManager();
	if (!PQG_JDO) $GLOBALS['pQgPMF'] = 'fake';

	if (PQG_LOG) pQgLog("pQgOpen:");
	
}

function pQgClose() {

	if ( !isset($GLOBALS['pQgPMF']) )
		return;
		
	if (PQG_JDO) $GLOBALS['pQgPMF']->close();

	unset($GLOBALS['pQgPMF']);
	
	if (PQG_LOG) pQgLog("pQgClose:");
	
}

function pQgParse($sql='') {

	global $pQgReparse;

	pQgParseSplit($sql);
	pQgParseSQL();
	pQgParseTable();
	
	if (PQG_CHECK) pQgCheckSQL();
			
	if ($pQgReparse)
		pQgReparse();
	
}

function pQgReParse() {

	global $pQgReparse;

	$pQgReparse = TRUE;
	
	while ($pQgReparse) {

		$pQgReparse = FALSE;
		
		pQgParseSplit();
		pQgParseSQL();
		
	}
	
}

function pQgParseSplit($sql='') {

	global  $pQgParse;

	if (!$sql)
		$sql = implode(' ', $pQgParse  );
			
	$pQgParse = array();

	if (PQG_LOG) pQgLog("pQgParseSplit-start: $sql");
	
	$sql = str_replace("\\\\", '###ESCAPED-ESCAPE###',       $sql);
	$sql = str_replace("\\'",  '###ESCAPED-SINGLE-QUOTE###', $sql);
	$sql = str_replace("\\\"", '###ESCAPED-DOUBLE-QUOTE###', $sql);
	$sql = str_replace("\\",   '###ESCAPE###',               $sql);
	
	$parts = explode("'", $sql);

	if (PQG_CHECK and  ! (count($parts) % 2) ) 
		pQgError("Not all strings are closed: $string");
	
	foreach($parts as $key => $value)
		if ($key % 2)
			$pQgParse  [] = pQgParseString($value);
		else
			$pQgParse  = array_merge($pQgParse , pQgParseNonString($value));

	$sql = implode(' ', $pQgParse  );

	if (PQG_LOG) pQgParseLog("pQgParseSplit-end: $sql");
	
	if (PQG_CHECK) {
		
		if ( strstr($sql, '###ESCAPED-ESCAPE###') )
			pQgError("An escaped escape char (\\\\) is only allowed inside a string: $sql");
	
		if ( strstr($sql, '###ESCAPED-SINGLE-QUOTE###') )
			pQgError("An escaped single-quote (\\') is only allowed inside a string: $sql");

		if ( strstr($sql, '###ESCAPED-DOUBLE-QUOTE###') )
			pQgError("An escaped double-quote (\\\") is only allowed inside a string: $sql");

		if ( strstr($sql, '###ESCAPE###') )
			pQgError("An escape char (\\) is only allowed inside a string: $sql");
		
		if ( strstr($sql, '"') )
			pQgError("A double quoute (\") is only allowed inside a string: $sql");
	
	}
	
	return $sql;

}

function pQgParseString($value) {

	if (PQG_LOG) pQgLog("pQgParseString-start: $value");
	
	global $pQgStrings, $pQgStringsKey;

	if ( ! isset ($pQgStringsKey) )
		$pQgStringsKey = 0;
	
	if ( substr($value, 0, 2) == '{{' and substr($value, -2) == '}}'  ) 
		$replace = $value;
	else {
		$key = array_search($value, $pQgStrings);
		if ($key !== FALSE)
			$replace = "{{{$key}}}";	 
		else {
			$pQgStringsKey++;		
			$pQgStrings [$pQgStringsKey] = $value; 
			$replace = "{{{$pQgStringsKey}}}";	
		}
	}
	
	if (PQG_LOG) pQgLog("pQgParseString-end: '$replace'");
	
	return "'$replace'";

}

function pQgParseNonString($value) {

	if (PQG_LOG) pQgLog("pQgParseNonString-start: $value");
	
	$pQgSplit = array();
	
	$len 	= strlen($value);
	$temp 	= '';
	$work 	= array();
	$i		= 0;
	
    while ( $i < $len ) {
  	
		$p = substr($value, $i, 1);

		if ($i+1 < $len)	$next = substr($value, $i+1, 1);
		else				$next = '';
				
		if (    ( $p == '<' and $next == '>' ) 
		     or ( $p == '>' and $next == '=' )
		     or ( $p == '<' and $next == '=' )
			 or ( $p == '!' and $next == '=' )
			 or ( $p == '=' and $next == '=' )
		     or ( $p == '&' and $next == '&' )
			 or ( $p == '|' and $next == '|' ) ) {

			if ($temp !== '') {
			 	$pQgSplit[] = $temp;
				$temp = '';
			}
			
			$pQgSplit[] = $p . $next;
			$i++;
			
		}  
		  
		elseif ( strstr(" \t\r\n()%^*/-+=<>!&," , $p) )  {  
		       	
			if ($temp !== '') {
				$pQgSplit[] = $temp;
				$temp = '';
			}
		
			if (! strstr(" \t\r\n", $p))
			  $pQgSplit[] = $p;

		}
		
		else
			$temp .= $p;

		$i++;
		
	}

	if ($temp !== '')
		$pQgSplit[] = $temp;

	$info = implode("|", $pQgSplit);
	if (PQG_LOG) pQgLog("pQgParseNonString-end: $info");
		
	return $pQgSplit;
		
}

function pQgStringRedo($sql, $option) {

	if (PQG_LOG) pQgLog("pQgStringRedo-start: |$sql|");
	
	$check = explode("'", $sql);

	$sql = '';

	foreach($check as $key => $value)
		if ($key % 2)  $sql .= pQgGetString($value, $option);
        else           $sql .= $value;

        return $sql;

	if (PQG_LOG) pQgLog("pQgStringRedo-End: |$sql|");
        
}

function pQgGetString ($key, $escape) {

	if (PQG_LOG) pQgLog("pQgGetString-start: |$key|");
	
	global $pQgStrings;
		
	$key = substr($key, 2, -2);
	
	if (PQG_CHECK and ! isset ( $pQgStrings [$key] ) )
		pQgError("Internal pQg error, string |$key| not found.");
	
	$string = $pQgStrings [$key];
	
	$string = str_replace('###ESCAPED-ESCAPE###',       "\\\\", $string);
	$string = str_replace('###ESCAPED-SINGLE-QUOTE###', "'",    $string);
	$string = str_replace('###ESCAPED-DOUBLE-QUOTE###', '"',    $string);
	$string = str_replace('###ESCAPE###',               "\\",   $string);
	
	$string = pQgStringQuotes($string, 'String', $escape, 'STRING');
		
	if (PQG_LOG) pQgLog("pQgGetString-end: |$string|");
	
	return $string;
	
}

function pQgStringQuotes($string, $type, $escape, $kind) {

	if (PQG_LOG) pQgLog("pQgStringQuotes-start: |$string|$type|$escape|");
	
	if ($kind == 'GET')
		$string = str_replace("\\", "\\\\",    $string);
	
	if ($escape == 'PHP') 
		$string = str_replace("'", "\\'",    $string);
	else
		$string = str_replace('"', "\\".'"', $string);
	
	if ($type == 'String') {
		if ($escape == 'PHP') 
			$string = "'" . $string . "'";
		else
			$string = '"' . $string . '"';
	}

	if (PQG_LOG) pQgLog("pQgStringQuotes-end: |$string|$type|$escape|");
	
	return $string;

}

function pQgParseSQL() {

	global $pQgReParse;
	
	if (PQG_LOG) pQgParseLog("pQgParseSQL-start");
	
	do {		

		$pQgReParse = FALSE;

		pQgParseSqlDouble         () ;
		pQgParseSqlWordsKeysParts () ;
		pQgParseSqlClose          () ;
		pQgParseSqlExists         () ;

	} while ($pQgReParse);
							
	if (PQG_LOG) pQgParseLog("pQgParseSQL-end: ");
					
}

function pQgParseSqlDouble () {
	
	global $pQgParse, $pQgDouble;
	
	if (PQG_LOG) pQgParseLog("pQgParseSqlDouble-start");
	
	foreach( $pQgParse as $k => $v ) 
	
		foreach( $pQgDouble as $p1 => $p2 )
		
			if ( strtolower($v) == $p1 and strtolower( $pQgParse  [$k+1] ) == $p2) {
				$pQgParse [$k] = strtolower("$p1 $p2");
				unset $pQgParse [$k+1];
			}
			
	$pQgParse  = array_merge($pQgParse);

	if (PQG_LOG) pQgParseLog("pQgParseSqlDouble-end");

}

function pQgParseSqlWordsKeysParts () {

	global $pQgCommand, $pQgParse, $pQgParts, $pQgClauses, $pQgCombined, $pQgWords, $pQgKeys;
	
	$pQgWords = $pQgKeys = $pQgParts = array();

	if (PQG_LOG) pQgParseLog("pQgParseSqlWordsKeys-start");
	
	foreach($pQgParse as $k1 => $v1) {
		
		$l = strtolower($v1);
		
		$pQgParts[$l][] = $k1;
			
		if (array_search($l, $pQgCombined) !== FALSE)
			$pQgParse [$k1] = $l;

		if (array_search($l, $pQgClauses) !== FALSE and ! isset($pQgWords [$l])) {	
				$pQgWords [$l]  = $k1;
				$pQgKeys  [$k1] = $l				
		}			
				
	}

	$pQgCommand = $pQgKeys [0];
	
	if (PQG_LOG) pQgParseLog("pQgParseSqlWordsKeys-end");

}

function pQgParseSqlClose () {
	
	global $pQgParse, $pQgParts, $pQgClose;

	$pQgClose = array();
	
	if (PQG_LOG) pQgParseLog("pQgParseSqlClose-start");
	
	foreach ( $pQgParts ['('] as $key ) {
	
		$start = $key + 1;
		$end   = count($pQgParse);

		$level = 1;

		for ( $i=$start; $i<=$end; $i++ ) {
		
			if ($pQgParse[$i] == '(') $level++;

			if ($pQgParse[$i] == ')') {
				$level--;
				if (!$level) {
					$pQgClose[$key] = $i;
					break;
				}
 			}
						
		}

	}

	if (PQG_LOG) pQgParseLog("pQgParseSqlClose-end");

}

function pQgParseSqlExists () {
	
	global $pQgReParse, $pQgParse, $pQgParts, $pQgExists, $pQgClose;

	//	$pQgExists = array();
	
	if (PQG_LOG) pQgParseLog("pQgParseSqlExists-start");
	
	foreach ( $pQgParts ['('] as $key ) {

		if ($pQgParse [ $key + 1] == 'select') {
		
			$sub = pQgGetContent($key);

			if (PQG_LOG) pQgLog("pQgParseSqlExists-sub: |$sub|");

			$exists['sub'] = pQgStringRedo($sub, 'PHP');

			if (PQG_CHECK and $pQgParse[$key-1] <> 'exists' and $pQgParse[$key-1] <> 'not exists')
				pQgError("Subqueries are only supported with EXISTS and NOT EXISTST");
			
			if ($pQgParse[$key-2] == 'and')
				unset($pQgParse[$key-2]);
				
			if ($pQgParse[$key-2] == 'where' and ! isset($pQgParse[$pQgClose[$key]+1]))
				unset($pQgParse[$key-2]);
				
			if (PQG_CHECK and $pQgParse[$key-2] == 'or')
				pQgError("'EXISTS' and 'NOT EXISTS' is only supported with 'AND' and not with 'OR'.");
			
			if ($pQgParse[$key-1] == 'exists')
				$exists['type'] = 'exists';
				
			if ($pQgParse[$key-1] == 'not exists')		
				$exists['type'] = 'not exists';

			$pQgExists[] = $exists; 
			 	
			for ( $i=$key-1; $i<=$pQgClose[$key]; $i++ )
				unset($pQgParse[$i]);
				
			$pQgReParse = TRUE;

		}

	}
	
	if (PQG_LOG) pQgParseLog("pQgParseSqlExists-end");

}

function pQgGetContent($key) {

	global $pQgParse, $pQgClose;
	
	$start = $key + 1;
	$end   = $pQgClose [$key] - 1;
		
	$return = '';
		
	for ( $i=$start; $i<=$end; $i++ )
		$return .= ' ' . $pQgParse[$i];
		
	return trim($return);
	
}    

function pQgParseTable () {
	
	global $pQgAlias, $pQgCfgFields, $pQgCommand, $pQgCfgClass, $pQgClass, $pQgTable, $pQgParse, $pQgReparse, $pQgTableFields;
	
	if (PQG_LOG) pQgLog("pQgTable-start: $pQgCommand");
	 
	$tableLocation ['select']        = 'from';
	$tableLocation ['update']        = 'set';	
	$tableLocation ['truncate']      = 'table';
	
	$key = FALSE;
	
	if ( isset ($tableLocation [$pQgCommand]) )
		$key = pQgGetByWord( $tableLocation [$pQgCommand] ) ;
	
	if (PQG_CHECK and ( ! $key and $pQgCommand <> 'truncate' and $pQgCommand <> 'insert into' and $pQgCommand <> 'delete from' ) ) 
		pQgError("Required option " . $tableLocation [$pQgCommand] . " not found");

	if ( ($pQgCommand == 'truncate' and !$key) or $pQgCommand == 'update' or $pQgCommand == 'insert into'  or $pQgCommand == 'delete from')
		$key = pQgGetByWord( $pQgCommand ) + 1;
	else
	    $key++;

	$pQgTable = $pQgParse[$key];
	if (PQG_LOG) pQgLog("pQgTable-set: $pQgTable");
	
	if ( PQG_CHECK and ! $pQgTable )
		pQgError("Table not found: $key");
	
	if ( PQG_CHECK and $pQgParse[$key + 1] == ',' )
		pQgError("Joins are not allowed (1): $key");
	
	if ( PQG_CHECK and $pQgParse[$key + 2] == ',')
		pQgError("Joins are not allowed (2): $key");
	
	$alias = $pQgParse[$key + 1] ;
	$alias = pQgAliasCheck( $alias );
	
	$pQgAlias = '';
	if ( $alias ) {
		$pQgAlias = $alias;
		unset($pQgParse[$key+1]);
		pQgAliasRemove( $alias );
		$pQgReparse = TRUE;
    	if (PQG_LOG) pQgLog("pQgParseTable: Alias removed |$pQgTable|$alias|");
    }

    $pos = strrpos($pQgTable, '.');
    
    if ($pos) {
    	$old = $pQgTable;
    	$database = substr($pQgTable, 0, $pos);
    	$pQgTable = substr($pQgTable, $pos+1);
    	if (PQG_LOG) pQgLog("pQgParseTable: Database removed |$old|$database|$pQgTable|");
    	pQgAliasRemove($database);
		if (PQG_CHECK) pQgWarning("Database name ignored: '$database' - pQg uses the package name of the class.");
		$pQgReparse = TRUE;		
    }

 	pQgAliasRemove($pQgTable);

	$pQgClass = $pQgCfgClass [$pQgTable];
	
	$pQgTableFields = array();
 	foreach( $pQgCfgFields [$pQgTable] as $key => $field )
		$pQgTableFields [$field] = $key;
			
}	

function pQgAliasCheck($alias) {

	global $pQgCommand;
	
	if (PQG_LOG) pQgLog("pQgAliasCheck-start: |$alias|$pQgCommand|");
	
	$alias = trim(strtolower($alias));
	
	$no_alias ['insert into' ] = array ( '(', 'set', 'select', 'value', 'values' ) ;
	$no_alias ['select' ]      = array ( 'where', 'group by', 'order by','limit', 'having', 'procedure', 'into', 'for' ) ;
	$no_alias ['update' ]      = array ( 'set' ) ;
	$no_alias ['delete from' ] = array ( 'where','order by','limit' ) ;
  	
    foreach ( $no_alias [$pQgCommand] as $nono )
    	if ($alias == $nono)
    		return '';
    		
	if (PQG_LOG) pQgLog("pQgAliasCheck-end: |$alias|");
    		
    return $alias;

}

function pQgAliasRemove($prefix) {

	global $pQgParse;
	
	$alias = trim($prefix) . '.';
	
	foreach($pQgParse  as $key => $value) {
		if ( stristr($value, $alias) ) {
			$pQgParse  [$key] = str_ireplace($alias, '', $value);
	    	if (PQG_LOG) pQgLog("pQgRemovePrefix: Because pQg can only work on a single table the prefix '$alias' is removed from '$value'");
		}
		
	}
	
}

function pQgSaveFields ($string) {

	$pos = 0;
	$len = strlen($string);
	$result = '';
	$open = 0;
	$close = 0;

	while ($pos < $len) {

		$one = substr($string, $pos, 1);

		if ($one == '(')	$open++;
		if ($one == ')')	$open--;
			
		if ($one == ',' and $open)	$result .= '###TEMP###';	
		else 						$result .= $one;
 
		$pos++;
		
	}

	return $result;
}

function pQgSaveFieldsUndo ($string) {

	return str_replace('###TEMP###', ',', $string);

}

function pQgGetKey($object) {

	global $pQgCfgKey, $pQgTable;
	
	$key = 'get' . strtoupper(substr($pQgCfgKey[$pQgTable], 0, 1)) . substr($pQgCfgKey[$pQgTable], 1) ;
	
	return $object->$key();

}

function pQgDeleteUpdateLimit () {

	global  $pQgParse;

	if (PQG_LOG) pQgLog("pQgDeleteUpdateLimit-start: $sql");
	
	$limit = pQgGetByWord('limit');
	
	if ($limit) {
		
		$options = pQgOptions('limit');
	
		$count = intval($options);

		if (PQG_LOG) pQgLog("pQgDeleteUpdateLimit-start: $count");
		
		if (PQG_CHECK and ! $count)
			pQgError("The option LIMIT must be an integer value: " . substr($sql, $limit));
	
		$pQgParse  [$limit + 1] = "0,$count";
	
		pQgParse();
			
	}

}

function pQgGetByWord($word) {

	global  $pQgWords;
	
	$return = FALSE;
	
	if ( isset  ( $pQgWords  [$word] ) )
		$return = $pQgWords  [$word] ;

	if (PQG_LOG) pQgLog("pQgGetByWord: |$word|$return|");

	return $return;
		
}

function pQgOptionsEnd ($option) {
	
		global $pQgParse, $pQgWords;

		$start = $pQgWords [$option] + 1;
			
		$len = count ( $pQgParse );
		$return = '';
		
		for ( $i=$start; $i<$len; $i++ )
			$return . ' ' . $pQgParse[$i];
			
		if (PQG_LOG) pQgLog("pQgOptionsEnd: |$option|$return|");

		return trim($return);
		
}

function pQgOptions ($option) {
	
		global $pQgParse, $pQgWords, $pQgKeys;

		if ( isset( $pQgWords  [$option] ) )
			$start = $pQgWords  [$option] + 1;
		elseif ( isset ($pQgParse  [$option]) )
			$start = $option + 1;

			//FIXME remove error supression but first check meaning of it.
		if ( ! isset ( $pQgParse  [@$start] ) ) {
			if (PQG_LOG) pQgLog("No options found for: $option");
			return FALSE;
		}
			
		$len = count ( $pQgParse );
		$return = '';
		
		for ( $i=$start; $i<$len; $i++ ) {
			
			if ( isset($pQgKeys[$i]) )
				break;
					
			$item = $pQgParse[$i];
			
			if ($item == "and")
		        $return .= ' &&';
        	elseif ($item =='or')
				$return .= ' ||';
        	elseif ($item == 'not')
				$return .= ' !';
        	elseif ($item == '=')
				$return .= ' ==';
    		elseif ($item == '<>')
				$return .= ' !=';
    		else
      			$return .= ' ' .  $item;

		}  			
			
	if (PQG_LOG) pQgLog("pQgOptions: |$option|$check|$start|$return|");

	return trim($return);
		
}

function pQgGetType($field) {
	
	global $pQgTable, $pQgCfgFields, $pQgCfgTypes;

	$key = array_search($field, $pQgCfgFields [$pQgTable] );
	
	return $pQgCfgTypes [$pQgTable] [$key];

}

function pQgGetValue($object, $field, $value, $escape) {

	global $pQgTableFields;
	
	if (PQG_LOG) pQgLog("pQgGetValue-start: |$field|$value|");

	$source = '';
	$parts = explode("'", $value);

	foreach($parts as $key => $value)
		if ($key % 2)
			$source .= ' '.pQgGetString($value, $escape);
		else {
			$items  = pQgParseNonString($value);
			foreach($items as $p)
				if ( substr($p, 0,1) == '`' )
					$source .= ' '.pQgGetObjectValue($object, substr($p, 1, -1), $escape);
				elseif ( strstr(" \t\r\n()%^*/-+=<>!&,." , $p) )
					$source .= ' '.$p;
				elseif (is_numeric($p))
					$source .= ' '.$p;
				else
					$source .= ' '.pQgGetObjectValue($object, $p, $escape);
		}	

    if ( ! trim($source) ) {
		if (PQG_LOG) pQgLog("pQgGetValue-empty-source: |$field|$value|");
		return '';
    }
    
	$eval = '$result = ' . $source;
	
	if (PQG_LOG) pQgLog("pQgGetValue-eval: |$field|$eval|");
	
	try   eval ($eval);
	catch (Exception $e) if (PQG_CHECK) pQgExceptionHandler($e);
	
	if (PQG_LOG) pQgLog("pQgGetValue-end: |$field|$result|");	
	
	return $result ;

}

function pQgGetObjectValue($object, $field, $escape) {

	global $pQgTable, $pQgTableFields;
	
	if (PQG_LOG) pQgLog("pQgGetObjectValue-start: |$field|");
	
	if (PQG_CHECK and ! isset($pQgTableFields[$field]) and ! function_exists ($field) )
		pQgError("'$field' is not a Field of table '$pQgTable' and is not a PHP function");

	if ( ! isset($pQgTableFields[$field]))
		return $field;
		
	if ( is_array($object) )
		$value = $object[$field];
	else 
		eval('$value = $object->'.'get'.strtoupper(substr($field, 0, 1)).substr($field, 1).'();');
	
	$value = pQgStringQuotes($value, pQgGetType($field), $escape, 'GET');
	
	if (PQG_LOG) pQgLog("pQgGetObjectValue-end: |$field|$value|");
	
	return $value;
		
}


function pQgLogLayout($log) {
	
	$log = str_replace("\t", '[tab]', $log);
	$log = str_replace("\r", '[cr]',  $log);
	$log = str_replace("\n", '[lf]',  $log);
	
	return $log;
	
}

function pQgGo ($go, $p1='', $p2='') {

	global $pQgLocation;
	
	include_once "$pQgLocation/pQg$go.inc";
	
	$go = "pQg$go";
	
	return $go ($p1, $p2);
	
}

?>