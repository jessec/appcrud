<?php

	// Test inits
	
	include "$testLocation/open.php";

	// Lets leveling it, the person with the lowest salary gets a raise of one third.
	// On the other hand the person with the highest salary gets a decrese of one third.
	
	pQgTest("update person set salary = salary * ( 1 + (1/3) ) order by salary asc  limit 1");
	pQgTest("update person set salary = salary * ( 1 - (1/3) ) order by salary desc limit 1");

	// Wim is the new manager.

	pQgTest("update person set salary = 7500 where firstName = 'Wim' ");
	
	// Ok, there is no SQL engine, but we have a PHP engine :-)
	
	pQgTest("update person set firstName = substr(firstName, 0, 1) . '.'    ,
							   lastName  = strtoupper(lastName)             , 
							   email     = str_replace('@', ' at ', email)  ");
	
	// Test exits
	
	include "$testLocation/close.php";

?>