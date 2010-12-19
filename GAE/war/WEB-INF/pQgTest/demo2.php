<?php
		
	// Open the HTML page.
	
	echo "<html><body><pre>\n";
	
	// Import the JDO java class for pQg
	
	import myDatabase.person;

	// Define the database tables for pQg
	
	$pQgCfgClass  ['person'] = 'myDatabase.person';
	$pQgCfgKey    ['person'] = 'id';
	$pQgCfgFields ['person'] = array('id',   'firstName', 'lastName', 'email',  'salary');
	$pQgCfgTypes  ['person'] = array('Long', 'String',    'String',   'String', 'Double');	

	// Setting pQg paratmeters
	
	define('PQG_TEST',  TRUE   );
	define('PQG_CHECK', FALSE  );
	define('PQG_LOG',   FALSE  );
	
	// Include the pQg functions
	
	$pQgLocation = 'WEB-INF/pQg';
	include "$pQgLocation/pQg.inc";
	
	// Start our demo with a empty table.
		
	sql("delete from person");
	
	// Fill the database with the test set.
	
	$jan = sql("insert into person ( firstName, lastName,   email,              salary )
				            values ( 'Jan',    'Modaal',   'j.modaal@het.net',  2500   ) ");

	echo "Added Jan to the database with primary key $jan \n";
	
	$john = sql("insert into person ( firstName, lastName,   email,              salary )
				             values ( 'John',    'Doe',      'JOHN@GMAIL.COM',   5000   ) ");
	
	echo "Added John to the database with primary key $john \n";
	
	$jane = sql("insert into person ( firstName, lastName,   email,              salary )
				             values ( 'Jane',    'Doe',      'JANE@AOL.COM',     4000   ) ");

	echo "Added Jane to the database with primary key $jane \n";
	
	// Let's see what is in the table right now
	
	pQgDumpTable('person');
		
	// getField, get a single field.
	
	$salary = sqlField(" select salary from person where firstName = 'John' and lastName = 'Doe' ");
	
	echo "The salary of John is $salary \n\n";
	
	// getRecord, retrieve fields from 1 records.
	
	$record = sqlRecord("SeLeCt strtoupper(firstName) aS FIRST, strtoupper(lastName) As LAST
		                 FrOm   person wHeRe id = $jane ");

	echo "Who is number $jane? It is: " . $record['FIRST'] . ' ' . $record['LAST'] . "\n\n";
	
	// Jane is getting a raise.
	
	sql("update person set salary = salary + 1500 where id = $jane");
	
	// Make the emails spam proof
	
	$count = sql("update person set email = strtolower(str_replace('@', ' at ', email))");

	echo "Transform emails: Updated $count record(s) \n\n";
	
	// Jan is fired
	
	$count = sql("delete from person where id = $jan");
	
	echo "Fire Jan: Deleted $count record(s) \n";
	
	// This is the end result.
	
	pQgDumpTable('person');
	
	// Close the Datastore connection (the open is done on first usage)
	
	pQgClose();
	
	// Close the HTML page.
	
	echo '</pre><a href="/">Back</a></body></html>';

?>