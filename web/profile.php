
<?php
session_start();
$logged = $_SESSION['logged'];
$user_id = $_SESSION['user_id'];
$name = $_SESSION['user_name'];
$phone = $_SESSION['user_phone'];
$email = $_SESSION['user_email'];
$date = $_SESSION['user_fecha'];

$conn = mysqli_connect("localhost","admin_alarmaiot","Gaston2001","admin_alarmaiot");

$result = $conn->query("SELECT * FROM `admin_alarmaiot`.`devices` WHERE `devices_user_id` = '".$user_id."' ");
$disp = $result->fetch_all(MYSQLI_ASSOC); $count = count($disp);



if(!$logged){
    echo '<meta http-equiv="refresh" content="0; url=login.php">';
  die();
}

include('application/app/Views/template/head.php');
include('application/app/Views/template/navigation.php');
include('application/app/Views/template/header.php');
include('application/app/Views/template/footer.php');

include('content/profile.php');

  
include('application/app/Views/template/end.php');?>

