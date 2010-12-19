<?php

function pQgCheckSQL () {
	
	global $pQgCommand;
	
	pQgCheckSQLCommand       ($pQgCommand, $sql) ;
	pQgCheckSQLSequence      ($pQgCommand, $sql) ;
	pQgCheckSQLCountBrackets ($pQgCommand, $sql) ;
	pQgCheckOptions          ();
	pQgCheckReserved         ();
	
}

function pQgCheckReserved () {
	
	global $pQgParse, $pQgCombined;
	
	foreach($pQgParse  as $k1 => $v1)
		foreach($pQgCombined as $k2 => $v2)
			if ($v1 == $v2)
				if (PQG_CHECK and ($pQgParse[$k1-1] == ',' or $pQgParse[$k1+1] == ','))
					pQgError("A reserved SQL word is surrounded with 1 or 2 commas: $v1 , use backtics, eq `xxx` if you want to use a SQL reserved word als fieldname in your table.");

}
					
function pQgCheckSQLCommand ($pQgCommand, $save_sql) {

	if (PQG_LOG) pQgLog("pQgCheckSQLCommand: $pQgCommand, $save_sql");
	
	$not ['insert'] = array('LOW_PRIORITY','DELAYED','HIGH_PRIORITY','IGNORE','ON DUPLICATE KEY UPDATE');	
	$not ['select'] = array('LIKE','NOT','EXISTS','GROUP BY','JOIN','HAVING','UNION','ALL','DISTINCT','DISTINCTROW','HIGH_PRIORITY','STRAIGHT_JOIN','SQL_SMALL_RESULT','SQL_BIG_RESULT','SQL_BUFFER_RESULT','SQL_CACHE','SQL_CALC_FOUND_ROWS','INTO','FOR UPDATE','LOCK IN SHARE MODE');
	$not ['update'] = array('LOW_PRIORITY','IGNORE');	
	$not ['delete'] = array('LOW_PRIORITY','QUICK','IGNORE','USING','JOIN');
	
	foreach( $not [$pQgCommand] as $nono) {

		$search = ' ' . strtolower($nono) . ' ';
		
		if (PQG_CHECK and stristr($save_sql, $search) ) {
	  		$pos = stripos($save_sql, $search);
	  		pQgError("Option $nono is not supported (use backtics, eq `xxx`, if you want to use it as a field name): " . substr($save_sql, $pos) ); 
		}	
	  	
	}

}

function pQgCheckSQLSequence ($pQgCommand, $sql) {

	$check = array('where','group by','having','order by','limit');
	
	$lastPos = 0;
	$lastStr = '';
	
	foreach($check as $sequence) {
		
		$pos = stripos($sql, " $sequence ");

		if ($pos) {
			
			if (PQG_CHECK and $pos < $lastPos)
				pQgError("Sequence error: '$lastStr' must be given before '$sequence' - $sql");

			$lastPos = $pos;
			$lastStr = $sequence;
				
		}

	}
	
}

function pQgCheckSQLCountBrackets ($pQgCommand, $sql) {

	if ( substr_count($sql, '(') <>  substr_count($sql, '(') )
		rpgError("The count of ( does not match the count of ): $sql");
		
}

function pQgCheckOptions () {

	$options = array('select','value','values','set','where','order by','group by','having','limit','between','like','not','exists');

	foreach($options as $option) 
		if (PQG_CHECK and pQgGetByWord($option) and ! pQgOptions($option) )
			pQgError("No options found for '$option'");

}

function pQgWarning($log) {
	
	logWarning(pQgLogLayout($log));
	
}

