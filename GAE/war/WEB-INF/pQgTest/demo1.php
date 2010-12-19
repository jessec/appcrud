<html>
<head>
<title>pQg - PHP with SQL on Google App Engine - CRUD Test</title>
<meta name="description"
	CONTENT="pQg - PHP with SQL on Google App Engine">
<meta name="keywords"
	CONTENT="pQp, PHP, JDO, SQL, GAE, Google App Engine, Google, App Engine, Quercus">
</head>
<body>
<center>
<h2>pQg - PHP with SQL on Google App Engine</h2>
<h3>Full CRUD (Insert, Read, Update & Delete)</h3>
<hr>
<p><a href="pQg.php?action=run&file=demo1&go=CREATE">Create</a></p>
<table>
	<tr>
		<td>Delete</td>
		<td>Update</td>
	</tr>

	<?php

	// Import the JDO java class for the database

	import myDatabase.test;

	// Define the database tables for pQg

	$pQgCfgClass  ['test'] = 'myDatabase.test';
	$pQgCfgKey    ['test'] = 'id';
	$pQgCfgFields ['test'] = array ( 'id',   'field1', 'field2' );
	$pQgCfgTypes  ['test'] = array ( 'Long', 'Long',   'String' );

	// Setting pQg paratmeters

	define('PQG_TEST',  TRUE   );
	define('PQG_CHECK', FALSE  );
	define('PQG_LOG',   FALSE  );

	// Include the pQg functions

	$pQgLocation = 'WEB-INF/pQg';
	include "$pQgLocation/pQg.inc";

	// Get the parameters.

	if (isset($_REQUEST['go']) )     $go     = addSlashes($_REQUEST['go']);
	else                             $go     = '';

	if (isset($_REQUEST['demo']) )   $id     = addSlashes($_REQUEST['demo']);
	else                             $id     = '';

	if (isset($_REQUEST['field1']) ) $field1 = addSlashes($_REQUEST['field1']);
	else                             $field1 = '';

	// Insert, Update, Delete

	if ( $go == 'CREATE' )
	sql("insert into test (field1, field2) values(1, 'Yes it works')");

	if ( $go == 'UPDATE' )
	sql("update test set field1 = field1 + 1 where id = $id");

	if ( $go == 'DELETE' )
	sql("delete from test where id = $id");

	// Retrieve all records and print it.

	$rows = sql("select * from test");



	$result = array();
	foreach ($rows as $row) {
		//print_r($row);
		array_push($result, $row);
		extract($row);
		echo "<tr>";
		echo "	<td align=\"center\"><a href=\"pQg.php?action=run&file=demo1&go=DELETE&demo={$id}\">{$id}</a></td>";
		echo "	<td align=\"center\"><a href=\"pQg.php?action=run&file=demo1&go=UPDATE&demo={$id}&field1={$field1}\">{$field1}</a></td>";
		echo "	<td>{$field2}</td>";
		echo "</tr>";
		
	}
	echo '<pre>';
	//print_r($result);
	echo json_encode($result);

	pQgClose();



	?>

</table>
<br>
<hr>
<br>
<a href="/">Back</a></center>
</body>
</html>
