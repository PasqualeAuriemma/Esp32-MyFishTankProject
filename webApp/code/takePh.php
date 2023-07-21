<?php
  include("php/connection.php");
  include("php/queryAndFunction.php");

  if(!empty($_POST)){
    $ph = $_POST["Ph"];
    $ph = round($ph,2);
    $dataSend = substr($_POST["Date"], 0, 10);
    $query = getPHInsertQuery($ph, $dataSend);
    if ($con->query($query) === TRUE) {
      echo "OK dato aggiunto ".$dataSend;
    } else {
      echo "Error: " . $sql . "<br>" . $con->error;
    }
  }
   $con->close();

?>