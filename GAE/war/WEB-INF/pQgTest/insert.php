<?php
	
	// Test inits

	$noBaseLine = TRUE;
	include "$testLocation/open.php";

	pQgTest("insert into person ( firstName, lastName,     email,                salary )
						 values ( 'Piet',    'Pietersen',  'piet@gmail.com',     4500   ) ,
								( 'Wim',     'Willemsen',  'wim@gmail.com',      3000   ) ,
								( 'Jan',     'Janssen',    'jan@gmail.com',      2500   ) ");
	
	pQgTest("insert into person set lastName = 'Groot Jebbink', salary = 3000 + 3000");	
	
	// Test exits
	
    include "$testLocation/close.php";
	
?>