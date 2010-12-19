<?php 

function getmicrotime() {
	list($usec, $sec) = explode(" ",microtime());
	return ((float)$usec + (float)$sec);
}

$time = getmicrotime();

#
# benchmark code here
# 
function pQgLog($log) {

	global $pQgLog, $pQgLogKey, $pQgTimes, $pQgLast;
	
	$now = getmicrotime();
	
	$last = $now;
	if ( ! isset($pQgLogKey) )	$pQgLogKey  = 1;
	else 						$pQgLogKey++;
	
	$info = pQgLogLayout($log);
	
	$pQgLog  [$pQgLogKey] = $info;
	
	if  ( $pQgLogKey > 100 )
		unset ( $pQgLog  [ $pQgLogKey - 100 ] );

	logInfo($info);
	
}

function pQgParseLog($info) {

	global  $pQgParse, $pQgWords, $pQgKeys;

	$log1 = $log2 = $log3 = '';
		
	if ( isset($pQgParse ) and is_array($pQgParse ) and count($pQgParse ) ) $log1 = implode('|', $pQgParse  );
	if ( isset($pQgWords ) and is_array($pQgWords ) and count($pQgWords ) ) $log2 = implode('|', $pQgWords  );
	if ( isset($pQgKeys  ) and is_array($pQgKeys  ) and count($pQgKeys  ) ) $log3 = implode('|', $pQgKeys   );
			
				        if (PQG_LOG) pQgLog($info);
	if ( $log1 !== '' ) if (PQG_LOG) pQgLog("parse: |$log1|");
	if ( $log2 !== '' ) if (PQG_LOG) pQgLog("words: |$log2|");
	if ( $log3 !== '' ) if (PQG_LOG) pQgLog("keys:  |$log3|");
	
}

?>