<?php
  include("php/connection.php");
  include("php/queryAndFunction.php");
  
  if(!empty($_POST)){                  //($_GET)){
    $temperature = $_POST["Temp"];     // $_GET["Temp"];
    $dataSend = substr($_POST["Date"], 0, 10);
    $query = getTEMPInsertQuery($temperature, $dataSend);
    if ($con->query($query) === TRUE) {
      echo "Ok ".$temperature  . " " . $dataSend;
    } else {
      echo "Error: " . $sql . "<br>" . $con->error;
    }
  }
   $con->close();

?>