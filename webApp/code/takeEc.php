<?php
  include("php/connection.php");
  include("php/queryAndFunction.php");
  
  if(!empty($_POST)){
    $ec = $_POST["Ec"];
    $dataSend = substr($_POST["Date"], 0, 10);
    $tds = $ec * 0.64;
    $queryEC = getECInsertQuery($ec, $dataSend);
    if ($con->query($queryEC) === TRUE) {
       echo "Ok ".$tds  . " " . $dataSend;
    } else {
      echo "Error: " . $sql . "<br>" . $con->error;
    }
    $query = getTDSInsertQuery($tds, $dataSend);
    if ($con->query($query) === TRUE) {
      echo "Ok ".$ec  . " " . $dataSend;
    } else {
      echo "Error: " . $sql . "<br>" . $con->error;
    }
  }
   $con->close();
?>