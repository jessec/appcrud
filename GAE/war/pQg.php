<?php





include dirname(__FILE__).'/headless/storedDataModels.php';
include dirname(__FILE__).'/headless/lib/url2sql.php';
include dirname(__FILE__).'/headless/lib/pipe.php';


function url ($name, $link)
{
	return '<a href='.$link.'> '.$name.' </a>';
}

echo '<pre>';

echo url('insert', '/?/m:ttest/i:test/v:field1.1,field2.test');
echo url('update', '/?/m:ttest/u:test/v:field1.88,field2.testupdate/w:field2.eq.test');
echo url('select', '/?/m:ttest/s:all/t:test');
//echo url('select', '/?/a:users/c:members/f:color/s:name,status,age/t:users/w:id.eq.4,name.lt.56/l:2,2/o:name,status.desc');
echo url('delete', '/?/m:ttest/d:/t:test/');

echo '<br><br>';

$uql = new Url2sql;
$p['url'] = $_SERVER['QUERY_STRING'];

if($p['url'] == ''){
	$p['url'] = '/?/m:ttest/s:all/t:test';
}

$uql->setParam($p);

$sql = $uql->getSql();
$meta = $uql->getMeta();


echo '<br><br>';
echo '<br>Meta :<br>';


print_r($meta);

extract($meta);

/*echo '<br><br>';
echo '<br>Action :<br>';
echo $action;
echo '<br><br>';
echo '<br>Model :<br>';
print_r($model);*/


$data = 'test';

$datamodel = array(
		'name' => 'testmodel',
		'action' => explode(',',$model),
		'data' => $p['url']
);




/*echo '<br>Data model :<br>';
print_r($datamodel);*/

$pipe = new Pipe;
$pipe->setPath('headless/models/');
$pipe->setData($datamodel['data']);
$pipe->setModel($datamodel['action']);
$result = $pipe->result();

echo '<br>Result :<br>';
print_r($result);

/*
 * 
 * 
 * 
 */






/*
 *  DB action
 * 
 */

	import myDatabase.test;

	// Define the database tables for pQg

	$pQgCfgClass  ['test'] = 'myDatabase.test';
	$pQgCfgKey    ['test'] = 'id';
	$pQgCfgFields ['test'] = array ( 'id',   'field1', 'field2' );
	//$pQgCfgTypes  ['test'] = array ( 'Long', 'String',   'String' );
	$pQgCfgTypes  ['test'] = array ( 'String', 'String',   'String' );

	// Setting pQg paratmeters

	define('PQG_TEST',  TRUE   );
	define('PQG_CHECK', FALSE  );
	define('PQG_LOG',   FALSE  );

	// Include the pQg functions

	$pQgLocation = 'WEB-INF/pQg';
	include "$pQgLocation/pQg.inc";


	echo '<br><br>';
	echo '<br><br>';
	// Insert, Update, Delete
	echo $sql;
	
	echo '<br><br>';
	echo '<br><br>';
	
	
/*	if ( $go == 'CREATE' )
	sql("insert into test (field1, field2) values(1, 'Yes it works')");

	if ( $go == 'UPDATE' )
	sql("update test set field1 = field1 + 1 where id = $id");

	if ( $go == 'DELETE' )
	sql("delete from test where id = $id");*/

	// Retrieve all records and print it.

	//$sql = 'delete from test where id > 0';
	
	//echo $sql;
	
	$rows = sql($sql);
	
	//var_dump($rows);

	if(!is_array($rows) || empty($rows)){
		$rows = sql('select * from  test  ');
	}


	echo '<table>';
	$result = array();
	foreach ($rows as $row) {
		extract($row);
		$update = $field1  + 1;
		//print_r($row);
		array_push($result, $row);

		echo "<tr>";
		echo "	<td align=\"center\"><a href=\"/?/m:ttest/d:where/t:test/w:id.eq.{$id}\">{$id}</a></td>";
		echo "	<td align=\"center\"><a href=\"/?/m:ttest/u:test/v:field1.{$update},field2.testupdate/w:id.eq.{$id}\">{$field1}</a></td>";
		echo "	<td>{$field2}</td>";
		echo "</tr>";
		
	}
	echo '</table>';
	echo '<pre>';
	//print_r($result);
	echo json_encode($result);

	pQgClose();



exit;

/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

include 'apps/'.$ctr['a'].'.php';

/*
 $obj = new $ctr['c'];

 $obj->set($data);

 $return = '';
 foreach ($ctr['f'] as $function){
 $return = $obj->$function($return);
 }
 print_r($return);
 */

exit;

//include 'WEB-INF/pQgTest/pQg.php';

$text = 'data to encrypt';

$data['data'] = $text;
$data['controller'] = 'insert';

$text = serialize($data);

$key = "haseveryletter";

$iv = mcrypt_create_iv(mcrypt_get_iv_size(MCRYPT_RIJNDAEL_128, MCRYPT_MODE_CFB), MCRYPT_RAND);
$encrypted = mcrypt_encrypt(MCRYPT_RIJNDAEL_128, $key, $text, MCRYPT_MODE_CFB, $iv);

$encrypted = base64_encode($encrypted);
$iv = base64_encode($iv);
echo 'iv : '.$iv;
echo '<br>encrypted : <br>';
echo substr($encrypted, 0, 10).'....';
echo '<br><br>';

echo 'iv : '.$iv;
echo '<br>decrypted : <br>';
echo '<br>';

if (isset($_POST['encrypted_data']))
{

	echo '<a href="/pQg.php">encrypt</a><br><br><br>';

	$iv = $_POST['encrypted_iv'];
	$encrypted = $_POST['encrypted_data'];

	echo 'strlen : '.strlen($encrypted);

	echo '<br><br><br>';

	$iv = base64_decode($iv);
	$good = base64_decode($encrypted);
	$good = mcrypt_decrypt(MCRYPT_RIJNDAEL_128, $key, $good, MCRYPT_MODE_CFB, $iv);

	$good = unserialize($good);

	print_r($good); // The quick brown fox jumps over the lazy dog

}
else
{
	echo '<form method="post" action="/pQg.php">
<input type="hidden" name="encrypted_iv" value="'.$iv.'">
<input type="hidden" name="encrypted_data" value="'.$encrypted.'">
<input type="submit" value="Send">
</form>
';
}
