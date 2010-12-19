<html>
	<head>
		<title>pQg - PHP with SQL on Google App Engine</title>
		<meta name="description" CONTENT="pQg - PHP with SQL on Google App Engine">
		<meta name="keywords" CONTENT="pQg, PHP, JDO, SQL, GAE, Google App Engine, Google, App Engine, Quercus">
	</head>
	<body>
<?php 

	$testLocation = './WEB-INF/pQgTest';		

	include "$testLocation/functions.php";

	if (isset($_REQUEST['action']) ) $action = $_REQUEST['action'];
	else                             $action = '';

	if (isset($_REQUEST['file']) )   $file   = $_REQUEST['action'];
	else                             $file   = '';
	
	if ($file) {
		$file = "$testLocation/" . strtr($_REQUEST['file'],  "~!@#$%^&*()_+-={}|[]/\\:\";'<>?,.") . '.php';
		if ( ! file_exists($file) )
			$file = '';
	}

	if     ( $action == 'run'  and $file )  	include     $file;
	elseif ( $action == 'view' and $file )  	pQgTestView ($file);
	elseif ( $action == 'test'           )  	include     ("$testLocation/menu_test.html");
	else										include     ("$testLocation/menu_main.html");

?>
		<script type="text/javascript">
			var gaJsHost = (("https:" == document.location.protocol) ? "https://ssl." : "http://www.");
			document.write(unescape("%3Cscript src='" + gaJsHost + "google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E"));
		</script>
		<script type="text/javascript">
			try {
				var pageTracker = _gat._getTracker("UA-84433-6");
				pageTracker._trackPageview();
			} catch(err) {}
		</script>		
	</body>
</html>