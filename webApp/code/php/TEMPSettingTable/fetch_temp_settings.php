<?php 
session_start();
include('../connection.php');
include("../queryAndFunction.php");

$output= array();
$sql = getFromTableQuery("temperature");

$totalQuery = mysqli_query($con,$sql);
$total_all_rows = mysqli_num_rows($totalQuery);

if(isset($_POST['search']['value'])){
	$search_value = $_POST['search']['value'];
    $sql = getSeachFromTableQuery($sql, $search_value, "temperature");
}

if(isset($_POST['order'])){
	$column_name = $_POST['order'][0]['column'];
	$order = $_POST['order'][0]['dir'];
	$sql = getOrderFromTableQuery($sql, $column_name, $order);
}else{
	$sql = getOrderFromTableQuery($sql, "id", "desc");
}

if($_POST['length'] != -1){
	$start = $_POST['start'];
	$length = $_POST['length'];
	$sql = getLimitFromTableQuery($sql, $start, $length);
}	

$query = mysqli_query($con,$sql);
$count_rows = mysqli_num_rows($query);
$data = array();

while($row = mysqli_fetch_assoc($query)){
	$sub_array = array();
	$sub_array[] = $row['id'];
	$sub_array[] = $row['temperature'];
	$sub_array[] = $row['data_send'];
	$sub_array[] = date_format(date_create($row['data_arrive']),"d/m/Y  H:m:s");
    $sub_array[] = '<a href="javascript:void();" data-id="'.$row['id'].'"  class="btn btn-info btn-sm editbtnTempS" ><i class="mdi mdi-table-edit"></i></a>  <a href="javascript:void();" data-id="'.$row['id'].'"  class="btn btn-danger btn-sm deleteBtnTempS" ><i class="mdi mdi-table-row-remove"></i></a>';
	
    $data[] = $sub_array;
}

$output = array(
	'draw'=> intval($_POST['draw']),
	'recordsTotal' =>$count_rows ,
	'recordsFiltered'=>   $total_all_rows,
	'data'=>$data,
);
echo  json_encode($output);
   $con->close();  
?>