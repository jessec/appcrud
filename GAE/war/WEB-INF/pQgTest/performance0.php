<?php

	$noBaseLine = TRUE;

	define('PQG_LOG',   TRUE  );
	define('PQG_CHECK', TRUE  );
		
	include "$testLocation/open.php";

	pQgTest("insert into person ( firstName, lastName,  email,	salary )
						 values ( '111',     '222',  	'333',  1000   ) ,
								( '444',     '555',  	'666',  2000   ) ");

    include "$testLocation/close.php";
	
?>