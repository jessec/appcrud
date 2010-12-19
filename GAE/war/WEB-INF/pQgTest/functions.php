<?php 

function pQgTestView ($file) {

	$functions = array('sql','pQgTest','sqlRecord','sqlField', 'sqlRecords');

	echo "$file".' &nbsp; &nbsp; <a href="#" onclick="history.go(-1)">Back</a><font color="#000000"><pre>'."\n";
	
	$source = trim(fread(fopen($file, 'r'), filesize($file)));

	$source = str_replace('<', "&lt;",   $source);
	$source = str_replace("\t", "    ",  $source);

	foreach($functions as $function)
		while ($parts = pQgParts($source, $function . '("', '");') )
			$source = $parts[1] . $function . '(&quot;' . pQgLayout($parts[2]) . '&quot;);' . $parts[3];

	$explode  = explode("pQg", $source);
	$source   = implode('<font color="#FF0000"><b>pQg</b></font>', $explode);

	$explode  = explode("PQG", $source);
	$source   = implode('<font color="#FF0000"><b>PQG</b></font>', $explode);

	$explode  = explode("sql(", $source);
	$source   = implode('<font color="#FF0000"><b>sql</b></font>(', $explode);
	
	$lines = split("\n", $source) ;

	for ( $i=1 ; $i <= sizeof($lines) ; ++$i )
		echo '<font color="#000000"><b>' . substr( ('000' . ($i)), -4)  . '</b></font> ' . rtrim($lines[$i-1]) ."\n" ;

	echo '</pre>';

}


function pQgTest($sql) {

	global $pQgCommand, $pQgTable;
	
	$print = str_replace("\t", "    ", $sql);
	$print = str_replace("\n             ", "", $print);
	$print = pQgLayout($print);

	echo "<br>$print\n\n";
	
	$result = sql($sql);

	echo pQgLogMessage($result) . "\n";

	if (PQG_LOG) pQgLog("pQqTest-mid: |$pQgCommand|$pQgTable|");
	
	if ($pQgCommand == 'select')
		pQgDumpRecords($result);
	else
		pQgDumpTable();

	echo "<hr>";
		
	return $result;
		
}

function pQgDumpTable($table='') {
	
	global $pQgTable;

	if (!$table)
		$table = $pQgTable;

	if (PQG_LOG) pQgLog("pQgDumpTable: |$table|");
	
	$records = sql("select * from $table");
	
	pQgDumpRecords($records);

}

function pQgDumpRecords ($records) {	

	global $pQgInfo, $pQgTableFields;
	
	echo '<br><table border="1">'."\n<tr>";

	if ( ! count ($records) )
		foreach ( $pQgTableFields as $key => $field )
			echo "<td><b>&nbsp;" . $key . "&nbsp;</b></td>";
	else 
		foreach($records[0] as $key => $field)
			echo "<td><b>&nbsp;$key&nbsp;</b></td>";

	echo "</tr>";

	foreach ($records as $record) {
		echo "<tr>";
		foreach($record as $field)
			echo '<td>&nbsp;'.htmlentities($field).'&nbsp;</td>';
		echo "</tr>\n";
	}

	echo "</table>\n";

}

function pQgLayout($string) {

	$string = str_replace("\\'", '###REPLACE###',  $string);

	$check = explode("'", $string);
	
	foreach($check as $key => $value)
		if ($key % 2)
			$check[$key] = '<font color="#000000"><b>' . $value . '</b></font>';
		else
			$check[$key] = pQgLayoutMore($value);

	$string = implode('<font color="#5C5858"><b>'."'".'</b></font>', $check);

	$string = str_replace('###REPLACE###',  "\\'", $string);

	$string = '<font color="BLUE"><b>' . $string . '</b></font>';

	$explode  = explode("pQg", $string);
	$string   = implode('<font color="#7D2252" size="+1"><b>pQg</b></font>', $explode);
	
	return $string;

}

function  pQgLayoutMore($layout) {
	
	$keywords = array('desc', 'asc', 'select', 'insert', 'update', 'delete', 'from', 'into', 'as', 'set', 'values', 'where', 'between', 'limit', 'order', 'by', 'table', 'trucate', 'or', 'and');

	$layout = str_replace('&lt;', ' ###LT### ', $layout);
	
	$len 	= strlen($layout);
	$temp 	= '';
	$work 	= array();

	for ( $i=0; $i<$len; $i++ ) {
		
		$p = substr($layout, $i, 1);

		if ( strstr( " \n\r\t()%^*/-+.=<>!&," , $p)  )  {
			$work[] = $temp;
			$work[] = $p;
			$temp = '';
		}
		else
			$temp .= $p;

	}

	if ($temp)
		$work[] = $temp;

	$layout	= '';

	foreach($work as $value)
		if ($value !== '')
			if ( is_numeric(trim($value)) )
				$layout .= '<font color="#000000"><b>'.$value.'</b></font>'  ;
			elseif ( function_exists ( strtolower($value)) )
				$layout .= '<font color="#8B008B"><b>'.$value.'</b></font>'  ;
			elseif ( strstr( '()%^*/-+=.<>!,' , $value) )
				$layout .= '<font color="#F88017">'.$value.'</font>'  ;
			else {
				$hit = FALSE;
				foreach ($keywords as $word)
					if ( strtolower($value) == $word)
						$hit = TRUE;
				if ($hit)
					$layout .= '<font color="GREEN"><b>'.$value.'</b></font>';
				else
					$layout .= $value;
			}

	$layout = str_replace(' ###LT### ', '<font color="RED"><b>&lt;</b></font>', $layout);
			
	return $layout;

}

function pQgLogMessage($result) {

	global $pQgCommand;

	if ($pQgCommand == 'insert into')
		if (is_array($result) )
			return "Added " . $result[0] . " records";
		else
			return "Added a record with primary key: $result";

	if ($pQgCommand == 'select')
		return "Found " .  count($result) . " record(s).";
	
	if ($pQgCommand == 'update')	
		return "Updated $result record(s).";
	
	if ($pQgCommand == 'delete from')
		return "Deleted $result record(s).";	

}



function pQgParts($string, $start, $end) {

	$work1 = explode($start, $string, 2);

	if ($work1[0] == $string)
		return FALSE;

	if (!$work1[1])  $string = $work1[0];
	else             $string = $work1[1];

	$work2 = explode($end, $string, 2);

	if ($work2[0] == $string)
		return FALSE;

	if (!$work1[1])  $parts[1] = '';
	else             $parts[1] = $work1[0];

	$parts[2] = $work2[0];
	$parts[3] = $work2[1];

	return $parts;

}

function a() {
	pQgError('Dump');
}

?>