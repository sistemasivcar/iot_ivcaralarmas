<?php
session_start();
$logged = $_SESSION['logged'];
$name = $_SESSION['user_name'];
$user_id = $_SESSION['user_id'];
$conn = mysqli_connect("localhost","admin_alarmaiot","Gaston2001","admin_alarmaiot");

// TRAIGO TODOS LOS DISPOSITOVOS
$result = $conn->query("SELECT * FROM `devices` WHERE `devices_user_id` = '".$user_id."'");
$devices = $result->fetch_all(MYSQLI_ASSOC);
$count = count($devices);


include('application/app/Views/template/head.php');
include('application/app/Views/template/navigation.php');
include('application/app/Views/template/header.php');
include('application/app/Views/template/footer.php');
?>
<div ui-view class="app-body" id="view">


    <?php

   if ($count == 0) {
       ?> <div ui-view class="app-body" id="view"><?php
     echo "<br><br><font size='5'><div style='color:red' align='center'>No tenés <strong>dispositivos...</strong></div></font>";
   }else if ($count == 1){
     include("content/historial_one.php");
   } else if ($count == 2){
     include("content/historial_two.php");
   }else{
     echo "<br><br><font size='5'><div style='color:red' align='center'>No tenés <strong>dispositivos...</strong></div></font>";
   }
   ?></div>
   <?php

include('application/app/Views/template/end.php');

 ?>
