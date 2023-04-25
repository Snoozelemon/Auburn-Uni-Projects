<?php
	include 'db.php';
	$con = getConnection();
	if(!$con) {
		echo mysqli_error($con);
		die();
	}
?>

<!DOCTYPE html>
<html style="background-color: #1F2023;">
<head>
	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<link rel="icon" href="./img/query-icon.png" type="image/png">
	<title>Query Page</title>
	<link type="text/css" rel="stylesheet" href="style.css" />
</head>
<body>
	<div class="text parent" style="background-color:#1F2023;">
		<div class="topleft-div ">
			<h1 style="color: #FF6E31;"><img src="./img/au.png" style="width:25px;height:25px"> Term project by szc0231</h1>
		</div>
		<div class="topright-div ">
			<button class="button1" style="color:whitesmoke; background-color: transparent;border: 2px solid whitesmoke; border-radius:5px; height: auto;font-size: 20px;cursor: pointer;" onClick="window.location.href='https://webhome.auburn.edu/~szc0239/index.php';">Return<img src="./img/return.png"></button>
		</div>
	</div><br>
	<hr><br>
	

<!--		Bottom div 		 -->
	<div class="text">
		<div class="border-line" id="leftdiv" style="margin-top: 25px;">
<!-- 		table side 			-->
			<h3 style=" color: #9EA1D4;text-align:center;">List of Tables</h3>
				<form style="cursor: pointer;font-size: 20px;">
					<ul id="db_book">				db_book</ul>
					<ul id="db_customer">			db_customer</ul>
					<ul id="db_employee">			db_employee</ul>
					<ul id="db_order">				db_order</ul>
					<ul id="db_order_detail">		db_order_detail</ul>
					<ul id="db_shipper">			db_shipper</ul>
					<ul id="db_subject">			db_subject</ul>
					<ul id="db_supplier">			db_supplier</ul>					
				</form>
				<a href="https://webhome.auburn.edu/~szc0239/list_of_queries.php" style="padding: 20px;">Pre-tested queries</a> 
			<br>

		</div>
		<br>
<!-- sql side -->
		<div id="rightdiv" >
			<div class="border-line">
				<h3 style="color:yellow;padding-left: 15px;">SQL Editor</h3>
				<form method="POST" action="query_page.php">
					<textarea name="queryBox" id="input" rows="4" cols="50" spellcheck="false" placeholder="Type your code here...."><?= stripslashes($_POST['queryBox'])?></textarea><br>
					<input type="Submit" value="Submit" id="ExecuteButton" style="text-align:right;margin:0px 15px 5px 15px; color:whitesmoke; background-color: transparent; border: 2px solid whitesmoke; border-radius: 5px;font-size: 15px; cursor: pointer;"/>
					<input type="button" value="Clear" style="text-align:right;margin:0px 15px 5px 15px; color:whitesmoke; background-color: transparent; border: 2px solid whitesmoke;font-size: 15px; border-radius: 5px;cursor: pointer;" onclick="javascript:eraseText();"/>	
				</form>
				
 			</div>
 			
		</div>
		<div style="width:30%;height:auto;padding-left: 5px;">
			
				<h2 style="margin:10px;color: indianred;">Result</h2>
				<?php

				if(isset($_POST['queryBox'])) {
				    $query = $_POST['queryBox'];
				    $query = stripcslashes($query);
				    $stmt = strtolower($query);
				    $forbidden = array('drop', 'delete', 'update', 'create', 'alter');
				    if ($stmt == ' ' || $stmt == '' || $stmt == NULL) {
				    	echo "Wow! So empty....., You cannot submit	empty querie. Please Try again.\n";
				    	die();
				    }
				    foreach($forbidden as $key) {
				        if(strpos($stmt, $key) !== false) {
				            echo "Queries like DROP, DELETE, UPDATE, CREATE and ALTER are not supported. Please try again.";
				            die();
				        }
				    }
				    $result = executeQuery($con, $query);
				    if($result == false) {
				    	echo "I think you got your syntax wrong. please check the error and try again";
				        
				        die();
				    }

				    ?>
				    <table class="bordered" style="padding: 5px;">
				        <thead>
				        <?php
				        $numFields = mysqli_num_fields($result);

				        echo '<tr>';
				        for($i = 0; $i < $numFields; $i++) {
				            $field = mysqli_fetch_field_direct($result, $i);
				            echo '<th>' . $field->name . '</th>';
				        }
				        echo '</tr>';
				        ?>
				        </thead>

				        <?php
				        $rows = array();
				        while($resultRow = mysqli_fetch_assoc($result)) {
				            $rows[] = $resultRow;
				        }
				        foreach($rows as $row) {
				            echo '<tr>';
				            foreach($row as $col) {
				                echo '<td>' . $col . '</td>';
				            }
				            echo '</tr>';
				        }

				        mysqli_free_result($result);

				        ?>
				    </table>
				    
				<?php
				}
					$numRows = countAffectedRows($con);
					if($numRows == 0 || $numRows == 1){
						echo "<br>";
						echo "Total {$numRows} row have been affected.";
					}else{
						echo "<br>";
						echo "Total {$numRows} rows have been affected.";	
					}
					
				?>
			</div>
	<!-- 	result 				-->
	
	
</body>
</html>
<?php mysqli_close($con); ?>
<script src="script.js">
		

</script>