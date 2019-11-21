<!DOCTYPE html>
<html>
	<head>
		<title>NodeMCU ESP8266 MySQL Database</title>
		<link rel="stylesheet" type="text/css" href="style.css">
    
		<link href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" rel="stylesheet">
    
		<link href="https://stackpath.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css" rel="stylesheet">
		<meta charset="utf-8">
		<!-- Script for updating pages without refreshing the page -->
		<script src="jquery.min.js"></script>
		<script>
			$(document).ready(function() {
				setInterval(function(){get_data()},5000);
				function get_data()
				{
					jQuery.ajax({
						type:"GET",
						url: "read_db.php",
						data:"",
						beforeSend: function() {
						},
						complete: function() {
						},
						success:function(data) {
							$("table").html(data);
						}
					});a
				}
			});
		</script>
		<style>
			table {
				border-collapse: collapse;
				width: 100%;
				color: #1f5380;
				font-family: monospace;
				font-size: 20px;
				text-align: left;
			} 
			th {
				background-color: #1f5380;
				color: white;
			}
			tr:nth-child(even) {background-color: #f2f2f2}
			tr:nth-child(odd) {color: white}
		</style>
	</head>
	<body>
	<nav class="navbar navbar-expand-lg py-3 navbar-dark bg-dark shadow-sm">
      <div class="container">
        <a href="#" class="navbar-brand"> 
          <span class="text-uppercase font-weight-bold">Smart Lamppost</span>
        </a>

        <div id="navbarSupportedContent" class="collapse navbar-collapse">
          <ul class="navbar-nav ml-auto">
            <li class="nav-item active"><a href="#" class="nav-link">Home <span class="sr-only">(current)</span></a></li>
          </ul>
        </div>
      </div>
	</nav>
		<h1 align="center" style="color:#f2f2f2;">LoLin NodeMCU ESP8266 MySQL Database</h1>
		<table>
			<tr>
				<th>No</th> 
				<th>Lamppost NO.01</th> 
				<th>Date</th>
				<th>Time</th>
				
			</tr>
		</table>
	</body>
</html>