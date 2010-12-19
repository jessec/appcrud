<?php 

function getmicrotime() {
	list($usec, $sec) = explode(" ",microtime());
	return ((float)$usec + (float)$sec);
}

$time = getmicrotime();

function pQgLog($log) {

	global $pQgTimes, $pQgLastTime, $pQgLog, $pQgLogKey, $pQgTimes, $pQgLast;
	
	list($mark, $rest) = explode(':', $log, 2);
	
	$now = getmicrotime();
 //	$pQgTimes[][$mark]	= $now - $pQgLastTime;
	$pQgLastTime = $now;
	
	if ( ! isset($pQgLogKey) )	$pQgLogKey  = 1;
	else 						$pQgLogKey++;
	
	$info = pQgLogLayout($log);
	
	$pQgLog  [$pQgLogKey] = $info;
	
	if  ( $pQgLogKey > 1000 )
		unset ( $pQgLog  [ $pQgLogKey - 1000 ] );

	logInfo($info);
	
}

function pQgParseLog($info) {

	global  $pQgParse, $pQgWords, $pQgKeys, $pQgClose;

	$log1 = $log2 = $log3 = $log4 = '';
		
	if ( isset($pQgParse ) and is_array($pQgParse ) and count($pQgParse ) ) $log1 = implode('|', $pQgParse  );
	if ( isset($pQgWords ) and is_array($pQgWords ) and count($pQgWords ) ) $log2 = implode('|', $pQgWords  );
	if ( isset($pQgKeys  ) and is_array($pQgKeys  ) and count($pQgKeys  ) ) $log3 = implode('|', $pQgKeys   );
	if ( isset($pQgClose ) and is_array($pQgClose ) and count($pQgClose ) ) $log4 = implode('|', $pQgClose  );
	
				        if (PQG_LOG) pQgLog($info);
	if ( $log1 !== '' ) if (PQG_LOG) pQgLog("parse: |$log1|");
	if ( $log2 !== '' ) if (PQG_LOG) pQgLog("words: |$log2|");
	if ( $log3 !== '' ) if (PQG_LOG) pQgLog("keys:  |$log3|");
	if ( $log4 !== '' ) if (PQG_LOG) pQgLog("close:  |$log4|");
	
}

?>