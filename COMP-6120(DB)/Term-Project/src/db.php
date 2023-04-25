<?php
// Logging credentials for DB 
$databasename = '*********';
$username = '***********';
$password = '***********';
$servername='sysmysql8.auburn.edu';

// Connecting to the database
function getConnection() {
global $username, $password, $databasename, $servername;
$con = mysqli_connect($servername, $username, $password, $databasename);
	if(!$con) {
		die('Could not connect: ' . mysqli_error(con));
	}
	return $con;
}

//Executing a query and returning the result
function executeQuery($con, $query) {
	$result = mysqli_query($con, $query);
	return $result;
}

//Counts no of rows
function countAffectedRows($con) {
	return mysqli_affected_rows($con);
}

//For errors
function getError($con) {
	return mysqli_error($con);
}
?>