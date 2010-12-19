<?php

include dirname(__FILE__).'/actions/ActionController.php';
session_start();

echo '<pre>';

$ctr = new ActionController;
$ctr->setParam($_POST, $_GET, $_SESSION);
$messages = $ctr->getMessages();
$res = $ctr->getResult();


foreach ($messages as $error){
	print_r($error);
}





exit;

$op['eq'] = '=';
$op['lt'] = '<';
$op['gt'] = '>';

echo '<pre>';

echo '<br><br>';

$s = array(
	'action' => 'select',
	'fields' => array('name', 'age'),
	'table'  => 'users',
	'where'  => array('id:eq:4', 'deleted:lt:1'),
	'limit'  => array(2, 9),
	'order'  => array('name', 'age')
);
$s = json_encode($s);
echo json_format($s);

echo '<br><br>';

$i = array(
	'action' => 'insert',
	'table'  => 'users',
	'data'   => array('name' => 'robert', 'sex' => 'male'),
);
$i = json_encode($i);
echo json_format($i);

echo '<br><br>';

$u = array(
	'action' => 'update',
	'table'  => 'users',
	'data'   => array('name' => 'robert', 'sex' => 'male'),
	'where'  => array('id' => '4'),
);
$u = json_encode($u);
echo json_format($u);

echo '<br><br>';

$d = array(
	'action' => 'delete',
	'table'  => 'users',
	'data'   => array('name' => 'robert', 'sex' => 'male'),
	'where'  => array('id' => '4'),
);
$d = json_encode($d);
echo json_format($d);
/*$r = getRequest($sub_dir = '/static/test_tree/headless/');

echo '<br><br>';

echo ':=db:create:table:users:id=int:name=varchar';

echo '<br><br>';

echo ':.=db:select:name.title.age.sex.status:from:users:where:name=robert:and:deleted=0';

echo '<br><br>';

echo 'db_select_name.title.age.sex.status_from_users_where_name=robert_and_deleted=0';

echo '<br><br>';

echo 'db-select-name.title.age.sex.status-from-users-where-name=robert-and-deleted=0';

echo '<br><br>';

echo 'db:select:name.title.age.sex.status:from:users:where:name=robert:and:deleted=0';
*/
//print_r($log);
//print_r($res);

function getRequest ($sub_dir)
{
	$raw_url = substr($_SERVER['REQUEST_URI'], (strlen($sub_dir) - strlen($_SERVER['REQUEST_URI'])));
	$url = substr($raw_url, 3);
	$separators = substr($raw_url, 0, 3 - (strlen($raw_url)));
	return array($separators, $url);
}

// Pretty print some JSON
function json_format ($json)
{
	$tab = "  ";
	$new_json = "";
	$indent_level = 0;
	$in_string = false;

	$len = strlen($json);

	for ($c = 0; $c < $len; $c++)
	{
		$char = $json[$c];
		switch ($char)
		{
			case '{':
			case '[':
				if (!$in_string)
				{
					$new_json .= $char."\n".str_repeat($tab, $indent_level + 1);
					$indent_level++;
				}
				else
				{
					$new_json .= $char;
				}
				break;
			case '}':
			case ']':
				if (!$in_string)
				{
					$indent_level--;
					$new_json .= "\n".str_repeat($tab, $indent_level).$char;
				}
				else
				{
					$new_json .= $char;
				}
				break;
			case ',':
				if (!$in_string)
				{
					$new_json .= ",\n".str_repeat($tab, $indent_level);
				}
				else
				{
					$new_json .= $char;
				}
				break;
			case ':':
				if (!$in_string)
				{
					$new_json .= ": ";
				}
				else
				{
					$new_json .= $char;
				}
				break;
			case '"':
				if ($c > 0 && $json[$c - 1] != '\\')
				{
					$in_string = !$in_string;
				}
			default:
				$new_json .= $char;
				break;
		}
	}

	return $new_json;
}

//Function will take an SQL query as an argument and format the resulting data as a
//    json(JavaScript Object Notation) string and return it.
function sql2json ($query)
{
	$data_sql = mysql_query($query) or die("'';//".mysql_error()); // If an error has occurred,
	//    make the error a js comment so that a javascript error will NOT be invoked
	$json_str = ""; //Init the JSON string.

	if ($total = mysql_num_rows($data_sql))
	{ //See if there is anything in the query
		$json_str .= "[\n";

		$row_count = 0;
		while ($data = mysql_fetch_assoc($data_sql))
		{
			if (count($data) > 1) $json_str .= "{\n";

			$count = 0;
			foreach ($data as $key => $value)
			{
				//If it is an associative array we want it in the format of "key":"value"
				if (count($data) > 1) $json_str .= "\"$key\":\"$value\"";
				else $json_str .= "\"$value\"";

				//Make sure that the last item don't have a ',' (comma)
				$count++;
				if ($count < count($data)) $json_str .= ",\n";
			}
			$row_count++;
			if (count($data) > 1) $json_str .= "}\n";

			//Make sure that the last item don't have a ',' (comma)
			if ($row_count < $total) $json_str .= ",\n";
		}

		$json_str .= "]\n";
	}

	//Replace the '\n's - make it faster - but at the price of bad redability.
	$json_str = str_replace("\n", "", $json_str); //Comment this out when you are debugging the script

	//Finally, output the data
	return $json_str;
}

