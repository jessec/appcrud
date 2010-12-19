<?php

	// Test inits
	
	$noBaseLine = TRUE;
	include "$testLocation/open.php";

	$slashes = "\\\\";
	
	$key = pQgTest("insert into person set lastName = '$slashes' ");
		   pQgTest("select id, lastName from person where lastName = '$slashes' ");
		   pQgTest("update person set lastName = '$slashes' where id = $key");
		   pQgTest("delete from person where lastName = '$slashes' ");

	// Test exits
	
    include "$testLocation/close.php";
		   

?>