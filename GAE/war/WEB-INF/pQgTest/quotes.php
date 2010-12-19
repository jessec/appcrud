<?php

	// Test inits
	
	$noBaseLine = TRUE;
	include "$testLocation/open.php";

	$single = "\'\'\'";
	$double = '"""';
	
	$a = pQgTest("insert into person set lastName = '$single' ");
	$b = pQgTest("insert into person set lastName = '$double' ");
	
	pQgTest("select id, lastName from person where lastName = '$single' ");
	pQgTest("select id, lastName from person where lastName = '$double' ");
	
	pQgTest("update person set lastName = '$single' where id = $b");
	pQgTest("update person set lastName = '$double' where id = $a");
	
	pQgTest("select id, lastName from person where lastName = '$single' ");
	pQgTest("select id, lastName from person where lastName = '$double' ");
	
	pQgTest("delete from person where lastName = '$single' ");
	pQgTest("delete from person where lastName = '$double' ");

	// Test exits
	
    include "$testLocation/close.php";
	
?>