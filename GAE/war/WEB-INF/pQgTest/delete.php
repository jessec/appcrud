<?php
	
	// Test inits
	
	include "$testLocation/open.php";

	// Delete John
	
	pQgTest("delete from person where firstName = 'Jan'");	

	// Delete the person with the highest salary :-)
	
	pQgTest("delete from person order by salary desc limit 1");

	// Delete all other records
	
	pQgTest("delete from person");

	// Test exits
	
    include "$testLocation/close.php";
	
?>