<?php 

	// Open the HTML page.
	
	echo "<h3>pQg - test - $file</h3><hr><pre>";
	
	// pQg - Import the JDO java classes for the database tables.
	
	import myDatabase.*;
	
	// pQg - Database structure
	
	$pQgCfgClass  ['person'] = 'myDatabase.person';
	$pQgCfgKey    ['person'] = 'id';
	$pQgCfgFields ['person'] = array('id',   'firstName', 'lastName', 'email',  'salary');
	$pQgCfgTypes  ['person'] = array('Long', 'String',    'String',   'String', 'Double');	

	$pQgCfgClass  ['personCheck'] = 'myDatabase.personCheck';
	$pQgCfgKey    ['personCheck'] = 'id';
	$pQgCfgFields ['personCheck'] = array('id',   'firstName');
	$pQgCfgTypes  ['personCheck'] = array('Long', 'String');	

	// pQg - settings
	
	defined('PQG_CHECK') or define('PQG_CHECK', FALSE  );
	defined('PQG_LOG'  ) or define('PQG_LOG',   FALSE  );
	defined('PQG_JDO'  ) or define('PQG_JDO',   TRUE  );
	
	// pQg - Include 
	
	$pQgLocation = './WEB-INF/pQg';
	include "$pQgLocation/pQg.inc";

	// Clear the (if existing) previous test
	
	if ( ! isset($noDelete) )
  		sql("truncate person");
	
	// Create the baseline for our test
	
	if ( ! isset($noBaseLine) ) {
		
		sql("insert into person ( firstName, lastName,     email,                salary )
						 values ( 'Piet',    'Pietersen',  'piet@gmail.com',     4500   ) ,
								( 'Klaas',   'Klaassens',  'klaas@gmail.com',    3500   ) ,
								( 'Hendrik', 'Hendriksen', 'hendrik@gmail.com',  5500   ) ,
								( 'Wim',     'Willemsen',  'wim@gmail.com',      3000   ) ,
								( 'Jan',     'Janssen',    'jan@gmail.com',      2500   ) ");

		echo "<b>The baseline for this test:</b>\n";
		
		pQgDumpTable("person");

		echo "<hr>";
		
	}
	
?>