function pQgError($log, $errcontext=array() ) {

	global $pQgJDO, $pQgTimes,$pQg, $pQgStrings, $pQgInfo, $pQgLog, $pQgParse, $pQgWords, $pQgKeys;
	
	pQgClose();
	
	echo "</div></span></table></div></span></table></div></span></table></center>";
	echo "<pre>";
	
	$error = pQgLogLayout($log);
	
	if ($error <> 'Dump')
		echo "<b>pQg Error</b>\n &nbsp; " . htmlentities($error) . "\n";

	if ( isset($pQgJDO) and count($pQgJDO) ) {
		echo "\n<b>Executed JDO queries</b>\n";
		foreach ($pQgJDO as $value) {
			$value = htmlentities($value);
			echo " &nbsp; $value\n";
		}
		unset($GLOBALS['pQgJDO']);
	}
	
	if ( count ($errcontext) ) {
		echo "\n<b>Active Scope variables</b>\n<table>";
		print_r($errcontext);
	}
		
	$log1 = $log2 = $log3 = '';
	
	if ( isset($pQgParse ) and is_array($pQgParse ) ) $log1 = implode('|', $pQgParse  );
	if ( isset($pQgWords ) and is_array($pQgWords ) ) $log2 = implode('|', $pQgWords  );
	if ( isset($pQgKeys  ) and is_array($pQgKeys  ) ) $log3 = implode('|', $pQgKeys   );
		
	if ($log1)
		echo "\n<b>Parsed SQL</b>\n &nbsp; $log1\n";
	
	if ($log2 or $log3)
		echo "\n<b>SQL words information</b>\n &nbsp; |$log2|$log3|\n";
		
	if ( isset($pQgLog) and count($pQgLog)) {
		echo "\n<b>Information messages</b>\n<table>";
		$work = array_reverse($pQgLog , TRUE);
		foreach ($work as $key => $info) {
			list($a, $b) = explode(':', $info, 2);
			if (strlen($b) > 120)
			 	$b = substr($b, 0, 120);
			$b = htmlentities($b);  
			echo "<tr><td>&nbsp; &nbsp; $key&nbsp;&nbsp;</td><td>$a</td><td>:&nbsp;$b</td></tr>\n";
		}
		echo "</table>";
		unset($GLOBALS['pQgLog']);
	}
	
	if ( count($pQgStrings) ) {
		echo "\n<b>Strings from SQL statements</b>\n";
		foreach ($pQgStrings as $key => $value) {
			$value = htmlentities($value);
			echo " &nbsp; [$key] => $value\n";
		}
		unset($GLOBALS['pQgStrings']);
	}
	
	if ( isset($pQgTimes) and count($pQgTimes)) {
		echo "\n<b>Times between Info messages</b>\n<table>";
		foreach ($pQgTimes as $key => $value)
			foreach ($value as $key => $time) {
				$time = substr(str_replace('0.00', '', $time), 0, 3);
				echo "<tr><td>&nbsp; &nbsp; $key&nbsp;&nbsp;</td><td>$time</td></tr>\n";
			}
		echo "</table>";
		unset($GLOBALS['pQgTimes']);
	}
	
	echo "\n<b>All Global variables</b>\n\n";
	print_r($GLOBALS);

	echo "</pre>";
	
	exit;

}

function pQgExceptionHandler($exception) {

	global $pQg_E;
	
    $pQg_E['getMessage'] 		= $exception->getMessage();                 // Exception message
    $pQg_E['getCode'] 			= $exception->getCode();                    // User-defined Exception code
    $pQg_E['getFile'] 			= $exception->getFile();                    // Source filename
    $pQg_E['getLine'] 			= $exception->getLine();                    // Source line
    $pQg_E['getTrace']			= $exception->getTrace();                   // An array of the backtrace()
    $pQg_E['getTraceAsString']	= $exception->getTraceAsString();           // Formated string of trace

	pQgError("PHP Exception: " . $pQg_E['getMessage']);
  
}

function pQgErrorHandler($errno, $errstr, $errfile, $errline, $errcontext) {

	switch ($errno) {
    	
    case E_USER_ERROR:
    	
    	pQgError("$errfile - $errline: - $errstr - $errno", $errcontext);
    	
        break;

    case E_USER_WARNING:
    	
    	pQgError("$errfile - $errline: - $errstr - $errno", $errcontext);
 
    	break;

    case E_USER_NOTICE:

    	pQgError("$errfile - $errline: - $errstr - $errno", $errcontext);
    	
  		break;

    default:
    	
    	pQgError("$errfile - $errline - $errstr - $errno", $errcontext);
 
  		break;

    }

    return true;

}

?>